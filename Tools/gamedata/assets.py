"""Cooked UE 4.27 packages with FText, TMap and GUID decoding, and a cached store over the extracted pak.

Builds on Tools/uasset_dump.py (package header and tagged properties).
Object references come back as strings: '/Game/Dir/Asset.Asset', '/Script/FSD.Class', 'export:Name' or 'None'.
"""
import os
import sys

from .pak import TOOLS, package_filter

if TOOLS not in sys.path:
    sys.path.insert(0, TOOLS)
from uasset_dump import Package, Reader  # noqa: E402


BINARY_STRUCTS = {"PointerToUberGraphFrame", "GameplayTagContainer", "PerPlatformFloat", "PerPlatformInt",
                  "FrameNumber"}  # native binary layout: skipped (value "<Name>")


def read_ftext(r):
    """Cooked FText -> dict. kind: base (ns, key, src), table (table, key), none (str), or historyN."""
    r.u32()  # flags
    kind = r.fmt("<b")[0]
    if kind == -1:
        has = r.i32()
        return {"kind": "none", "str": r.fstring() if has else ""}
    if kind == 0:
        ns, key, src = r.fstring(), r.fstring(), r.fstring()
        return {"kind": "base", "ns": ns, "key": key, "src": src}
    if kind == 11:
        table, key = r.fname(), r.fstring()
        return {"kind": "table", "table": table, "key": key}
    return {"kind": "history%d" % kind}


class Asset(Package):
    """Package whose property values decode FText, TMap and FGuid."""

    def read_value(self, r, typ, size, tag, depth):
        if typ == "TextProperty":
            end = r.p + size
            try:
                return read_ftext(r)
            finally:
                r.p = end
        if typ == "ArrayProperty" and tag.get("inner") == "TextProperty":
            end = r.p + size
            try:
                return [read_ftext(r) for _ in range(r.i32())]
            finally:
                r.p = end
        if typ == "MapProperty":
            end = r.p + size
            try:
                r.i32()  # keys to remove
                return {"map": [(self.simple(r, tag["inner"]), self.simple(r, tag["value"])) for _ in range(r.i32())]}
            except (IndexError, ValueError, UnicodeDecodeError):  # native struct keys or values: not decoded
                return {"map": None}
            finally:
                r.p = end
        return Package.read_value(self, r, typ, size, tag, depth)

    def read_struct(self, r, sname, end, depth):
        if sname == "Guid":
            return "%08X%08X%08X%08X" % r.fmt("<4I")
        if sname in BINARY_STRUCTS and end is not None:
            r.p = end
            return "<%s>" % sname
        return Package.read_struct(self, r, sname, end, depth)

    def simple(self, r, typ):
        if typ in ("ObjectProperty", "InterfaceProperty"):
            return self.ref(r.i32())
        if typ == "FloatProperty":
            return round(r.f32(), 6)
        if typ == "IntProperty":
            return r.i32()
        if typ in ("NameProperty", "EnumProperty"):
            return r.fname()
        if typ == "StrProperty":
            return r.fstring()
        if typ == "BoolProperty":
            return bool(r.u8())
        if typ == "TextProperty":
            return read_ftext(r)
        if typ == "StructProperty":
            return dict(self.read_props(r, 1))
        raise ValueError("unsupported map type " + typ)

    def export(self, name=None):
        """Export entry by name, ignoring case like FNames do (default: the export named like the package,
        else the first)."""
        want = (name or self.short_name).lower()
        for e in self.exports:
            if e["name"].lower() == want:
                return e
        return None if name else self.exports[0]

    def props(self, name=None):
        e = self.export(name)
        return dict(self.export_props(e)) if e else {}

    def export_class(self, e):
        return self.ref(e["cls"])


def package_of(path):
    """'/Game/A/B.B_C' -> '/Game/A/B'."""
    return path.split(".")[0]


def object_name(path):
    """'/Game/A/B.B_C' -> 'B_C'; '/Game/A/B' -> 'B'."""
    return path.split(".")[-1].split("/")[-1]


def is_ref(v):
    return isinstance(v, str) and v.startswith("/Game/")


class Store:
    """Cached package loader over <root>/FSD/Content. With an extractor, missing packages are extracted on demand."""

    def __init__(self, root, extractor=None):
        self.root, self.extractor = root, extractor
        self.cache, self.tried, self.missing, self._dirs = {}, set(), set(), {}

    def file(self, path):
        pkg = package_of(path)
        if not pkg.startswith("/Game/"):
            raise ValueError("not a /Game path: " + path)
        return os.path.join(self.root, "FSD", "Content", *pkg[len("/Game/"):].split("/")) + ".uasset"

    def prefetch(self, paths):
        """Extract the given packages that are not on disk yet, in parallel."""
        if not self.extractor:
            return
        need = sorted({package_of(p) for p in paths if is_ref(p)} - self.tried)
        need = [p for p in need if not os.path.exists(self.file(p))]
        self.tried.update(need)
        self.extractor.extract(package_filter(p) for p in need)

    def get(self, path):
        """Asset for a /Game path, or None when it is not in the pak."""
        if not is_ref(path):
            return None
        pkg = package_of(path)
        if pkg in self.cache:
            return self.cache[pkg]
        f = self.file(pkg)
        if not os.path.exists(f):
            self.prefetch([pkg])
        asset = None
        if os.path.exists(f):
            asset = Asset(f)
            asset.short_name = object_name(pkg)
            asset.game_path = pkg
        else:
            self.missing.add(pkg)
        self.cache[pkg] = asset
        return asset

    def name(self, path):
        """Asset name as spelled by its file in the pak. FNames ignore case, so references and even the
        package itself may spell it differently (UPG_FlameThrower_E_HeatAoE vs UPG_Flamethrower_E_HeatAoE)."""
        f = self.file(path)
        d, base = os.path.dirname(f), os.path.basename(f).lower()
        if base not in self._dirs.get(d, {}):  # (re)list: files may have been extracted since
            self._dirs[d] = {x.lower(): x for x in os.listdir(d)} if os.path.isdir(d) else {}
        real = self._dirs[d].get(base)
        return real[:-len(".uasset")] if real else object_name(path)

    def props(self, path, export=None):
        """Properties of an object path's export (the name after the dot), {} when missing."""
        asset = self.get(path)
        if not asset:
            return {}
        name = export or (path.split(".")[1] if "." in path else None)
        return asset.props(name)

    def native_parents(self, class_path, limit=12):
        """Class chain of a Blueprint class path up to the first /Script class: ['BP_X_C', ..., 'NativeClass']."""
        chain = []
        while class_path and limit:
            limit -= 1
            chain.append(object_name(class_path))
            if not class_path.startswith("/Game/"):
                break
            asset = self.get(class_path)
            e = asset and asset.export(object_name(class_path))
            if not e:
                break
            class_path = asset.ref(e["sup"])
        return chain


def struct_list(value):
    """Array of structs (list of (key, value) lists) -> list of dicts."""
    return [dict(x) for x in value or []]


__all__ = ["Asset", "Store", "Reader", "read_ftext", "package_of", "object_name", "is_ref", "struct_list"]
