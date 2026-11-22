"""English display text: Game.locres, string tables, FText -> string.

Base FText: locres [namespace] key, else the inline source string.
String-table FText: locres [table namespace] key, else the table's source string.
"""
import struct

from .assets import Reader, object_name

LOCRES_MAGIC = bytes.fromhex("0E147475674A03FC4A15909DC3377F1B")
LOCRES_PATH = "/Game/Localization/Game/en/Game.locres"


def _fstr(d, p):
    n = struct.unpack_from("<i", d, p)[0]
    p += 4
    if n == 0:
        return "", p
    if n < 0:
        return d[p:p - 2 * n].decode("utf-16-le").rstrip("\0"), p - 2 * n
    return d[p:p + n].decode("latin-1").rstrip("\0"), p + n


def read_locres(path):
    """Locres (versions 0..3) -> {(namespace, key): text}."""
    with open(path, "rb") as f:
        d = f.read()
    p, ver = 0, 0
    if d[:16] == LOCRES_MAGIC:
        ver, p = d[16], 17
    strings = []
    if ver >= 1:
        q = struct.unpack_from("<q", d, p)[0]
        p += 8
        n = struct.unpack_from("<i", d, q)[0]
        q += 4
        for _ in range(n):
            s, q = _fstr(d, q)
            q += 4 if ver >= 3 else 0  # ref count
            strings.append(s)
    if ver >= 2:
        p += 4  # entries count
    out = {}
    nns = struct.unpack_from("<I", d, p)[0]
    p += 4
    for _ in range(nns):
        p += 4 if ver >= 2 else 0
        ns, p = _fstr(d, p)
        nk = struct.unpack_from("<I", d, p)[0]
        p += 4
        for _ in range(nk):
            p += 4 if ver >= 2 else 0
            key, p = _fstr(d, p)
            p += 4  # source hash
            if ver >= 1:
                out[(ns, key)] = strings[struct.unpack_from("<i", d, p)[0]]
                p += 4
            else:
                out[(ns, key)], p = _fstr(d, p)
    return out


def read_string_table(asset):
    """StringTable asset -> (namespace, {key: source})."""
    e = [x for x in asset.exports if asset.ref(x["cls"]).endswith("StringTable")][0]
    r = Reader(asset.body, asset.names)
    r.p = e["off"] - asset.body_base
    asset.read_props(r)
    if r.i32():
        r.p += 16
    ns = r.fstring()
    entries = {}
    for _ in range(r.i32()):
        k = r.fstring()
        entries[k] = r.fstring()
    return ns, entries


class Texts:
    """Resolves FText dicts from assets.Asset to English strings; collects what failed."""

    def __init__(self, store, locres_file):
        self.store = store
        self.locres = read_locres(locres_file)
        self.tables = {}
        self.problems = []

    def table(self, path):
        if path not in self.tables:
            asset = self.store.get(path)
            self.tables[path] = read_string_table(asset) if asset else ("", {})
        return self.tables[path]

    def get(self, t, where=""):
        """String for an FText dict, None for no text. Newlines become \\n, line ends lose trailing spaces."""
        if not isinstance(t, dict):
            return None
        kind = t.get("kind")
        s = None
        if kind == "base":
            s = self.locres.get((t["ns"], t["key"]), t["src"])
        elif kind == "none":
            s = t["str"]
        elif kind == "table":
            ns, entries = self.table(t["table"])
            s = self.locres.get((ns, t["key"]), entries.get(t["key"]))
            if s is None:
                self.problems.append("%s: string table %s has no key %s" % (where, object_name(t["table"]), t["key"]))
        else:
            self.problems.append("%s: unsupported FText %s" % (where, kind))
        if s is None:
            return None
        return "\n".join(line.rstrip() for line in s.replace("\r\n", "\n").replace("\r", "\n").split("\n"))
