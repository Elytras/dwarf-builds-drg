"""Perks from the perk bank (GD_KpiSettings.Perks.PerkAssets) with descriptions at the highest rank.

IDs come from Tools/dwarfbuilds/perk_ids.json (append-only). Type rule (beta_perks.md section 5):
ProficiencyPerkAsset -> proficiency; a ForcePassivePerkActivation effect -> passive;
MaxUseCharges, CoolDownBetweenUse or CoolDownAfterDropPodLanding > 0 -> active; else passive.

Description: every effect with text, joined by newlines, placeholders filled with the effect's value at the
highest rank (RankValues index clamped to the last value; effects without any value are debug lines and are
skipped). Proficiencies have one Value per effect and no rank list: the shown value is Value x max rank, with
max rank = the highest perk tier used by any perk rank (10). Rich tags <h> <hi> <active> <passive> </> stay;
input images like <Fire/> become [FIRE].
"""
import json
import re

from .assets import is_ref, object_name, struct_list

KPI = "/Game/Game/GameData/GD_KpiSettings.GD_KpiSettings"
RICH_TAGS = {"h", "hi", "active", "passive", ""}
INPUT_LABELS = {"fire": "FIRE", "mine": "PICKAXE", "jump": "JUMP", "use": "USE", "run": "SPRINT",
                "grenade": "GRENADE", "calldonkey": "CALL M.U.L.E.", "fearthebeardactivate": "PERK ACTION",
                "equipperkitem": "PERK ITEM", "reload": "RELOAD", "flashlight": "FLASHLIGHT", "flare": "FLARE"}
PLACEHOLDER = re.compile(r"\{([A-Za-z_]+(?:\*100)?)\}(?:\|plural\(([^)]*)\))?")


def fmt_num(v):
    """Up to two decimals, no trailing zeros: 5 -> '5', 1.5 -> '1.5', 0.125 -> '0.13'."""
    s = ("%.2f" % v).rstrip("0").rstrip(".")
    return "0" if s in ("-0", "") else s


def fill(text, v, problems, where):
    """Fill {value}, {float_value}, {percent}, {percent*100}, {percent_onebased}, {percent_zerobased}, {minutes}
    and plural forms '{value}|plural(one=x,other=y)' with the number v."""
    def one(m):
        name, plural = m.group(1), m.group(2)
        if plural is not None:
            forms = dict(p.split("=", 1) for p in plural.split(",") if "=" in p)
            return forms.get("one" if abs(v) == 1 else "other", forms.get("other", ""))
        conv = {"value": lambda x: fmt_num(x), "float_value": lambda x: fmt_num(x),
                "percent": lambda x: fmt_num(x) + "%", "percent*100": lambda x: fmt_num(x * 100) + "%",
                "percent_zerobased": lambda x: fmt_num(x * 100) + "%",
                "percent_onebased": lambda x: fmt_num((x - 1) * 100) + "%", "minutes": lambda x: fmt_num(x / 60)}
        if name not in conv:
            problems.append("%s: unknown placeholder {%s}" % (where, name))
            return m.group(0)
        return conv[name](v)
    return PLACEHOLDER.sub(one, text)


def input_tags(text):
    """<Fire/> -> [FIRE]."""
    def one(m):
        tag = m.group(1)
        label = INPUT_LABELS.get(tag.lower()) or re.sub(r"(?<=[a-z])(?=[A-Z])", " ", tag).upper()
        return "[%s]" % label
    return re.sub(r"<([A-Za-z0-9_]+)\s*/>", one, text)


