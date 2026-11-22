"""Build the Dwarf Builds website data from the installed game (read-only).

Usage: python Tools/dwarfbuilds_export.py [--skip-extract] [--game <DRG install folder>]

1. Extracts the needed files from FSD-WindowsNoEditor.pak with UnrealPak into Saved/GameData/pak
   (cleared first; --skip-extract reuses what is there).
2. Writes Web/public/data/game.json (format: Docs/dwarfbuilds-site.md section 3) and the PNGs it references
   into Web/public/img/{class,items,render,upg,perks,frames}.
3. Checks the result and writes a contact sheet of all icons to Saved/GameData/check/contact.png.
New equippable perks are appended to Tools/dwarfbuilds/perk_ids.json (never reordered).
Exit code 1 when a check fails.
"""
import json
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gamedata import pak  # noqa: E402
from gamedata.assets import Store  # noqa: E402
from gamedata.gear import SLOTS, Gear  # noqa: E402
from gamedata.icons import Icons, contact_sheet  # noqa: E402
from gamedata.perks import Perks  # noqa: E402
from gamedata.stats import Stats, check_stats  # noqa: E402
from gamedata.text import Texts  # noqa: E402

ROOT = pak.ROOT
PAK_DIR = os.path.join(ROOT, "Saved", "GameData", "pak")
CHECK_DIR = os.path.join(ROOT, "Saved", "GameData", "check")
WEB = os.path.join(ROOT, "Web", "public")
PERK_IDS = os.path.join(ROOT, "Tools", "dwarfbuilds", "perk_ids.json")
GAME_DIR = pak.DEFAULT_GAME

BULK = [  # small folders taken whole; everything else is extracted per package when first needed
    "FSD/Content/Character/InventoryLists/*",
    "FSD/Content/WeaponsNTools/*/ID_*",
    "FSD/Content/WeaponsNTools/*/IDAT_*",
    "FSD/Content/WeaponsNTools/*/UPG_*",
    "FSD/Content/WeaponsNTools/*/OC_*",
    "FSD/Content/WeaponsNTools/*/OSB_*",
    "FSD/Content/WeaponsNTools/_UpgradeCategories/*",
    "FSD/Content/Game/Text/*",
    "FSD/Content/Localization/Game/en/*",
    "FSD/Content/Game/GameData/GD_KpiSettings.*",
    "FSD/Content/GameElements/KPI/Perks/*",
    "FSD/Content/GameElements/KPI/Prophecies/*",
    "FSD/Content/GameElements/Schematics/Categories/*",
]
SCAT = "/Game/GameElements/Schematics/Categories/SCAT_OC_%s"
FRAMES = [  # key, file name, texture, mode
    ("upgBg", "upg_bg", "/Game/UI/Menu_Loadout/Assets/Upgrade_BG", "fill"),
    ("upgOutline", "upg_outline", "/Game/UI/Menu_Loadout/Assets/Upgrade_Outline", "fill"),
    ("profFrame", "prof_frame", "/Game/Art/Textures/Icon_PerkProf_Frame", "fill"),
    ("profFrameOutline", "prof_frame_outline", "/Game/Art/Textures/Icon_PerkProf_Frame_02", "fill"),
    ("perkActiveFill", "perk_active_fill", "/Game/UI/Art/MainOnScreenHUD/SelectionBox_Corner_Large2px_Filled", "fill"),
    ("perkActiveOutline", "perk_active_outline",
     "/Game/UI/Art/MainOnScreenHUD/SelectionBox_Corner_Large2px_Outline", "fill"),
    ("statArrow", "stat_arrow", "/Game/Art/UI/Crosshairs/New_Crosshairs/Crosshair_Basic_Triangle_Filled", "fill"),
]
# UI/Art/WidgetParts/LIB_MenuColors and LIB_PerkColors (linear colours converted to sRGB), rich text styles.
COLORS = {
    "passive": "#B92E4E", "active": "#4BBFA5", "proficiency": "#FF9C00",
    "orange": "#FF9C00", "orangeLight": "#FFD200", "orangeDark": "#5B412D", "orangeVeryDark": "#31261F",
    "hud": "#B8B483", "hudLight": "#F7F5D8", "hudDark": "#26271E", "white": "#FFFFFF", "black": "#000000",
    "windowBg": "#000000B3", "windowBgSelected": "#593F2CCC", "greyMedium": "#ADADAD", "greyDark": "#343434",
    "statGood": "#54E200", "statBad": "#EC0000", "warning": "#EC0000", "approval": "#54E200",
    "textH": "#FFF390", "textHi": "#FFFFFF", "textHighlight": "#FFBA00", "ocBackground": "#099CFE",
}


