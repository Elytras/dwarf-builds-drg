"""Cooked UE 4.27 Blueprint bytecode -> statement list of nested tuples (read-only).

Each statement is (memory offset, node). Jump targets in the bytecode are in-memory offsets: an object
reference is 8 bytes in memory but 4 on disk, an FName 12 vs 8, a property pointer 8 vs 8 + 8 * names.
Nodes: ("local", name), ("inst", name), ("let", target, value), ("call", kind, func, [args]),
("ctx", object, node), ("member", struct node, name), ("const", value), ("text", text dict),
("obj", path), ("jump", target), ("jumpifnot", target, cond), ("push", target), ("pop",),
("popifnot", cond), ("switch", index, [(key, value)], default), ("array", [items]), ("struct", name, [items]),
("cast", class, node), ("return", node), ("nop",), ("end",), ("self",).
"""
import struct

from uasset_dump import Reader
from uasset_funcs import read_field

CALL_KINDS = {0x1B: "virtual", 0x45: "localvirtual", 0x1C: "final", 0x46: "localfinal", 0x68: "math"}


class Parser:
    def __init__(self, pkg, data):
        self.pkg, self.r, self.extra = pkg, Reader(data, pkg.names), 0

    def mem(self):
        return self.r.p + self.extra

    def obj(self):
        self.extra += 4
        idx = self.r.i32()
        if idx > 0 and getattr(self.pkg, "game_path", None):  # export of this package: full object path
            names, e = [], self.pkg.exports[idx - 1]
            while e:
                names.append(e["name"])
                e = self.pkg.exports[e["outer"] - 1] if e["outer"] > 0 else None
            return self.pkg.game_path + "." + ".".join(reversed(names))
        return self.pkg.ref(idx)

    def name(self):
        self.extra += 4
        return self.r.fname()

    def prop(self):
        n = self.r.i32()
        names = [self.r.fname() for _ in range(n)]
        self.r.i32()
        self.extra -= 8 * n
        return names[0] if names else None

    def cstr(self, wide=False):
        out = []
        while True:
            c = self.r.fmt("<H")[0] if wide else self.r.u8()
            if c == 0:
                return "".join(out)
            out.append(chr(c))

    def params(self):
        out = []
        while self.r.d[self.r.p] != 0x16:
            out.append(self.expr())
        self.r.p += 1
        return out

    def until(self, stop):
        out = []
        while self.r.d[self.r.p] != stop:
            out.append(self.expr())
        self.r.p += 1
        return out

    def text(self):
        kind = self.r.u8()
        if kind == 0:
            return {"kind": "none", "str": ""}
        if kind == 1:
            src, key, ns = self.expr(), self.expr(), self.expr()
            return {"kind": "base", "ns": ns[1], "key": key[1], "src": src[1]}
        if kind in (2, 3):
            return {"kind": "none", "str": self.expr()[1]}
        if kind == 4:
            table = self.obj()
            self.expr()
            key = self.expr()
            return {"kind": "table", "table": table, "key": key[1]}
        raise ValueError("text kind %d" % kind)

    def expr(self):
        r = self.r
        op = r.u8()
        if op in (0x00, 0x48):
            return ("local", self.prop())
        if op in (0x01, 0x6C):
            return ("inst", self.prop())
        if op == 0x02:
            return ("default", self.prop())
        if op == 0x04:
            return ("return", self.expr())
        if op == 0x06:
            return ("jump", r.u32())
        if op == 0x07:
            t = r.u32()
            return ("jumpifnot", t, self.expr())
        if op == 0x09:
            r.fmt("<HB")
            return ("nop", self.expr())
        if op in (0x0B, 0x50, 0x5A, 0x5E):
            return ("nop",)
        if op == 0x0F:
            self.prop()
            a = self.expr()
            return ("let", a, self.expr())
        if op in (0x14, 0x5F, 0x60, 0x43, 0x44):
            a = self.expr()
            return ("let", a, self.expr())
        if op in (0x12, 0x19, 0x1A):
            target = self.expr()
            r.u32()
            self.prop()
            return ("ctx", target, self.expr())
        if op in (0x13, 0x2E, 0x52, 0x54, 0x55):
            cls = self.obj()
            return ("cast", cls, self.expr())
        if op == 0x15:
            return ("endparm",)
        if op == 0x17:
            return ("self",)
        if op == 0x18:
            r.u32()
            return self.expr()
        if op in (0x1B, 0x45):
            name = self.name()
            return ("call", CALL_KINDS[op], name, self.params())
        if op in (0x1C, 0x46, 0x68, 0x63):
            func = self.obj()
            return ("call", CALL_KINDS.get(op, "final"), func, self.params())
        if op == 0x1D:
            return ("const", r.i32())
        if op == 0x1E:
            return ("const", r.f32())
        if op == 0x1F:
            return ("const", self.cstr())
        if op == 0x34:
            return ("const", self.cstr(True))
        if op == 0x20:
            return ("obj", self.obj())
        if op == 0x21:
            return ("const", self.name())
        if op in (0x22, 0x23):
            return ("const", r.fmt("<3f"))
        if op == 0x2B:
            return ("const", r.fmt("<10f"))
        if op in (0x24, 0x2C):
            return ("const", r.u8())
        if op == 0x25:
            return ("const", 0)
        if op == 0x26:
            return ("const", 1)
        if op == 0x27:
            return ("const", True)
        if op == 0x28:
            return ("const", False)
        if op in (0x2A, 0x2D):
            return ("const", None)
        if op == 0x29:
            return ("text", self.text())
        if op == 0x2F:
            name = self.obj()
            r.i32()
            return ("struct", name, self.until(0x30))
        if op == 0x31:
            target = self.expr()
            return ("setarray", target, self.until(0x32))
        if op in (0x39, 0x3B):  # EX_SetSet / EX_SetMap: target, element count, elements (map: key, value...)
            target = self.expr()
            r.i32()
            return ("setset" if op == 0x39 else "setmap", target, self.until(0x3A if op == 0x39 else 0x3C))
        if op in (0x33, 0x6D):
            return ("const", self.prop())
        if op == 0x35:
            return ("const", r.i64())
        if op == 0x36:
            return ("const", r.fmt("<Q")[0])
        if op == 0x38:
            r.u8()
            return ("primcast", self.expr())
        if op == 0x42:
            member = self.prop()
            return ("member", self.expr(), member)
        if op == 0x4B:
            return ("const", self.name())
        if op == 0x4C:
            return ("push", r.u32())
        if op == 0x4D:
            return ("pop",)
        if op == 0x4E:
            return ("computedjump", self.expr())
        if op == 0x4F:
            return ("popifnot", self.expr())
        if op == 0x51:
            return self.expr()
        if op == 0x53:
            return ("end",)
        if op == 0x5B:
            return ("const", r.u32())
        if op in (0x5C, 0x62):
            a = self.expr()
            return ("nop", a, self.expr())
        if op == 0x5D:
            return ("nop", self.expr())
        if op == 0x61:
            self.name()
            a = self.expr()
            return ("nop", a, self.expr())
        if op == 0x64:
            name = self.prop()
            return ("let", ("inst", name), self.expr())
        if op == 0x65:
            self.prop()
            r.i32()
            return ("array", self.until(0x66))
        if op == 0x67:
            return ("soft", self.expr())
        if op == 0x69:
            cases = r.fmt("<H")[0]
            r.u32()
            index = self.expr()
            parts = []
            for _ in range(cases):
                key = self.expr()
                r.u32()
                parts.append((key, self.expr()))
            return ("switch", index, parts, self.expr())
        if op == 0x6A:
            r.u8()
            return ("nop",)
        if op == 0x6B:
            a = self.expr()
            return ("index", a, self.expr())
        raise ValueError("opcode 0x%02X at %d" % (op, r.p - 1))


class Function:
    """A Blueprint function export: parameters [(name, type, extra, flags)] and statements."""

    def __init__(self, pkg, export):
        self.pkg, self.name = pkg, export["name"]
        r = Reader(pkg.body, pkg.names)
        r.p = export["off"] - pkg.body_base
        pkg.read_props(r)
        r.i32()
        self.super = pkg.ref(r.i32())
        children = r.i32()
        r.p += 4 * children
        self.props = [read_field(pkg, r) for _ in range(r.i32())]
        self.params = [p for p in self.props if p and "parm" in p[3]]
        r.i32()  # bytecode size in memory
        size = r.i32()
        parser = Parser(pkg, pkg.body[r.p:r.p + size])
        self.stmts, self.at = [], {}
        while parser.r.p < size:
            mem = parser.mem()
            node = parser.expr()
            self.at[mem] = len(self.stmts)
            self.stmts.append((mem, node))
        self.flags = struct.unpack_from("<I", pkg.body, r.p + size)[0] if r.p + size + 4 <= len(pkg.body) else 0


def functions(pkg):
    """{name: Function} of every function export in a package."""
    out = {}
    for e in pkg.exports:
        if pkg.ref(e["cls"]).endswith(".Function"):
            out[e["name"]] = e
    return out
