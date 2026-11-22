"""Classes, their items per slot, upgrade tiers and overclocks, in the order the game (and the DB2 code) uses.

Item order  = the class inventory list (BP_<Class>Inventory.<Slot list>), 1-based.
Tier order  = UpgradeTiers of the item's upgradable gear component (= GetItemUpgradeTiers), options 1-based.
OC order    = OverclockBank.Overclocks TMap in serialized order (= Map_Keys at runtime), 1-based.
"""
from .assets import is_ref, object_name, struct_list

CLASSES = [  # letter, PlayerCharacterID, inventory list, class glyph, portrait
    ("D", "/Game/Character/DrillerID", "/Game/Character/InventoryLists/BP_DrillerInventory"),
    ("E", "/Game/Character/EngineerID", "/Game/Character/InventoryLists/BP_EngineerInventory"),
    ("G", "/Game/Character/GunnerID", "/Game/Character/InventoryLists/BP_GunnerInventory"),
    ("S", "/Game/Character/ScoutID", "/Game/Character/InventoryLists/BP_ScoutInventory"),
]
CLASS_GLYPH = "/Game/UI/Art/Icons/Icons_Server/ClassIcons_No_Outline/Class_Icon_Without_Outline_%s"
CLASS_PORTRAIT = "/Game/UI/Art/MainOnScreenHUD/ClassIcon/Characters/Icon_Character_%s"
SLOTS = [  # site key, inventory list property, display name
    ("primary", "PrimaryWeapons", "Primary Weapon"),
    ("secondary", "SecondaryWeapons", "Secondary Weapon"),
    ("traversal", "TraversalTools", "Traversal Tool"),
    ("tool", "ClassTools", "Support Tool"),
    ("armor", "Armors", "Armor"),
    ("grenade", "Grenades", "Throwable"),
    ("pickaxe", "MiningTools", "Pickaxe"),
]
FLARES = "Flares"  # shown on the loadout screen, not in the code (one flare per class, no upgrades)
OC_TYPES = {"SCAT_OC_Clean": "clean", "SCAT_OC_Balanced": "balanced", "SCAT_OC_Unstable": "unstable"}


