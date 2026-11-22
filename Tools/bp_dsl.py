"""Small helpers to write BPForge specs (Blueprint graphs as JSON) from Python.

A graph is a list of nodes, links between pins ("node.pin") and pin defaults. Execution order is written
with flow(); call() takes pin values: ref("node.pin") makes a link, anything else is a pin default.
Used by Tools/dwarfbuilds_spec.py. (Tools/umbra_spec.py keeps its own copy.)
"""

KSL = "/Script/Engine.KismetSystemLibrary"
KML = "/Script/Engine.KismetMathLibrary"
KSTR = "/Script/Engine.KismetStringLibrary"
KTXT = "/Script/Engine.KismetTextLibrary"
KARR = "/Script/Engine.KismetArrayLibrary"
KMAP = "/Script/Engine.BlueprintMapLibrary"
GS = "/Script/Engine.GameplayStatics"
ACTOR = "/Script/Engine.Actor"
UMG = "/Script/UMG."
WBL = UMG + "WidgetBlueprintLibrary"


def t(name, typ, cls=None, **extra):
    """Pin or variable type: float, int, bool, string, text, name, or object/class/struct/interface with cls."""
    out = {"name": name, "type": typ}
    if cls:
        out["class"] = cls
    out.update(extra)
    return out


def ref(pin):
    """Marks a call argument as a link to another node's pin."""
    return "@" + pin


class Graph:
    def __init__(self):
        self.nodes, self.links, self.defaults = [], [], {}
        self._n = 0

    def uid(self, prefix="n"):
        self._n += 1
        return "%s%d" % (prefix, self._n)

    def node(self, node_id, kind, **fields):
        self.nodes.append(dict(id=node_id, k=kind, **fields))
        return node_id

    def pins(self, node_id, pins):
        for pin, value in pins.items():
            pin = "self" if pin == "target" else pin  # the object the function is called on
            if isinstance(value, str) and value.startswith("@"):
                self.link(value[1:], "%s.%s" % (node_id, pin))
            elif value is not None:
                self.defaults["%s.%s" % (node_id, pin)] = value
        return node_id

    def call(self, node_id, fn, **pins):
        self.node(node_id, "call", fn=fn)
        return self.pins(node_id, pins)

    def get(self, node_id, var, cls=None, target=None):
        self.node(node_id, "get", var=var, **({"class": cls} if cls else {}))
        if target:
            self.link(target, node_id + ".self")
        return node_id + "." + var

    def set(self, node_id, var, cls=None, target=None, value=None, default=None):
        self.node(node_id, "set", var=var, **({"class": cls} if cls else {}))
        if target:
            self.link(target, node_id + ".self")
        if value:
            self.link(value, node_id + "." + var)
        if default is not None:
            self.defaults["%s.%s" % (node_id, var)] = default
        return node_id

    def cast(self, node_id, cls, obj):
        self.node(node_id, "cast", **{"class": cls})
        self.link(obj, node_id + ".Object")
        return node_id + ".$cast"

    def loop(self, node_id, array):
        self.node(node_id, "macro", name="ForEachLoop")
        self.link(array, node_id + ".Array")
        return node_id + ".Array Element"

    def for_range(self, node_id, first, last):
        """ForLoop macro from first to last (inclusive); first/last are pin refs or literal strings."""
        self.node(node_id, "macro", name="ForLoop")
        self.pins(node_id, {"FirstIndex": first, "LastIndex": last})
        return node_id + ".Index"

    def branch(self, node_id, condition):
        self.node(node_id, "branch")
        self.link(condition, node_id + ".Condition")
        return node_id

    def self_(self, node_id="self"):
        if not any(n["id"] == node_id for n in self.nodes):
            self.node(node_id, "self")
        return node_id + ".self"

    def link(self, a, b):
        self.links.append([a, b])

    def flow(self, *steps):
        """Chain execution: each step is a node id, or 'node.pin' for a non-default pin."""
        for a, b in zip(steps, steps[1:]):
            out_pin = a if "." in a else a + ".then"
            in_pin = b if "." in b else b + ".execute"
            self.link(out_pin, in_pin)

    def data(self):
        return {"nodes": self.nodes, "links": self.links, "defaults": self.defaults}
