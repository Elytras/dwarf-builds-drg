"""Cooked UE 4.27 Texture2D (.uasset + .uexp [+ .ubulk]) to RGBA, PNG writer, small image helpers. Stdlib only.

Formats: PF_B8G8R8A8, PF_R8G8B8A8, PF_G8, PF_DXT1, PF_DXT3, PF_DXT5, PF_BC4, PF_BC5, PF_BC7, PF_FloatRGBA.
UI textures are sRGB; the bytes are kept as they are, which is right for the web.
CLI: python -m gamedata.texture info|png <file.uasset> [out.png]
"""
import os
import struct
import sys
import zlib

from .assets import Package, Reader

BULK_AT_END = 0x0001
BULK_ZLIB = 0x0002
BULK_UNUSED = 0x0020
BULK_INLINE = 0x0040
BULK_SEPARATE = 0x0100
BULK_OPTIONAL = 0x0800
BULK_SIZE64 = 0x2000


# ---------------------------------------------------------------- PNG writer and image helpers
PNG_TYPES = {1: 0, 2: 4, 3: 2, 4: 6}  # channels -> PNG colour type (gray, gray+alpha, RGB, RGBA)


def write_png(path, w, h, pixels, channels=4):
    """Write 8-bit pixels (row-major, `channels` bytes per pixel). Filter 'Up' keeps glyph PNGs small."""
    stride = w * channels
    raw = bytearray()
    prev = bytes(stride)
    for y in range(h):
        row = bytes(pixels[y * stride:(y + 1) * stride])
        if row == prev:
            raw += b"\x02" + bytes(stride)
        else:
            raw += b"\x00" + row
        prev = row

    def chunk(tag, data):
        c = struct.pack(">I", len(data)) + tag + data
        return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)

    png = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, PNG_TYPES[channels], 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(bytes(raw), 9)) + chunk(b"IEND", b"")
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "wb") as f:
        f.write(png)