def load_ids(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def save_ids(path, ids):
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("[\n" + ",\n".join("  " + json.dumps(x) for x in ids) + "\n]\n")


class Perks:
    def __init__(self, store, texts, icons):
        self.store, self.texts, self.icons = store, texts, icons
        self.problems, self.warnings = [], []

    def chain(self, asset, ref):
        """Class chain names of an effect's PerkActivation object ('export:Name')."""
        if not isinstance(ref, str) or not ref.startswith("export:"):
            return []
        e = asset.export(ref[len("export:"):])
        return self.store.native_parents(asset.ref(e["cls"])) if e else []

    def perk(self, path, max_tier):
        asset = self.store.get(path)
        if not asset:
            self.problems.append("perk missing: " + path)
            return None
        name = self.store.name(path)
        pr = asset.props(name)
        cls = object_name(asset.ref(asset.export(name)["cls"]))
        proficiency = cls == "ProficiencyPerkAsset"
        ranks = max_tier if proficiency else len(pr.get("Ranks") or [])
        lines, cooldown, charges, force_passive = [], None, None, False
        for eff in struct_list(pr.get("Effects")):
            chain = self.chain(asset, eff.get("PerkActivation"))
            force_passive |= "ForcePassivePerkActivation" in chain
            if proficiency:
                v = (eff.get("Value") or 0.0) * ranks
                have = True
            else:
                vals = [x.get("Value", 0.0) for x in struct_list(eff.get("RankValues"))]
                have = bool(vals)
                v = vals[min(max(ranks, 1), len(vals)) - 1] if vals else 0.0
            if "SetCooldownPerkActivation" in chain and have:
                cooldown = v
            if "SetMaxUsesPerkActivation" in chain and have:
                charges = v
            text = self.texts.get(eff.get("FormattedDescription"), name)
            if not text or not text.strip():
                continue
            if not have:
                self.warnings.append("%s: skipped effect text without values: %r" % (name, text[:60]))
                continue
            lines.append(input_tags(fill(text.strip(), v, self.problems, name)))
        desc = "\n".join(lines)
        for tag in re.findall(r"</?([A-Za-z]*)>", desc):
            if tag.lower() not in RICH_TAGS:
                self.problems.append("%s: unknown rich tag <%s>" % (name, tag))
        if proficiency:
            typ = "proficiency"
        elif force_passive:
            typ = "passive"
        elif (pr.get("MaxUseCharges") or 0) > 0 or (pr.get("CoolDownBetweenUse") or 0) > 0 \
                or (pr.get("CoolDownAfterDropPodLanding") or 0) > 0:
            typ = "active"
        else:
            typ = "passive"
        if (typ == "active") != ("<active>" in desc.lower()):
            self.warnings.append("%s: type %s but <active> tag %s" % (name, typ, "<active>" in desc.lower()))
        active = typ == "active"
        cooldown = cooldown if cooldown is not None else (pr.get("CoolDownBetweenUse") or 0)
        charges = charges if charges is not None else (pr.get("MaxUseCharges") or 0)
        return {"asset": name, "name": self.texts.get(pr.get("Title"), name), "type": typ,
                "icon": self.icons.add("perks", name[5:] if name.startswith("PERK_") else name, pr.get("Icon")),
                "description": desc, "ranks": ranks,
                "cooldown": round(cooldown, 3) if active else 0, "charges": int(charges) if active else 0}

    def build(self, ids_file):
        kpi = dict(self.store.props(KPI).get("Perks") or [])
        bank = [p for p in kpi.get("PerkAssets") or [] if is_ref(p)]
        if not bank:
            self.problems.append("perk bank is empty or GD_KpiSettings is missing")
        self.store.prefetch(bank)
        # Highest perk-menu tier column (rank tiers go 1..10); proficiencies get one rank per tier column.
        max_tier = max([r.get("Tier") or 0 for p in bank for r in struct_list(self.store.props(p).get("Ranks"))] + [1])
        found = {}
        for p in bank:
            if not self.store.props(p).get("bIsEquippable", True):
                continue
            perk = self.perk(p, max_tier)
            if perk:
                found[perk["asset"]] = perk
        ids = load_ids(ids_file)
        new = [a for a in found if a not in ids]
        for a in new:
            self.warnings.append("new perk appended to perk_ids.json: %s (id %d)" % (a, len(ids) + 1))
            ids.append(a)
        if new:
            save_ids(ids_file, ids)
        out = []
        for i, a in enumerate(ids):
            if a not in found:
                self.warnings.append("perk id %d %s is not in the game's perk bank, left out" % (i + 1, a))
                continue
            out.append(dict({"id": i + 1}, **found[a]))
        return out, max_tier
