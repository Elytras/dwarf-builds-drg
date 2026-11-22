"""Icon registry: maps game textures to web PNGs under Web/public/img and writes exactly those.

Modes: 'mask'  white glyph, alpha = alpha * brightness (black parts become transparent)
       'fill'  white glyph, alpha = texture alpha (the whole shape)
       'color' the texture's own colours (RGB when opaque, else RGBA)
       'render' like 'color', at the largest mip not wider than RENDER_WIDTH (the 2nd mip of the
                1024 wide GearGraphic_* weapon renders)
"""
import os
import shutil

from .assets import object_name, package_of
from .texture import Texture, mask_alpha, rgb_of, white_mask, write_png

KINDS = ("class", "items", "render", "upg", "perks", "frames")
STRIP = ("Icon_Upgrade_", "Icon_Overclock_", "Icon_")
RENDER_WIDTH = 512


def render_mip(tex):
    """Index of the largest mip with pixel data that is at most RENDER_WIDTH wide (else the largest one)."""
    ok = [i for i, m in enumerate(tex.mips) if m.data]
    small = [i for i in ok if tex.mips[i].w <= RENDER_WIDTH]
    return small[0] if small else (ok[0] if ok else 0)


class Icons:
    def __init__(self, store):
        self.store = store
        self.entries = {}   # web path -> (texture path, mode)
        self.glyphs = {}    # texture path -> web path (upgrade/OC glyphs)
        self.problems = []

    def add(self, kind, name, texture, mode="mask"):
        """Register img/<kind>/<name>.png for a texture path; returns the web path or None."""
        if not texture or texture == "None":
            return None
        texture = package_of(texture)
        web = "img/%s/%s.png" % (kind, name)
        old = self.entries.get(web)
        if old and old != (texture, mode):
            raise ValueError("icon name clash %s: %s vs %s" % (web, old, (texture, mode)))
        self.entries[web] = (texture, mode)
        return web

    def glyph(self, texture):
        """Upgrade / overclock glyph, named after the texture without the Icon_Upgrade_ prefix."""
        if not texture or texture == "None":
            return None
        texture = package_of(texture)
        if texture not in self.glyphs:
            base = object_name(texture)
            for p in STRIP:
                if base.startswith(p):
                    base = base[len(p):]
                    break
            name, n = base, 2
            while "img/upg/%s.png" % name in self.entries:
                name, n = "%s_%d" % (base, n), n + 1
            self.glyphs[texture] = self.add("upg", name, texture)
        return self.glyphs[texture]

    def write(self, img_dir):
        """Decode and write every registered icon. Returns [(web path, w, h, channels, pixels)]."""
        for kind in KINDS:
            shutil.rmtree(os.path.join(img_dir, kind), ignore_errors=True)
        self.store.prefetch([t for t, _ in self.entries.values()])
        decoded, out = {}, []
        for web, (tex, mode) in sorted(self.entries.items()):
            key = (tex, mode == "render")
            if key not in decoded:
                asset_file = self.store.file(tex)
                try:
                    t = Texture(asset_file) if os.path.exists(asset_file) else None
                    decoded[key] = t and (t.rgba(render_mip(t)) if mode == "render" else t.rgba())
                except Exception as ex:  # report, keep going
                    self.problems.append("%s: cannot decode %s (%s)" % (web, tex, ex))
                    decoded[key] = None
            if not decoded[key]:
                self.problems.append("%s: texture missing %s" % (web, tex))
                continue
            w, h, rgba = decoded[key]
            if mode in ("color", "render"):
                opaque = min(rgba[3::4]) == 255
                px, ch = (rgb_of(rgba), 3) if opaque else (rgba, 4)
            else:
                px, ch = white_mask(mask_alpha(rgba, bright=(mode == "mask"))), 2
            write_png(os.path.join(img_dir, *web[len("img/"):].split("/")), w, h, px, ch)
            out.append((web, w, h, ch, px))
        return out


def contact_sheet(written, path, cell=96, cols=16):
    """All written icons on one dark PNG (glyphs white, colour art as is), one block of rows per folder."""
    pad, inner = 4, cell - 8
    blocks = {}
    for item in written:
        blocks.setdefault(item[0].split("/")[1], []).append(item)
    rows = sum((len(v) + cols - 1) // cols for v in blocks.values())
    W, H = cols * cell, rows * cell + 6 * len(blocks)
    canvas = bytearray(b"\x16\x16\x16" * (W * H))
    y0 = 0
    for kind in KINDS:
        items = blocks.get(kind, [])
        if not items:
            continue
        canvas[y0 * W * 3:(y0 + 3) * W * 3] = b"\xff\x9c\x00" * (W * 3)  # orange separator line
        y0 += 6
        for n, (_, w, h, ch, px) in enumerate(items):
            cx, cy = (n % cols) * cell + pad, y0 + (n // cols) * cell + pad
            for y in range(cy, cy + inner):
                canvas[(y * W + cx) * 3:(y * W + cx + inner) * 3] = b"\x2a\x2a\x2a" * inner
            s = max(w, h) / inner
            dw, dh = max(1, int(w / s)), max(1, int(h / s))
            ox, oy = cx + (inner - dw) // 2, cy + (inner - dh) // 2
            for y in range(dh):
                sy = [min(h - 1, int((y + f) * s)) for f in (0.25, 0.75)]
                for x in range(dw):
                    sx = [min(w - 1, int((x + f) * s)) for f in (0.25, 0.75)]
                    acc = [0, 0, 0, 0]
                    for yy in sy:
                        for xx in sx:
                            i = (yy * w + xx) * ch
                            if ch == 2:
                                a = px[i + 1]
                                acc[0] += 255 * a; acc[1] += 255 * a; acc[2] += 255 * a; acc[3] += a
                            else:
                                a = px[i + 3] if ch == 4 else 255
                                acc[0] += px[i] * a; acc[1] += px[i + 1] * a; acc[2] += px[i + 2] * a; acc[3] += a
                    o = ((oy + y) * W + ox + x) * 3
                    alpha = acc[3] / (4 * 255)
                    for c in range(3):
                        fg = acc[c] / acc[3] if acc[3] else 0
                        canvas[o + c] = int(fg * alpha + canvas[o + c] * (1 - alpha))
        y0 += (len(items) + cols - 1) // cols * cell
    write_png(path, W, H, canvas, 3)
    return W, H