def mask_alpha(rgba, bright=True):
    """Alpha mask of a glyph. bright=True keeps only the light parts (alpha * max(r,g,b)),
    so black insides (OC frames) and black glyphs on white (class hexagons) become transparent."""
    a = rgba[3::4]
    if not bright:
        return bytes(a)
    m = map(max, rgba[0::4], rgba[1::4], rgba[2::4])
    return bytes((x * y + 127) // 255 for x, y in zip(a, m))


def white_mask(alpha):
    """Gray+alpha pixels: white with the given alpha."""
    out = bytearray(len(alpha) * 2)
    out[0::2] = b"\xff" * len(alpha)
    out[1::2] = alpha
    return out


def rgb_of(rgba):
    out = bytearray(len(rgba) // 4 * 3)
    out[0::3], out[1::3], out[2::3] = rgba[0::4], rgba[1::4], rgba[2::4]
    return out


# ---------------------------------------------------------------- cooked header
class Mip:
    pass


def read_bulk(r, pkg_path, header_size):
    flags = r.u32()
    if flags & BULK_SIZE64:
        count, disk = r.i64(), r.i64()
    else:
        count, disk = r.i32(), r.i32()
    off = r.i64()
    m = Mip()
    m.flags, m.count, m.disk, m.off = flags, count, disk, off
    if flags & BULK_UNUSED or count == 0:
        m.where, m.data = "empty", b""
    elif flags & BULK_INLINE:
        m.where, m.data = "uexp-inline", r.d[r.p:r.p + disk]
        r.p += disk
    elif flags & BULK_OPTIONAL:
        m.where, m.data = "uptnl", None
    elif flags & BULK_SEPARATE:
        m.where = "ubulk"
        with open(pkg_path[:-7] + ".ubulk", "rb") as f:
            f.seek(off)
            m.data = f.read(disk)
    elif flags & BULK_AT_END:
        # Offset counts from the start of .uasset; payload sits in .uexp after the exports.
        m.where = "uexp-end"
        with open(pkg_path[:-7] + ".uexp", "rb") as f:
            f.seek(off - header_size)
            m.data = f.read(disk)
    else:
        m.where, m.data = "unknown(0x%x)" % flags, None
    if m.data is not None and flags & BULK_ZLIB:
        m.data = zlib.decompress(m.data)  # not seen for UI textures
    return m


class Texture:
    def __init__(self, path):
        self.path = path
        pkg = Package(path)
        exp = [e for e in pkg.exports if pkg.ref(e["cls"]).endswith(".Texture2D")]
        if not exp:
            raise ValueError("no Texture2D export in %s" % path)
        e = exp[0]
        self.name = e["name"]
        r = Reader(pkg.body, pkg.names)
        r.p = e["off"] - pkg.body_base
        self.props = dict(pkg.read_props(r))
        end = e["off"] - pkg.body_base + e["size"]
        if r.i32():                      # UObject: has object guid
            r.p += 16
        r.p += 2                         # UTexture FStripDataFlags
        r.p += 2                         # UTexture2D FStripDataFlags
        if not r.i32():
            raise ValueError("not cooked")
        self.formats = []
        while True:
            pf = r.fname()
            if pf == "None":
                break
            skip = r.i64()
            sx, sy, packed = r.i32(), r.i32(), r.u32()
            fmt = r.fstring()
            if packed & (1 << 30):       # optional data
                r.p += 8
            first_mip = r.i32()
            nmips = r.i32()
            mips = []
            for _ in range(nmips):
                r.i32()                  # bCooked
                m = read_bulk(r, path, pkg.header_size)
                m.w, m.h, m.d = r.i32(), r.i32(), r.i32()
                mips.append(m)
            virtual = r.i32()
            self.formats.append(dict(pf=fmt, w=sx, h=sy, slices=packed & 0x3FFFFFFF,
                                     first_mip=first_mip, mips=mips, virtual=virtual))
            if r.p > end:
                break
        f = self.formats[0]
        self.pf, self.w, self.h, self.mips = f["pf"], f["w"], f["h"], f["mips"]

    def info(self):
        srgb = self.props.get("SRGB", True)
        comp = self.props.get("CompressionSettings", "TC_Default")
        where = ",".join(sorted({m.where for m in self.mips}))
        return "%-48s %-13s %4dx%-4d mips=%-2d %-14s comp=%s srgb=%s lod=%s %s" % (
            self.name, self.pf, self.w, self.h, len(self.mips), where, comp, srgb,
            self.props.get("LODGroup", "-"), "NeverStream" if self.props.get("NeverStream") else "")

    def rgba(self, mip=None):
        """(w, h, RGBA bytearray) of the given mip, default: the largest mip with pixel data."""
        if mip is None:
            mip = next((i for i, m in enumerate(self.mips) if m.data), 0)
        m = self.mips[mip]
        if m.data is None:
            raise ValueError("mip %d payload in %s, not available" % (mip, m.where))
        return m.w, m.h, decode(self.pf, m.data, m.w, m.h)


# ---------------------------------------------------------------- decoders
def _565(c):
    r = (c >> 11) & 31; g = (c >> 5) & 63; b = c & 31
    return (r << 3 | r >> 2, g << 2 | g >> 4, b << 3 | b >> 2)


def _color_block(blk, out, x0, y0, w, h, alpha_from_c, force4=False):
    c0, c1, bits = struct.unpack_from("<HHI", blk, 0)
    p0, p1 = _565(c0), _565(c1)
    if c0 > c1 or force4:
        pal = [p0 + (255,), p1 + (255,),
               tuple((2 * a + b) // 3 for a, b in zip(p0, p1)) + (255,),
               tuple((a + 2 * b) // 3 for a, b in zip(p0, p1)) + (255,)]
    else:
        pal = [p0 + (255,), p1 + (255,), tuple((a + b) // 2 for a, b in zip(p0, p1)) + (255,), (0, 0, 0, 0)]
    for i in range(16):
        x, y = x0 + (i & 3), y0 + (i >> 2)
        if x < w and y < h:
            o = (y * w + x) * 4
            px = pal[(bits >> (2 * i)) & 3]
            out[o:o + 3] = bytes(px[:3])
            if alpha_from_c:
                out[o + 3] = px[3]


def _alpha_block(blk):
    a0, a1 = blk[0], blk[1]
    bits = int.from_bytes(blk[2:8], "little")
    if a0 > a1:
        pal = [a0, a1] + [((7 - i) * a0 + i * a1) // 7 for i in range(1, 7)]
    else:
        pal = [a0, a1] + [((5 - i) * a0 + i * a1) // 5 for i in range(1, 5)] + [0, 255]
    return [pal[(bits >> (3 * i)) & 7] for i in range(16)]


def decode_bc1_3_5(pf, data, w, h):
    out = bytearray(w * h * 4)
    bs = 8 if pf == "PF_DXT1" else 16
    bw, bh = (w + 3) // 4, (h + 3) // 4
    p = 0
    for by in range(bh):
        for bx in range(bw):
            blk = data[p:p + bs]; p += bs
            x0, y0 = bx * 4, by * 4
            if pf == "PF_DXT1":
                _color_block(blk, out, x0, y0, w, h, True)
                continue
            if pf == "PF_DXT3":
                al = [((int.from_bytes(blk[0:8], "little") >> (4 * i)) & 15) * 17 for i in range(16)]
            else:
                al = _alpha_block(blk[0:8])
            _color_block(blk[8:16], out, x0, y0, w, h, False, force4=True)
            for i in range(16):
                x, y = x0 + (i & 3), y0 + (i >> 2)
                if x < w and y < h:
                    out[(y * w + x) * 4 + 3] = al[i]
    return out


def decode_bc4_5(pf, data, w, h):
    out = bytearray(w * h * 4)
    bs = 8 if pf == "PF_BC4" else 16
    bw, bh = (w + 3) // 4, (h + 3) // 4
    p = 0
    for by in range(bh):
        for bx in range(bw):
            blk = data[p:p + bs]; p += bs
            r = _alpha_block(blk[0:8])
            g = _alpha_block(blk[8:16]) if bs == 16 else r
            b = [0] * 16 if bs == 16 else r
            for i in range(16):
                x, y = bx * 4 + (i & 3), by * 4 + (i >> 2)
                if x < w and y < h:
                    o = (y * w + x) * 4
                    out[o:o + 4] = bytes((r[i], g[i], b[i], 255))
    return out


# BC7 tables
_P2 = [0xCCCC, 0x8888, 0xEEEE, 0xECC8, 0xC880, 0xFEEC, 0xFEC8, 0xEC80, 0xC800, 0xFFEC, 0xFE80, 0xE800, 0xFFE8, 0xFF00,
       0xFFF0, 0xF000, 0xF710, 0x008E, 0x7100, 0x08CE, 0x008C, 0x7310, 0x3100, 0x8CCE, 0x088C, 0x3110, 0x6666, 0x366C,
       0x17E8, 0x0FF0, 0x718E, 0x399C, 0xAAAA, 0xF0F0, 0x5A5A, 0x33CC, 0x3C3C, 0x55AA, 0x9696, 0xA55A, 0x73CE, 0x13C8,
       0x324C, 0x3BDC, 0x6996, 0xC33C, 0x9966, 0x0660, 0x0272, 0x04E4, 0x4E40, 0x2720, 0xC936, 0x936C, 0x39C6, 0x639C,
       0x9336, 0x9CC6, 0x817E, 0xE718, 0xCCF0, 0x0FCC, 0x7744, 0xEE22]
_P3 = [
    "0011001102210222", "0001001122112221", "0000200122112211", "0222002200110111", "0000000011221122",
    "0011001100220022", "0022002211111111", "0011001122112211", "0000000011112222", "0000111111112222",
    "0000111122222222", "0012001200120012", "0112011201120112", "0122012201220122", "0011011211221222",
    "0011200122002220", "0001001101121122", "0111001120012200", "0000112211221122", "0022002200221111",
    "0111011102220222", "0001000122212221", "0000001101220122", "0000110022102210", "0122012200110000",
    "0012001211222222", "0110122112210110", "0000011012211221", "0022110211020022", "0110011020022222",
    "0011012201220011", "0000200022112221", "0000000211221222", "0222002200120011", "0011001200220222",
    "0120012001200120", "0000111122220000", "0120120120120120", "0120201212010120", "0011220011220011",
    "0011112222000011", "0101010122222222", "0000000021212121", "0022112200221122", "0022001100220011",
    "0220122102201221", "0101222222221212", "0000212121212121", "0101010101012222", "0222011102220111",
    "0002111200021112", "0000211221122112", "0222011101110222", "0002111211121112", "0110011001102222",
    "0000000021122112", "0110011022222222", "0022001100110022", "0022112211220022", "0000000000002112",
    "0002000100020001", "0222122202221222", "0101222222222222", "0111201122012220"]
_A2 = [15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 2, 8, 2, 2, 8, 8, 15, 2, 8, 2, 2, 8, 8, 2, 2,
       15, 15, 6, 8, 2, 8, 15, 15, 2, 8, 2, 2, 2, 15, 15, 6, 6, 2, 6, 8, 15, 15, 2, 2, 15, 15, 15, 15, 15, 2, 2, 15]
_A3a = [3, 3, 15, 15, 8, 3, 15, 15, 8, 8, 6, 6, 6, 5, 3, 3, 3, 3, 8, 15, 3, 3, 6, 10, 5, 8, 8, 6, 8, 5, 15, 15, 8, 15,
        3, 5, 6, 10, 8, 15, 15, 3, 15, 5, 15, 15, 15, 15, 3, 15, 5, 5, 5, 8, 5, 10, 5, 10, 8, 13, 15, 12, 3, 3]
_A3b = [15, 8, 8, 3, 15, 15, 3, 8, 15, 15, 15, 15, 15, 15, 15, 8, 15, 8, 15, 3, 15, 8, 15, 8, 3, 15, 6, 10, 15, 15, 10,
        8, 15, 3, 15, 10, 10, 8, 9, 10, 6, 15, 8, 15, 3, 6, 6, 8, 15, 3, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 3, 15,
        15, 8]
_W = {2: [0, 21, 43, 64], 3: [0, 9, 18, 27, 37, 46, 55, 64], 4: [0, 4, 9, 13, 17, 21, 26, 30, 34, 38, 43, 47, 51, 55, 60, 64]}
# mode: (subsets, partbits, rotbits, isbbits, colbits, alphabits, pbits(endpoint), sharedpbit, ib, ib2)
_MODES = [(3, 4, 0, 0, 4, 0, 1, 0, 3, 0), (2, 6, 0, 0, 6, 0, 0, 1, 3, 0), (3, 6, 0, 0, 5, 0, 0, 0, 2, 0),
          (2, 6, 0, 0, 7, 0, 1, 0, 2, 0), (1, 0, 2, 1, 5, 6, 0, 0, 2, 3), (1, 0, 2, 0, 7, 8, 0, 0, 2, 2),
          (1, 0, 0, 0, 7, 7, 1, 0, 4, 0), (2, 6, 0, 0, 5, 5, 1, 0, 2, 0)]


def _bc7_block(blk):
    v = int.from_bytes(blk, "little")
    mode = 0
    while mode < 8 and not (v >> mode) & 1:
        mode += 1
    if mode == 8:
        return [(0, 0, 0, 0)] * 16
    pos = mode + 1

    def take(n):
        nonlocal pos
        x = (v >> pos) & ((1 << n) - 1)
        pos += n
        return x

    ns, pb, rb, isb, cb, ab, epb, spb, ib, ib2 = _MODES[mode]
    part = take(pb)
    rot = take(rb)
    isel = take(isb)
    ne = ns * 2
    col = [[0] * 4 for _ in range(ne)]
    for c in range(3):
        for e in range(ne):
            col[e][c] = take(cb)
    if ab:
        for e in range(ne):
            col[e][3] = take(ab)
    else:
        for e in range(ne):
            col[e][3] = 255
    cbits, abits = cb, ab
    if epb:
        for e in range(ne):
            p = take(1)
            for c in range(4 if ab else 3):
                col[e][c] = (col[e][c] << 1) | p
        cbits += 1
        if ab:
            abits += 1
    elif spb:
        for s in range(ns):
            p = take(1)
            for e in (2 * s, 2 * s + 1):
                for c in range(3):
                    col[e][c] = (col[e][c] << 1) | p
        cbits += 1
    for e in range(ne):
        for c in range(3):
            x = col[e][c] << (8 - cbits)
            col[e][c] = x | (x >> cbits)
        if ab:
            x = col[e][3] << (8 - abits)
            col[e][3] = x | (x >> abits)
    if ns == 1:
        sub = [0] * 16; anchors = [0]
    elif ns == 2:
        m = _P2[part]
        sub = [(m >> i) & 1 for i in range(16)]; anchors = [0, _A2[part]]
    else:
        sub = [int(ch) for ch in _P3[part]]; anchors = [0, _A3a[part], _A3b[part]]
    idx = [0] * 16
    for i in range(16):
        n = ib - (1 if i in anchors else 0)
        idx[i] = take(n)
    idx2 = None
    if ib2:
        idx2 = [0] * 16
        for i in range(16):
            n = ib2 - (1 if i == 0 else 0)
            idx2[i] = take(n)
    out = []
    w1, w2 = _W[ib], _W[ib2] if ib2 else None
    for i in range(16):
        s = sub[i]
        e0, e1 = col[2 * s], col[2 * s + 1]
        if idx2 is None:
            wc = wa = w1[idx[i]]
        elif isel:
            wc, wa = w2[idx2[i]], w1[idx[i]]
        else:
            wc, wa = w1[idx[i]], w2[idx2[i]]
        r = ((64 - wc) * e0[0] + wc * e1[0] + 32) >> 6
        g = ((64 - wc) * e0[1] + wc * e1[1] + 32) >> 6
        b = ((64 - wc) * e0[2] + wc * e1[2] + 32) >> 6
        a = ((64 - wa) * e0[3] + wa * e1[3] + 32) >> 6
        if rot == 1: a, r = r, a
        elif rot == 2: a, g = g, a
        elif rot == 3: a, b = b, a
        out.append((r, g, b, a))
    return out


def decode_bc7(data, w, h):
    out = bytearray(w * h * 4)
    bw, bh = (w + 3) // 4, (h + 3) // 4
    p = 0
    cache = {}
    for by in range(bh):
        for bx in range(bw):
            blk = bytes(data[p:p + 16]); p += 16
            px = cache.get(blk)
            if px is None:
                px = _bc7_block(blk)
                if len(cache) < 65536:
                    cache[blk] = px
            for i in range(16):
                x, y = bx * 4 + (i & 3), by * 4 + (i >> 2)
                if x < w and y < h:
                    o = (y * w + x) * 4
                    out[o:o + 4] = bytes(px[i])
    return out


def _half(hv):
    s = -1.0 if hv & 0x8000 else 1.0
    e = (hv >> 10) & 31; m = hv & 1023
    if e == 0:
        return s * m / 16777216.0
    if e == 31:
        return s * 65504.0
    return s * (1 + m / 1024.0) * 2 ** (e - 15)


def decode(pf, data, w, h):
    n = w * h
    if pf == "PF_B8G8R8A8":
        out = bytearray(data[:n * 4])
        out[0::4], out[2::4] = data[2:n * 4:4], data[0:n * 4:4]
        return out
    if pf == "PF_R8G8B8A8":
        return bytearray(data[:n * 4])
    if pf == "PF_G8":
        out = bytearray(n * 4)
        for c in range(3):
            out[c::4] = data[:n]
        out[3::4] = b"\xff" * n
        return out
    if pf in ("PF_DXT1", "PF_DXT3", "PF_DXT5"):
        return decode_bc1_3_5(pf, data, w, h)
    if pf in ("PF_BC4", "PF_BC5"):
        return decode_bc4_5(pf, data, w, h)
    if pf == "PF_BC7":
        return decode_bc7(data, w, h)
    if pf == "PF_FloatRGBA":
        out = bytearray(n * 4)
        vals = struct.unpack_from("<%dH" % (n * 4), data)
        for i, hv in enumerate(vals):
            f = _half(hv)
            if i % 4 != 3:   # linear -> sRGB
                f = 12.92 * f if f <= 0.0031308 else 1.055 * max(f, 0) ** (1 / 2.4) - 0.055
            out[i] = max(0, min(255, int(f * 255 + 0.5)))
        return out
    raise NotImplementedError(pf)


# ---------------------------------------------------------------- CLI
def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("info", "png"):
        sys.exit(__doc__)
    if sys.argv[1] == "info":
        for p in sys.argv[2:]:
            try:
                print(Texture(p).info())
            except Exception as ex:
                print("%s: %s" % (os.path.basename(p), ex))
    else:
        t = Texture(sys.argv[2])
        w, h, px = t.rgba()
        write_png(sys.argv[3], w, h, px)
        print("%s -> %s (%s %dx%d)" % (t.name, sys.argv[3], t.pf, w, h))


if __name__ == "__main__":
    main()