def srgb_hex(linear):
    def c(x):
        x = max(0.0, min(1.0, x))
        x = 12.92 * x if x <= 0.0031308 else 1.055 * x ** (1 / 2.4) - 0.055
        return int(x * 255 + 0.5)
    return "#%02X%02X%02X" % tuple(c(v) for v in linear[:3])


def build(store, texts, icons):
    stats = Stats(store, texts)
    gear = Gear(store, texts, icons, stats)
    classes = gear.classes()
    perks = Perks(store, texts, icons)
    perk_list, max_tier = perks.build(PERK_IDS)
    frames, colors = {}, {}
    for t in ("Clean", "Balanced", "Unstable"):
        pr = store.props(SCAT % t)
        key = t.lower()
        frames["oc" + t] = icons.add("frames", "oc_" + key, pr.get("FrameTexture"), "mask")
        frames["oc" + t + "Fill"] = icons.add("frames", "oc_%s_fill" % key, pr.get("FrameTexture"), "fill")
        colors[key] = srgb_hex(pr["FrameTint"]) if pr.get("FrameTint") else None
    for key, name, tex, mode in FRAMES:
        frames[key] = icons.add("frames", name, tex, mode)
    colors.update(COLORS)
    data = {
        "version": pak.steam_version(GAME_DIR),
        "slots": [s for s, _, _ in SLOTS],
        "slotNames": {s: n for s, _, n in SLOTS},
        "classes": classes,
        "perks": perk_list,
        "perkSlots": {"passive": 3, "active": 2, "proficiency": 1},
        "frames": frames,
        "colors": colors,
    }
    n_items = sum(len(v) for c in classes for v in c["slots"].values()) + sum(1 for c in classes if c.get("flare"))
    return data, gear.problems + perks.problems, perks.warnings + stats.warnings(n_items), max_tier