class Gear:
    def __init__(self, store, texts, icons, stats=None):
        self.store, self.texts, self.icons, self.stats = store, texts, icons, stats
        self.problems = []
        self.categories = {}

    def text(self, ft, where):
        s = self.texts.get(ft, where)
        return s.strip() if s else s

    # ------------------------------------------------------------ gear component
    def component(self, class_path, depth=0):
        """Merged props of the item class's Upgradable*Component template (parent templates first)."""
        asset = self.store.get(class_path)
        if not asset or depth > 8:
            self.problems.append("item class package missing: " + class_path)
            return {}
        comp = next((e for e in asset.exports
                     if "Upgradable" in asset.ref(e["cls"]) and asset.ref(e["cls"]).endswith("Component")), None)
        if not comp:  # only inherited: look at the parent class
            for e in asset.exports:
                if asset.ref(e["cls"]).endswith("BlueprintGeneratedClass") and asset.ref(e["sup"]).startswith("/Game/"):
                    return self.component(asset.ref(e["sup"]), depth + 1)
            return {}
        chain = [(asset, comp)]
        while len(chain) < 12:
            a, e = chain[-1]
            tmpl = a.ref(e["tmpl"])
            if not tmpl.startswith("/Game/"):
                break
            ta = self.store.get(tmpl)
            found = ta and next((x for x in ta.exports if x["name"] == object_name(tmpl)
                                 and "Upgradable" in ta.ref(x["cls"])), None)
            if not found:
                self.problems.append("template missing: " + tmpl)
                break
            chain.append((ta, found))
        merged = {}
        for a, e in reversed(chain):
            merged.update(dict(a.export_props(e)))
        return merged

    def category(self, path):
        """Upgrade category (UPC_*) -> (name, glyph web path)."""
        if not is_ref(path):
            return None, None
        if path not in self.categories:
            pr = self.store.props(path)
            self.categories[path] = (self.text(pr.get("CategoryName"), object_name(path)),
                                     self.icons.glyph(pr.get("CategoryIcon")))
        return self.categories[path]

    # ------------------------------------------------------------ upgrades and overclocks
    def upgrade(self, path, index):
        pr = self.store.props(path)
        name = self.store.name(path)
        cat, icon = self.category(pr.get("Category"))
        if not icon:
            self.problems.append("%s: no category glyph" % name)
        return {"index": index, "id": name, "name": self.text(pr.get("Name"), name),
                "description": self.text(pr.get("Description"), name), "category": cat, "icon": icon}

    def overclock(self, path, index):
        pr = self.store.props(path)
        name = self.store.name(path)
        scat = object_name(pr.get("SchematicCategory") or "None")
        if scat not in OC_TYPES:
            self.problems.append("%s: unknown schematic category %s" % (name, scat))
        _, icon = self.category(pr.get("Category"))
        if not icon:
            self.problems.append("%s: no category glyph" % name)
        stats = [{"text": self.text(s.get("StatText"), name), "good": bool(s.get("IsAdventageous"))}
                 for s in struct_list(pr.get("StatTexts"))]
        return {"index": index, "id": name, "name": self.text(pr.get("Name"), name), "type": OC_TYPES.get(scat),
                "description": self.text(pr.get("Description"), name), "icon": icon, "stats": stats}

    # ------------------------------------------------------------ items and classes
    def item(self, id_path, index):
        idp = self.store.props(id_path)
        asset_name = self.store.name(id_path)
        item_id = asset_name[3:] if asset_name.startswith("ID_") else asset_name
        comp = self.component(idp["Item"]) if is_ref(idp.get("Item")) else {}
        if not comp:
            self.problems.append("%s: no upgradable gear component" % item_id)
        idat = self.store.props(comp.get("ItemData")) if is_ref(comp.get("ItemData")) else {}

        def pick(key):  # ItemData first, as the game's menus do, else the gear component
            return self.text(idat.get(key), item_id) or self.text(comp.get(key), item_id)

        tiers, tier_paths = [], []
        for t in struct_list(comp.get("UpgradeTiers")):
            ups = [u for u in t.get("Upgrades") or [] if is_ref(u)]
            tiers.append({"options": [self.upgrade(u, i + 1) for i, u in enumerate(ups)]})
            tier_paths.append(ups)
        ocs, oc_paths = [], []
        bank = comp.get("OverclockBank")
        if is_ref(bank):
            entries = (self.store.props(bank).get("Overclocks") or {}).get("map", [])
            ocs = [self.overclock(k, i + 1) for i, (k, _) in enumerate(entries)]
            oc_paths = [k for k, _ in entries]
        out = {"index": index, "id": item_id, "name": pick("Name"), "category": pick("Category"),
               "description": pick("Description"),
               "icon": self.icons.add("items", item_id, idat.get("IconBG") or comp.get("IconBG"), "mask"),
               "iconLine": self.icons.add("items", item_id + "_line", idat.get("IconLine") or comp.get("IconLine"),
                                          "mask"),
               "tiers": tiers, "overclocks": ocs}
        if self.stats and is_ref(idp.get("Item")):  # loadout stats panel (gamedata/stats.py)
            self.stats.attach(out, idp["Item"], tier_paths, oc_paths)
        return out

    def classes(self):
        out = []
        for letter, id_path, inv_path in CLASSES:
            cid = self.store.props(id_path)
            name = cid.get("AssetName") or object_name(id_path)[:-2]
            inv = self.store.props(inv_path)
            if not inv:
                self.problems.append("inventory list missing: " + inv_path)
            ids = {key: [p for p in inv.get(prop) or [] if is_ref(p)] for key, prop, _ in SLOTS}
            flares = [p for p in inv.get(FLARES) or [] if is_ref(p)]
            self.store.prefetch([p for v in ids.values() for p in v] + flares)
            self.store.prefetch([self.store.props(p).get("Item") for v in ids.values() for p in v] +
                                [self.store.props(p).get("Item") for p in flares])
            key = name.lower()
            out.append({
                "letter": letter, "key": key, "name": name,
                "icon": self.icons.add("class", key, CLASS_GLYPH % name, "mask"),
                "portrait": self.icons.add("class", key + "_portrait", CLASS_PORTRAIT % name, "color"),
                "slots": {k: [self.item(p, i + 1) for i, p in enumerate(ids[k])] for k, _, _ in SLOTS},
                "flare": self.item(flares[0], 1) if flares else None})
            if not flares:
                self.problems.append("%s: no flare in the inventory list" % name)
            for p, it in zip(ids["grenade"], out[-1]["slots"]["grenade"]):
                it["iconDetail"] = self.detail_icon(p, it["id"])
        return out

    def detail_icon(self, id_path, item_id):
        """IconDetailed of an item (the glyph of the game's GRENADE SELECTION hexagons), None if none."""
        idp = self.store.props(id_path)
        comp = self.component(idp["Item"]) if is_ref(idp.get("Item")) else {}
        idat = self.store.props(comp.get("ItemData")) if is_ref(comp.get("ItemData")) else {}
        tex = idat.get("IconDetailed") or comp.get("IconDetailed")
        return self.icons.add("items", item_id + "_detail", tex, "mask") if is_ref(tex) else None