def check(data, img_dir):
    """Counts and consistency checks. Returns (report lines, errors)."""
    errors, lines = [], []
    items = [(c, s, it) for c in data["classes"] for s in data["slots"] for it in c["slots"][s]]
    items += [(c, "flare", c["flare"]) for c in data["classes"] if c.get("flare")]
    ups = [o for _, _, it in items for t in it["tiers"] for o in t["options"]]
    ocs = [o for _, _, it in items for o in it["overclocks"]]
    per_class = {c["letter"]: sum(len(v) for v in c["slots"].values()) for c in data["classes"]}
    types = {}
    for p in data["perks"]:
        types[p["type"]] = types.get(p["type"], 0) + 1
    lines.append("classes %d, items per class %s, upgrade options %d, overclocks %d, perks %d %s" % (
        len(data["classes"]), per_class, len(ups), len(ocs), len(data["perks"]), types))
    oc_types = {}
    for o in ocs:
        oc_types[o["type"]] = oc_types.get(o["type"], 0) + 1
    lines.append("overclock types %s" % oc_types)

    def need(obj, keys, where):
        for k in keys:
            if not obj.get(k):
                errors.append("%s: empty %s" % (where, k))
    for c in data["classes"]:
        need(c, ("name", "icon", "portrait"), c["key"])
    for c, s, it in items:
        need(it, ("name", "category", "description", "icon", "iconLine"), "%s %s %s" % (c["letter"], s, it["id"]))
        if s not in ("grenade", "flare") and not it["tiers"]:
            errors.append("%s: no upgrade tiers" % it["id"])
        for t in it["tiers"]:
            if not 1 <= len(t["options"]) <= 3:
                errors.append("%s: tier with %d options (code digits are 1..3)" % (it["id"], len(t["options"])))
    for o in ups:
        need(o, ("name", "description", "category", "icon"), o["id"])
    for o in ocs:
        need(o, ("name", "description", "type", "icon", "stats"), o["id"])
        for st in o["stats"]:
            need(st, ("text",), o["id"] + " stat")
    for p in data["perks"]:
        need(p, ("name", "type", "icon", "description"), p["asset"])
        if "{" in p["description"] or "}" in p["description"]:
            errors.append("%s: placeholder left in description" % p["asset"])
    paths = set()

    def walk(v):
        if isinstance(v, dict):
            for x in v.values():
                walk(x)
        elif isinstance(v, list):
            for x in v:
                walk(x)
        elif isinstance(v, str) and v.startswith("img/"):
            paths.add(v)
    walk(data)
    for p in sorted(paths):
        if not os.path.isfile(os.path.join(WEB, *p.split("/"))):
            errors.append("missing image " + p)
    on_disk = set()
    for d, _, files in os.walk(img_dir):
        for f in files:
            on_disk.add(os.path.relpath(os.path.join(d, f), WEB).replace(os.sep, "/"))
    extra = sorted(on_disk - paths)
    if extra:
        errors.append("images not referenced by game.json: %s" % extra[:10])
    size = sum(os.path.getsize(os.path.join(WEB, *p.split("/"))) for p in paths if
               os.path.isfile(os.path.join(WEB, *p.split("/"))))
    lines.append("images %d, total %.1f KB" % (len(paths), size / 1024))
    st_lines, st_errors = check_stats([("%s %s %s" % (c["letter"], s, it["id"]), it) for c, s, it in items])
    lines += st_lines
    errors += st_errors
    return lines, errors


def main():
    global GAME_DIR
    args = sys.argv[1:]
    GAME_DIR = args[args.index("--game") + 1] if "--game" in args else pak.DEFAULT_GAME
    skip = "--skip-extract" in args
    t0 = time.time()
    extractor = None
    if not skip:
        pak_file = pak.game_pak(GAME_DIR)
        if not os.path.isfile(pak_file):
            sys.exit("game pak not found: " + pak_file)
        extractor = pak.Extractor(pak_file, PAK_DIR)
        extractor.clear()
        n = extractor.extract(BULK)
        print("extracted %d files (bulk) in %.0f s" % (n, time.time() - t0))
    elif not os.path.isdir(PAK_DIR):
        sys.exit("nothing extracted yet in " + PAK_DIR + "; run without --skip-extract")
    store = Store(PAK_DIR, extractor)
    locres = os.path.join(PAK_DIR, "FSD", "Content", "Localization", "Game", "en", "Game.locres")
    texts = Texts(store, locres)
    icons = Icons(store)
    data, problems, warnings, max_tier = build(store, texts, icons)

    img_dir = os.path.join(WEB, "img")
    written = icons.write(img_dir)

    os.makedirs(os.path.join(WEB, "data"), exist_ok=True)
    out = os.path.join(WEB, "data", "game.json")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        json.dump(data, f, ensure_ascii=False, separators=(",", ":"))
    if extractor:
        print("UnrealPak calls: %d" % extractor.calls)
    lines, errors = check(data, img_dir)
    errors = problems + texts.problems + icons.problems + errors
    if store.missing:
        errors.append("packages not in the pak: %s" % sorted(store.missing))
    sheet = contact_sheet(written, os.path.join(CHECK_DIR, "contact.png"))

    print("version: %s" % data["version"])
    for line in lines:
        print(line)
    print("proficiency ranks (perk tiers): %d" % max_tier)
    print("game.json %.1f KB" % (os.path.getsize(out) / 1024))
    print("contact sheet %dx%d: %s" % (sheet[0], sheet[1], os.path.join(CHECK_DIR, "contact.png")))
    for w in warnings:
        print("warning:", w)
    for e in errors:
        print("ERROR:", e)
    print("done in %.0f s, %d errors" % (time.time() - t0, len(errors)))
    sys.exit(1 if errors else 0)


if __name__ == "__main__":
    main()
