"""Gear stats panel of the loadout screen, rebuilt from the cooked game (read-only).

How the game builds it: UpgradableGearComponent::GetGearStats calls the item class's Blueprint function
GetGearStatEntry (interface UpgradableGear). Every item Blueprint fills the list itself, mostly with
BPL_UpgradeHelpers functions (UPGDESC_Damage, UPGDESC_AmmoDriven, ..., UPGDESC_Generic). Those call a native
<UpgradeClass>::GetUpgradedValue(item, player, upgrade type) and FormatUpgradeToEntry(label, values,
stat format, preview format). Labels are string-table texts picked per upgrade type inside the helper.

Here the Blueprint bytecode of GetGearStatEntry and the helpers is run by a small interpreter. Upgrade values
stay symbolic: a row is (label, key, base value, format) and an equipped upgrade with the same key changes it.
Rows behind "only if upgraded" branches get a condition; an if/else on an upgrade in GetGearStatEntry runs
both ways ("!key" = not equipped); one in a function it calls repeats the whole run both ways. Anything the
interpreter cannot follow (unknown native, arithmetic on a symbolic value, a base value it cannot find) makes
the whole item unsupported: it
then gets no "stats" and the site shows the description. Base values come from BASES below, else from a
property named like the upgrade type (listed by the exporter as a warning).

Blueprints that compute with the values (text to number and back, rounding, joined texts, comparisons) keep
them symbolic as expression trees (EXPR_OPS) that the site evaluates. A comparison must give the same answer
for every build of the item; in GetGearStatEntry a comparison that does not is forked on like an upgrade and
becomes a row "when" condition.

game.json shape (Docs/dwarfbuilds-site.md section 3):
  item.stats = [{"label", "fmt", "base", "key", "better", "if", "when"} | {"label", "expr", "dexpr", "if", "when"}
                | {"label", "text", "up", "if"} | {"line", "tone", "if"}]
  if: "key", "!key", "a|b" (any), "=key" (shown build only), "^key" (equipped build only); when: trees.
  option.effects / overclock.effects = [[key, amount, 1 if percentage else 0], ...] and [key] for "equipped" keys.
"""
import itertools
import json
import math
import os
import re
import struct

from .assets import Reader, is_ref, object_name  # first: puts Tools/ on sys.path for kismet
from . import kismet

HELPERS = "/Game/WeaponsNTools/BPL_UpgradeHelpers"
SOURCE = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
                      "Source", "FSD")

# Native upgrade class -> how GetUpgradedValue finds the base value for an upgrade type (enum value name):
#   ("item", prop)          property of the item class default object
#   ("damage", prop)        damage component of the sub item (or the item) picked by DamageComponentType
#   ("comp", class, prop)   first component of that class on the item
#   ("const", value)        a fixed value (multipliers start at 1, flags at 0)
#   ("plus", n, spec) / ("times", n, spec)  another spec's value + n / x n
# Only entries listed here are supported; checked against in-game numbers where noted in the report.
# "exe:" notes: read from the native base-value code in the game exe (the switch on the upgrade type that
# returns a field of the item's default object).
BASES = {
    "DamageUpgrade": {
        "Damage": ("damage", "Damage"),
        "WeakpointDamageMultiplier": ("damage", "WeakpointDamageMultiplier"),
        "StaggerChance": ("damage", "StaggerChance"),
        "StaggerDuration": ("damage", "StaggerDuration"),
        "RadialDamage": ("damage", "RadialDamage"),
        "RadialRange": ("damage", "DamageRadius"),
        "ArmorDamageMultiplier": ("damage", "ArmorDamageMultiplier"),
        "FearFactor": ("damage", "FearFactor"),
        "FriendlyFireModifier": ("damage", "FriendlyFireModifier"),
        "ArmorPenetration": ("damage", "ArmorPenetration"),
        # UDamageComponent fields the stubs lack (names from the game exe); not set in the packages: 0
        "ProjectileLifetimeDamageMultiplier": ("damage", "ProjectileLifetimeDamageMultiplier", 0.0),
        "ProjectileLifetimeDamageMax": ("damage", "ProjectileLifetimeDamageMax", 0.0),
    },
    "AmmoDrivenWeaponUpgrade": {
        "MaxAmmo": ("item", "MaxAmmo"),
        "ClipSize": ("item", "ClipSize"),
        "RateOfFire": ("item", "RateOfFire"),
        "ReloadSpeed": ("item", "ReloadDuration"),
        "ShotCost": ("item", "ShotCost"),
        "BurstCount": ("plus", 1, ("item", "BurstCount")),  # exe: BurstCount + 1 (BRT7: 2 + 1 = 3 rounds)
        "BurstRateOfFire": ("item", "BurstCycleTime"),  # the only other burst field; shown as 1/x (fmt 11)
        "RecoilMultiplier": ("mult",),
        "RecoilMultiplierV": ("mult",),
        "RecoilMultiplierH": ("mult",),
        "HeatUpRateModifier": ("mult",),
    },
    "HitscanBaseUpgrade": {
        "MaxVerticalSpread": ("comp", "/Script/FSD.HitscanBaseComponent", "MaxVerticalSpread"),
        "MaxHorizontalSpread": ("comp", "/Script/FSD.HitscanBaseComponent", "MaxHorizontalSpread"),
        "MaxPenetrations": ("comp", "/Script/FSD.HitscanBaseComponent", "MaxPenetrations"),
        "SpreadPerShot": ("comp", "/Script/FSD.HitscanBaseComponent", "SpreadPerShot"),
        "MinSpreadWhileMoving": ("comp", "/Script/FSD.HitscanBaseComponent", "MinSpreadWhenMoving"),
        "MinSpreadWhileSprinting": ("comp", "/Script/FSD.HitscanBaseComponent", "MinSpreadWhenSprinting"),
        "MaxSpread": ("comp", "/Script/FSD.HitscanBaseComponent", "MaxSpread"),
        "RicochetChance": ("comp", "/Script/FSD.HitscanBaseComponent", "RicochetChance"),
        "WeaponAccuracySpreadMultiplier": ("mult",),
        "SpreadRecoveryMultiplier": ("mult",),
    },
    "MultiHitscanUpgrade": {
        "BulletsPerShot": ("comp", "/Script/FSD.MultiHitscanComponent", "BulletsPerShot"),
    },
    "CapsuleHitscanUpgrade": {  # exe: twice the capsule's Width (a radius; WPN_MicrowaveGun: 60 -> 120)
        "ShotWidth": ("times", 2.0, ("comp", "/Script/FSD.CapsuleHitscanComponent", "Width")),
    },
    "PickaxeUpgrade": {
        "PowerAttackCoolDown": ("item", "SpecialCooldown"),
        "EnablePowerAttack": ("const", 0),
        "RockMining": ("item", "RockMiningBonus"),
    },
    "GrenadeUpgrade": {
        "MaxGrenades": ("item", "MaxGrenades"),
        "FuseTime": ("item", "FuseTime"),
    },
    "InventoryItemUpgrade": {
        "MovementSpeedWhileUsing": ("item", "MovementRateWhileUsing"),
        "CooldownRate": ("item", "CooldownRate"),
        "UnJamDuration": ("item", "UnjamDuration"),
        "ManualCooldownDelay": ("item", "ManualCooldownDelay"),
        "ManualHeatPerUse": ("item", "ManualHeatPerUse"),
    },
    "ProjectileUpgrade": {
        "Velocity": ("mult",),  # shown as "200%": a multiplier of the projectile speed (+1.8 adds 180 %)
        # the projectile's movement MaxSpeed (micro missile 1500; one upgrade adds 750, the others multiply)
        "MaxVelocity": ("comp", "/Script/Engine.ProjectileMovementComponent", "MaxSpeed"),
        "Lifetime": ("sub", "Lifespan"),  # PRJ_LineCutter2: 1.5 s
    },
    "CapacityUpgrade": {
        None: ("comp", "/Script/FSD.CapacityHoldingItemAggregator", "MaxAmmo"),
    },
    "GooGunProjectileUpgrade": {  # the goo projectile (sub item): PuddleLifeTime 12 s
        "PuddleLifeSpan": ("sub", "PuddleLifeTime"),
        "FragmentCount": ("sub", "FragmentCount"),
    },
    "ModifyDynamicStatusEffectDamageBonusUpgrade": {  # the sub item's PushDynamicStatusEffectDamageBonus
        "Duration": ("dynste", "Duration"),  # its BaseSTE's duration (goo projectile: 4 s; upgrades x1.5, x0.5)
    },
    "SingleUsableUpgrade": {  # on the sub item (sentry gun: DeployUsable, 4 s)
        "UseDuration": ("comp", "/Script/FSD.SingleUsableComponent", "UseDuration"),
    },
    "CooldownUpgrade": {  # the item's only cooldown (grappling hook: 4 s)
        None: ("comp", "/Script/FSD.CoolDownItemAggregator", "CoolDownDuration"),
    },
    "AutoCannonUpgrade": {  # AAutoCannon fields of the same names (StatusEffectAtFullROF: a bool, 0)
        "FireTimeIncreaseScaleMultiplier": ("item", "FireTimeIncreaseScale"),
        "DamageBonusAtFullROF": ("item", "DamageBonusAtFullROF"),
        "StatusEffectAtFullROF": ("item", "StatusEffectAtFullROF"),
        "StartingFireRate": ("item", "StartingFireRate"),
        "MaxFireRate": ("item", "MaxFireRate"),
    },
    "SawedOffShotgunUpgrade": {  # ASawedOffShotgun fields (flags as 0/1)
        "FearOnShoot": ("item", "FearFactorOnFire"),
        "ShockWave": ("item", "ShockwaveEnabled"),
        "ShotgunJump": ("item", "ShotgunJumpEnabled"),
    },
    "BoltActionRifleUpgrade": {  # ABoltActionWeapon fields: "focus" is "charge" in the C++ (focus x2 damage)
        "FocusSpeed": ("item", "ChargeSpeed"),
        "AimedShotStagger": ("item", "AimedShotStaggerChance"),
        "FocusDamageBonus": ("item", "FullChargeDamageBonus"),
        "KillReloadTimeBoost": ("item", "TargetKilledReloadTimeBoost"),
        "AimedShotAmmoCost": ("item", "ChargeAmmoCost"),
        "AimedShotWeakpointDamageBonusMultiplier": ("item", "AimedShotWeakpointDamageBonusMultiplier"),
        "NoGravityOnFocus": ("item", "IsNoGravityOnFocusEnabled"),
        "FocusedWeakspotHitStatusEffect": ("const", 0.0),  # FocusedHitSTE is a class, not a number
    },
    "CryoSprayUpgrade": {  # ACryosprayItem fields; AoE cold: the AoE damage component has radius 0
        "RePressurisationTime": ("item", "RePressureDuration"),
        "LongReach": ("item", "MaxFlameDistance"),
        "AoECold": ("const", 0.0),
        "FrozenTargetsCanShatter": ("const", 0.0),
    },
    "FlameThrowerUpgrade": {  # AFlameThrowerItem fields; sticky flames: the spawner's lifetime (2 s)
        "StickyFlameDuration": ("comp", "/Script/FSD.StickyFlameSpawner", "FlameLifetime"),
        "LongReach": ("item", "MaxFlameDistance"),
        "AoEHeat": ("const", 0.0),  # the AoE heat damage component has radius 0
        "KilledTargetsExplodeChance": ("item", "KilledTargetsExplosionChance"),
    },
    "ElectricalSMGUpgrade": {
        "AoEChance": ("item", "AoEStatusEffectChance"),  # SMG Electric Arc: 0 + 0.25 = "+25%" in game
    },
    "ProjectileLauncherBaseUpgrade": {
        "VerticalSpread": ("comp", "/Script/FSD.ProjectileLauncherBaseComponent", "VerticalSpread"),
        "HorizontalSpread": ("comp", "/Script/FSD.ProjectileLauncherBaseComponent", "HorizontalSpread"),
    },
    # Bases below have no field of the same name; "const" ones have none in the C++ or the cooked data at all
    # (a feature the upgrade turns on), so they start at 0 (listed as inferred in Docs/dwarfbuilds-site.md).
    "BurstWeaponUpgrade": {  # ABurstWeapon's BurstFireDamageBonus component (Damage 0)
        "FullBurstHitBonusDamage": ("named", "BurstFireDamageBonus", "Damage"),
        "FullBurstStaggerDuraion": ("const", 0.0),  # Burst Stun: 0 + 4 s
    },
    "CoilgunUpgrade": {
        "EShotwaveRange": ("const", 0.0),  # exe: 0 (trajectory effect radius: 0 + 150 / 500 cm)
        "EResistanceWhileCharging": ("const", 0.0),  # exe: a native-only flag (0)
        "EEffectTrailDuration": ("item", "TrailSettings:ContrailSettings.LifeTime"),  # 5 s
        "EEffectTrailBonusRadius": ("item", "TrailSettings:ContrailSettings.Radius"),  # C++ default 0
    },
    "DetPackUpgrade": {  # fields of the charge (DetPackClass): ADetPack and its UExplosionComponent
        "CarveDiameter": ("via", "DetPackClass", ("named", "Explosion", "CarveDiameterCM")),
        "ExtraFearRadius": ("via", "DetPackClass", ("item", "IncreasedFearRadius")),
        "ExtraStaggerRadius": ("via", "DetPackClass", ("item", "IncreasedStaggerRadius")),
        "CanPickUp": ("const", 0.0),
        "ExplodeOnDeath": ("const", 0.0),
    },
    "PlatformGunUpgrade": {  # UItemUpgrade without Amount: the rows show the label and 0
        "BiggerPlatform": ("const", 0.0),
        "LessFallDamage": ("const", 0.0),
        "BugRepellant": ("const", 0.0),
        "CubePlatform": ("const", 0.0),
    },
    "ZiplineGunUpgrade": {  # AZipLineItem fields
        "MaxAngle": ("item", "MaxAngle"),
        "MaxDistance": ("item", "MaxDistance"),
        "MovementSpeed": ("const", 250.0),  # exe: a constant 250 (not BP_ZipLineState's cooked TravelSpeed 175)
        "FallDamageReduction": ("const", 0.0),  # exe: 0
    },
    "SentryGunUpgrade": {  # the item (ARecallableSentryGunItem) and its sentry (SentryGunObjectClass)
        "MaxAmmo": ("via", "SentryGunObjectClass", ("item", "MaxAmmoCount")),  # 90 (AmmoCount 200 is the
        # current ammo of a fresh sentry, not its capacity)
        "ReloadSpeed": ("via", "SentryGunObjectClass",
                        ("comp", "/Script/FSD.ReloadSentryUsableComponent", "AmmoPerSec")),
        "ExtraSentry": ("item", "MaxSentryCount"),
        "MaxRange": ("via", "SentryGunObjectClass", ("item", "AquisitionRange")),
        "RotationSpeed": ("via", "SentryGunObjectClass", ("item", "RotationSpeed")),
        "BurstCooldown": ("via", "SentryGunObjectClass", ("item", "BurstShotCoolDown")),
        "AngleRestriction": ("const", 0.0),
        "ManualTargeting": ("const", 0.0),
    },
    "LineCutterProjectileUpgrade": {  # ALineCutterProjectile fields of the item's projectile (its launcher's class)
        "LineSize": ("launcher", ("item", "LineMaxSize")),
        "LineSizeAdd": ("launcher", ("item", "LineMaxSize")),  # C++ default 150 (1.5 m)
        "SetDeployDelay": ("launcher", ("item", "LineExpansionDelay")),
        "AddDeployTime": ("launcher", ("item", "TimeToLineExpansion")),
        "SetDeployTime": ("launcher", ("item", "TimeToLineExpansion")),
    },
    "CrossbowUpgrade": {  # ACrossbow fields; the projectile argument is the default or the special bolt
        "DamageDefault": ("damage", "Damage"),  # PRJ_Crossbow_Projectile MainDamage: 105
        "RadialDamageDefault": ("damage", "RadialDamage"),  # 60
        "IncreaseSpecialAmmo": ("item", "SpecialAmmoMax"),  # exe; C++ default 0: the special bolt adds 9 / 12
        # exe: a field of the projectile argument, 0 without one (no special bolt equipped)
        "IncreaseSpecialEffectDuration": ("sub", "StatusEffectTime", 0.0),  # special bolts: 10 / 12 / 14 s
        "ReloadTimeDefaultArrow": ("item", "ReloadDuration"),  # exe; "Get Reload Stat" adds its base
        "BansheeModule": ("const", 0.0),  # exe: a projectile field, 0 without a projectile (the row passes none)
    },
    "ShieldGeneratorUpgrade": {  # AShieldGeneratorItem and its thrown AShieldGeneratorActor (ThrowableClass)
        "Radius": ("via", "ThrowableClass", ("item", "GeneratorRadius")),
        "Duration": ("via", "ThrowableClass", ("item", "GeneratorLifetime")),
        "DeployTime": ("via", "ThrowableClass", ("item", "DeployDelay")),
        "PushStatusEffect": ("via", "ThrowableClass", ("item", "bPushStatusEffectOnExit")),
        "RechargeTime": ("item", "RechargeDuration"),
    },
}

# Upgrade classes whose value is a bonus that starts at 0: class -> (asset property that must match the
# helper's argument, or None; asset property with the amount, or None). Their rows only show when upgraded.
BONUS = {
    "FlatDamageUpgrade": ("DamageClass", "Damage"),
    "DamageConversionUpgrade": ("DamageClass", "ConversionPercentage"),
    "GatlingHotShellsBonusUpgrade": ("DamageClass", "ConversionPercentage"),
    "TargetSpecificDamageBonusUpgrade": ("Key", "Bonus"),
    "PushSatusEffectDamageBonusUpgrade": ("StatusEffect", "Chance"),
    "LockCountSTEBonusUpgrade": ("StatusEffect", "Amount"),
    "FearAoEBonusUpgrade": (None, "FearFactor"),
    "RandomDamageUpgrade": (None, None),
    "StatusDamageBonusUpgrade": ("StatusEffects", "Bonus"),  # an array: the helper's status effect is in it
    "TargetStateDamageBonusUpgrade": ("TargetState", "Bonus"),  # an enum: "ETargetStateDamageBonusType::OnFire"
    # shown as a label only (stat format 5): only "equipped" matters
    "CryoSprayProjectileUpgrade": ("ProjectileClass", None),
    "StickyFlameStatusEffectUpgrade": ("StatusEffect", None),
}
# Typed upgrades that GetUpgradedValue also matches on a class argument: upgrade class -> asset properties
# that may hold it (key "<class>.<type>~<argument>"). Grappling hook: Safety First and Momentum share the type.
PARAM_KEYS = {"TriggeredStatusEffectUpgrade": ("StatusEffect", "OwnerEffect")}

# The item's own damage bonuses (DamageComponent.DamageBonuses) of the matching class: upgrade class ->
# (bonus class, True when the value starts at their sum). Checked for PushStatus: the SMG has a built-in 25 %,
# with Upgraded Capacitors (25 %) the game shows "+50%". False: not known, an item with such a built-in bonus
# (same damage class or status) is unsupported.
BONUS_BASE = {
    "PushSatusEffectDamageBonusUpgrade": ("PushStatusEffectDamageBonus", True),
    # Cryospray "Freezing Power" / Flamethrower "Heat" are always shown and are these built-in bonuses (8, 10)
    "FlatDamageUpgrade": ("FlatDamageBonus", True),
    # Heavy Particle Cannon "Direct Target Heating": always shown, its built-in 25 % heat (+75 % from T4)
    "DamageConversionUpgrade": ("DamageConversionBonus", True),
    "StatusDamageBonusUpgrade": ("StatusDamageBonus", False),
    "TargetStateDamageBonusUpgrade": ("TargetStateDamageBonus", False),
    "TargetSpecificDamageBonusUpgrade": ("TargetSpecificDamageBonus", False),
}

# Damage component types set by native constructors (not in the cooked data): (native class, component) -> type.
# PickaxeItem: the power attack upgrades target Secondary and change SpecialDamage (in game: 120 + 120 = 240).
NATIVE_DAMAGE_TYPES = {("PickaxeItem", "SpecialDamage"): 1}


# Enums the stubs have shorter than the game (values read from the game exe's reflection names, in order).
ENUMS_FROM_GAME = {
    "EDamageUpgrade": ["Damage", "WeakpointDamageMultiplier", "StaggerChance", "StaggerDuration", "RadialDamage",
                       "RadialRange", "ArmorDamageMultiplier", "FearFactor", "RadialRangeMultiplier",
                       "StaggerOnWeakpointOnly", "FriendlyFireModifier", "ShattersArmor", "ArmorPenetration",
                       "ProjectileLifetimeDamageMultiplier", "SelfFriendlyFireMultiplier",
                       "ProjectileLifetimeDamageMax"],
    # the stubs lack EExplodingTrail in the middle (values from 21 on would shift)
    "ECoilgunUpgrades": ["EResistanceWhileCharging", "ETransferAilment", "EFlamekills", "EOvercharge", "EUnderCharge",
                         "EWeaponBlast", "EShotwaveRange", "EImpactFearFactor", "EDamageOnShotWave",
                         "EImpactFearRadius", "EEffectTrailDuration", "EEffectTrailBonusRadius", "EBrokenShieldBonus",
                         "EPostChargeBurst", "EOwnerShieldCapacity", "EBlockShieldWhileCharing",
                         "EWeaknessWhileCharging", "EPierceDepthDamage", "EBurnGround", "EElectricTrail",
                         "EFireTrail", "EExplodingTrail", "EPenetrationDepth", "EPrimaryBonusShotWidth",
                         "EShieldDelay", "EShowFallbackShotEffects"],
}


# C++ constructor defaults of classes the stubs lack, read from the constructor code in the game exe
# (immediates stored to the reflected field offsets). AElectricField: the Supercell Payload field.
EXE_CTOR_DEFAULTS = {
    "ElectricField": {"FieldRadius": 500.0, "FieldDuration": 6.0, "ElectricDamagePerTick": 0.0,
                      "DamageTickInterval": 0.5, "FriendlyFireDamageMultiplier": 0.5, "FieldExtinguishTime": 1.5},
}


class Unsupported(Exception):
    pass


# ---------------------------------------------------------------- native classes from the stub source
class Native:
    """Class parents, constructor defaults and enums of the game's C++ (stub headers and constructors)."""

    def __init__(self, root=SOURCE):
        self.parent, self.ctor, self.enums, self.sigs = {}, {}, {}, {}
        pub, priv = os.path.join(root, "Public"), os.path.join(root, "Private")
        cls_re = re.compile(r"class\s+(?:\w+_API\s+)?[AUF](\w+)\s*:\s*public\s+[AUFI](\w+)")
        enum_re = re.compile(r"enum class (\w+)\s*:\s*\w+\s*\{(.*?)\};", re.S)
        sig_re = re.compile(r"static FUpgradeValues (GetUpgradedValue|GetGenericUpgradedValue)\((.*?)\);")
        for f in os.listdir(pub):
            with open(os.path.join(pub, f), encoding="utf-8", errors="replace") as fh:
                src = fh.read()
            for m in cls_re.finditer(src):
                self.parent.setdefault(m.group(1), m.group(2))
            for m in enum_re.finditer(src):
                # indexed by value; explicit values ("ShockWave = 2") leave None gaps
                names = []
                for part in m.group(2).split(","):
                    part = part.split("UMETA")[0].strip()
                    if not part:
                        continue
                    name, _, val = part.partition("=")
                    val = val.strip()
                    if val.isdigit():
                        names.extend([None] * (int(val) - len(names)))
                    names.append(name.strip())
                self.enums[m.group(1)] = names
            for m in sig_re.finditer(src):
                owner = cls_re.search(src)
                if owner:
                    params = [p.strip().rsplit(" ", 1) for p in m.group(2).split(",")]
                    self.sigs[owner.group(1)] = [(t.strip(), n.strip("*& ")) for t, n in params]
        self.enums.update(ENUMS_FROM_GAME)
        # classes by name without prefix; structs (FContrailSettings) as "F" + name
        ctor_re = re.compile(r"\b([AUF])(\w+)::\1\2\((?:const FObjectInitializer& ObjectInitializer)?\)[^{;]*\{(.*?)\n\}",
                             re.S)
        set_re = re.compile(r"this->(\w+) = ([^;]+);")
        for f in os.listdir(priv):
            with open(os.path.join(priv, f), encoding="utf-8", errors="replace") as fh:
                src = fh.read()
            for m in ctor_re.finditer(src):
                name = ("F" if m.group(1) == "F" else "") + m.group(2)
                vals = {}
                for prop, v in set_re.findall(m.group(3)):
                    v = v.strip()
                    if re.fullmatch(r"-?\d+\.\d+f", v):
                        vals[prop] = float(v[:-1])
                    elif re.fullmatch(r"-?\d+", v):
                        vals[prop] = int(v)
                    elif v in ("true", "false"):
                        vals[prop] = v == "true"
                    elif v == "NULL":
                        vals[prop] = None
                    elif re.fullmatch(r"\w+::\w+", v):
                        vals[prop] = v
                    elif v.startswith("CreateDefaultSubobject"):
                        sub = re.search(r'TEXT\("([^"]+)"\)', v)
                        vals[prop] = "subobject:" + sub.group(1) if sub else None
                self.ctor[name] = vals
        for name, vals in EXE_CTOR_DEFAULTS.items():
            self.ctor.setdefault(name, {}).update(vals)

    def chain(self, name):
        out = []
        while name and len(out) < 30:
            out.append(name)
            name = self.parent.get(name)
        return out

    def defaults(self, name):
        merged = {}
        for n in reversed(self.chain(name)):
            ci_update(merged, self.ctor.get(n, {}))
        return merged

    def is_a(self, name, base):
        return base in self.chain(name)

    def subobjects(self, name):
        """Names of the default subobjects the C++ constructors create, in creation order (base class first)."""
        out = []
        for n in reversed(self.chain(name)):
            for v in self.ctor.get(n, {}).values():
                if isinstance(v, str) and v.startswith("subobject:") and v[10:] not in out:
                    out.append(v[10:])
        return out

    def enum_index(self, value):
        """'EDamageComponentType::Secondary' -> 1."""
        if isinstance(value, str) and "::" in value:
            enum, name = value.split("::", 1)
            names = self.enums.get(enum)
            if names and name in names:
                return names.index(name)
        return value


def ci_update(base, props):
    """dict.update where keys that differ only in case are the same (FNames ignore case)."""
    lower = {k.lower(): k for k in base}
    for k, v in props.items():
        old = lower.get(k.lower())
        if old is not None and old != k:
            del base[old]
        base[k] = v
        lower[k.lower()] = k


# ---------------------------------------------------------------- objects of the cooked game
class Obj:
    """A default object, component template or data asset with merged properties."""

    def __init__(self, path, cls, props, native):
        self.path, self.cls, self.props, self.native = path, cls, props, native
        self.vars = {}

    def get(self, name):
        if name in self.vars:
            return self.vars[name]
        if name in self.props:
            return self.props[name]
        for k, v in self.props.items():  # FNames ignore case
            if k.lower() == name.lower():
                return v
        raise Unsupported("no property %s on %s" % (name, object_name(self.path)))

    def __repr__(self):
        return "Obj(%s)" % object_name(self.path)


class World:
    def __init__(self, store, texts, native):
        self.store, self.texts, self.native = store, texts, native
        self.cdos, self.objs, self.funcs, self.comps = {}, {}, {}, {}

    def text(self, t):
        if isinstance(t, dict):
            return self.texts.get(t) or ""
        return t

    def export_path(self, asset, e):
        """Full object path of an export: /Game/Pkg.Outer.Name."""
        names = []
        while e:
            names.append(e["name"])
            e = asset.exports[e["outer"] - 1] if e["outer"] > 0 else None
        return asset.game_path + "." + ".".join(reversed(names))

    def find_export(self, path):
        """(asset, export) for an object path like /Game/A/B.Default__B_C.Damage."""
        asset = self.store.get(path)
        if not asset:
            return None, None
        parts = path.split(".")[1:]
        parts = [x.lower() for x in parts]
        for e in asset.exports:
            if [x.lower() for x in self.export_path(asset, e).split(".")[1:]] == parts:
                return asset, e
        return asset, None

    def native_name(self, cls):
        """Native class name of a class path (Blueprint classes: their first native parent)."""
        seen = 0
        while cls and cls.startswith("/Game/") and seen < 20:
            seen += 1
            asset = self.store.get(cls)
            e = asset and asset.export(object_name(cls))
            if not e:
                return None
            cls = asset.ref(e["sup"])
        return object_name(cls) if cls else None

    def class_chain(self, cls):
        out = []
        while cls and len(out) < 20:
            out.append(cls)
            if not cls.startswith("/Game/"):
                break
            asset = self.store.get(cls)
            e = asset and asset.export(object_name(cls))
            if not e:
                break
            cls = asset.ref(e["sup"])
        return out

    def is_a(self, cls, base):
        if base in self.class_chain(cls):
            return True
        nat, bnat = self.native_name(cls), object_name(base)
        return bool(nat) and base.startswith("/Script/") and self.native.is_a(nat, bnat)

    def merged(self, asset, e, depth=0):
        """Properties of an export over its template chain; native templates give constructor defaults."""
        props = dict(tolerant_props(asset, e))
        tmpl = asset.ref(e["tmpl"])
        if tmpl.startswith("/Game/") and depth < 20:
            ta, te = self.find_export(tmpl)
            base = self.merged(ta, te, depth + 1) if te else {}
        else:
            cls = asset.ref(e["cls"])
            nat = object_name(cls) if cls.startswith("/Script/") else self.native_name(
                self.class_of(asset, e))
            base = self.native.defaults(nat or "")
        ci_update(base, props)
        return base

    def class_of(self, asset, e):
        cls = asset.ref(e["cls"])
        if cls.startswith("export:"):
            return asset.game_path + "." + cls[len("export:"):]
        return cls

    def cdo(self, cls):
        if cls not in self.cdos:
            if not cls or not cls.startswith("/Game/"):
                nat = object_name(cls or "")
                self.cdos[cls] = Obj(cls, cls, self.native.defaults(nat), nat)
            else:
                asset = self.store.get(cls)
                e = asset and asset.export("Default__" + object_name(cls))
                if not e:
                    raise Unsupported("no default object for " + str(cls))
                self.cdos[cls] = Obj(cls, cls, self.merged(asset, e), self.native_name(cls))
        return self.cdos[cls]

    def obj(self, path):
        """Data asset or other object by path (an upgrade, a status effect class, ...)."""
        if path not in self.objs:
            if path.endswith("_C") and self.store.get(path) and self.store.get(path).export(
                    "Default__" + object_name(path)):
                self.objs[path] = self.cdo(path)
            else:
                asset, e = self.find_export(path)
                if not e:
                    raise Unsupported("object missing: " + path)
                cls = self.class_of(asset, e)
                self.objs[path] = Obj(path, cls, self.merged(asset, e), object_name(cls))
        return self.objs[path]

    def components(self, cls):
        """{name: Obj} of the components of a class: default subobjects, construction script, overrides."""
        if cls in self.comps:
            return self.comps[cls]
        out = {}
        for c in reversed(self.class_chain(cls)):
            if not c.startswith("/Game/"):
                continue
            asset = self.store.get(c)
            cdo_name = "Default__" + object_name(c)
            for i, e in enumerate(asset.exports):
                outer = asset.exports[e["outer"] - 1]["name"] if e["outer"] > 0 else None
                ecls = asset.ref(e["cls"])
                if outer == cdo_name and not ecls.endswith(".Function"):
                    out[e["name"]] = (asset, e)
                elif ecls.endswith("SCS_Node"):
                    pr = dict(asset.export_props(e))
                    t = pr.get("ComponentTemplate", "")
                    if t.startswith("export:") and pr.get("InternalVariableName"):
                        te = asset.export(t[len("export:"):])
                        if te:
                            out[pr["InternalVariableName"]] = (asset, te)
                elif ecls.endswith("InheritableComponentHandler"):
                    for rec in dict(asset.export_props(e)).get("Records") or []:
                        rec = dict(rec)
                        key = dict(rec.get("ComponentKey") or [])
                        t = rec.get("ComponentTemplate", "")
                        if t.startswith("export:") and key.get("SCSVariableName"):
                            te = asset.export(t[len("export:"):])
                            if te:
                                out[key["SCSVariableName"]] = (asset, te)
        res = {}
        for name, (asset, e) in out.items():
            ccls = self.class_of(asset, e)
            res[name] = Obj(self.export_path(asset, e), ccls, self.merged(asset, e),
                            self.native_name(ccls) if ccls.startswith("/Game/") else object_name(ccls))
        self.comps[cls] = res
        return res

    def component_of(self, cls, comp_cls):
        """First component of that class; one of exactly that class first (the sentry gun's deploy usable
        before its ReloadSentryUsableComponent)."""
        comps = [o for o in self.components(cls).values() if self.is_a(o.cls, comp_cls)]
        exact = [o for o in comps if o.native == object_name(comp_cls)]
        return (exact or comps or [None])[0]

    def components_of(self, cls, comp_cls):
        """Components of that class in the order the C++ constructors create them, then the Blueprint's own
        (WPN_ShieldRegeneratorItem: ChargeCapacity, then Capacity)."""
        order = self.native.subobjects(self.native_name(cls) or "")
        comps = [(n, o) for n, o in self.components(cls).items() if self.is_a(o.cls, comp_cls)]
        comps.sort(key=lambda x: order.index(x[0]) if x[0] in order else len(order))
        return [o for _, o in comps]

    def damage_component(self, cls, dct):
        """Damage component of a class by EDamageComponentType index (0 Primary, 1 Secondary, ...). Several of
        one type: the first the C++ constructor creates (WPN_Cryospray: DamageComponent before
        AoEDamageComponent, though the package lists them the other way), then the Blueprint's own ones."""
        found = []
        nat = self.native_name(cls)
        order = self.native.subobjects(nat or "")
        comps = list(self.components(cls).items())
        comps.sort(key=lambda x: order.index(x[0]) if x[0] in order else len(order))  # stable: package order
        for name, o in comps:
            if self.is_a(o.cls, "/Script/FSD.DamageComponent"):
                t = next((v for (c, n), v in NATIVE_DAMAGE_TYPES.items()
                          if n == name and self.native.is_a(nat or "", c)), None)
                if t is None:
                    t = o.props.get("DamageComponentType", 0)
                found.append((self.native.enum_index(t), o))
        for t, o in found:
            if t == dct:
                return o
        raise Unsupported("no damage component %s on %s" % (dct, object_name(cls)))

    def damage_bonus(self, cls, bonus_cls, prop, param, field):
        """Sum of field over the damage bonuses of class bonus_cls on every damage component of cls whose prop
        is param (or holds it, for an array): (sum, how many)."""
        total, count = 0.0, 0
        for bo in self.damage_bonus_objects(cls, bonus_cls):
            v = bo.props.get(prop)
            names = [object_name(x) if is_ref(x) else x for x in (v if isinstance(v, list) else [v])]
            if (object_name(param) if is_ref(param) else param) in names:
                total += float(bo.props.get(field) or 0.0)
                count += 1
        return total, count

    def damage_bonus_objects(self, cls, bonus_cls):
        """The damage bonuses of class bonus_cls on every damage component of cls."""
        out = []
        for o in self.components(cls).values():
            if not self.is_a(o.cls, "/Script/FSD.DamageComponent"):
                continue
            for b in o.props.get("DamageBonuses") or []:
                if not isinstance(b, str):
                    continue
                if b.startswith("export:"):  # instanced subobject in the component's package
                    asset, ce = self.find_export(o.path)
                    name = b[len("export:"):]
                    # the one inside this component first: names repeat under other components
                    # (WPN_HeavyParticleCannon: DamageConversionBonus_0 of Damage and of ExplosionDamage)
                    inner = [x for i, x in enumerate(asset.exports) if x["name"] == name and ce
                             and x["outer"] == asset.exports.index(ce) + 1] if asset else []
                    e = inner[0] if inner else asset and asset.export(name)
                    if not e:
                        raise Unsupported("damage bonus %s missing" % b)
                    bo = Obj(b, self.class_of(asset, e), self.merged(asset, e), None)
                else:
                    bo = self.obj(b)
                if object_name(bo.cls) == bonus_cls:
                    out.append(bo)
        return out

    def function(self, cls, name):
        """(Function, owning class) for a Blueprint function name on a class chain."""
        for c in self.class_chain(cls):
            if not c.startswith("/Game/"):
                break
            key = (c, name)
            if key not in self.funcs:
                asset = self.store.get(c)
                e = asset and kismet.functions(asset).get(name)
                self.funcs[key] = kismet.Function(asset, e) if e else None
            if self.funcs[key]:
                return self.funcs[key], c
        return None, None

    def function_at(self, ref):
        """Function for a reference like /Game/A/B.B_C.Func."""
        parts = ref.split(".")
        return self.function(parts[0] + "." + parts[1], parts[-1])


def tolerant_props(asset, e):
    """Tagged properties of an export; a property that does not decode is left out (value "<undecoded>")."""
    r = Reader(asset.body, asset.names)
    r.p = e["off"] - asset.body_base
    out = []
    while True:
        name = r.fname()
        if name == "None":
            break
        typ = r.fname()
        size, aidx = r.i32(), r.i32()
        tag = {}
        if typ == "StructProperty":
            tag["struct"] = r.fname()
            r.p += 16
        elif typ == "BoolProperty":
            tag["bool"] = bool(r.u8())
        elif typ in ("ByteProperty", "EnumProperty"):
            tag["enum"] = r.fname()
        elif typ in ("ArrayProperty", "SetProperty"):
            tag["inner"] = r.fname()
        elif typ == "MapProperty":
            tag["inner"] = r.fname()
            tag["value"] = r.fname()
        if r.u8():
            r.p += 16
        end = r.p + (0 if typ == "BoolProperty" else size)
        try:
            val = asset.read_value(r, typ, size, tag, 0)
        except (IndexError, ValueError, KeyError, UnicodeDecodeError, struct.error):
            val = "<undecoded>"
        r.p = end
        out.append((name if aidx == 0 else "%s[%d]" % (name, aidx), val))
    return out


# ---------------------------------------------------------------- symbolic values
class UV:
    """Symbolic FUpgradeValues: key and base value."""

    def __init__(self, key, base):
        self.key, self.base = key, base

    def __repr__(self):
        return "UV(%s=%s)" % (self.key, self.base)


class Cond:
    """A boolean that is true when an equipped upgrade has this key (negate: when none has it)."""

    def __init__(self, key, negate=False):
        self.key, self.negate = key, negate

    def text(self):
        return ("!" if self.negate else "") + self.key

    def inverse(self):
        return [Cond(self.key, not self.negate)]


class AnyCond:
    """OR of plain upgrade conditions (cond text "a|b"); its inverse is "!a" and "!b"."""

    def __init__(self, keys):
        self.keys = list(dict.fromkeys(keys))

    def text(self):
        return "|".join(self.keys)

    def inverse(self):
        return [Cond(k, True) for k in self.keys]


def cond_parts(c):
    """'!=a|b' -> (True, '=', ['a', 'b']): negated, build prefix, keys. Prefix "=": the shown build only (the
    hovered one; PreviewStatus 1 or 2), "^": the equipped build only (GetIsItemUpgradeEquipped), "": either."""
    neg = c.startswith("!")
    body = c[1:] if neg else c
    pre = body[0] if body[:1] in ("=", "^") else ""
    return neg, pre, body[len(pre):].split("|")


def cond_keys(c):
    """Upgrade keys a cond text ("a", "!a", "a|b", "=a", "^a", "?<tree>") depends on."""
    if c.startswith("?"):
        return sorted(k for k, _ in tree_keys(json.loads(c[1:])))
    return cond_parts(c)[2]


class Status:
    """PreviewStatus of a symbolic value: read as Interp.status_as (1 Upgraded, the 'equipped' path).
    pfmt / base: set when FormatPreviewStatus made it from the value (FormatUpgradeToEntry), so the panel
    colours its delta by direction (0 higher is better, 1 lower is better); None: the native status."""

    def __init__(self, key, pfmt=None, base=None, delta=None):
        self.key, self.pfmt, self.base = key, pfmt, base
        self.delta = delta  # change tree for the colour, when not that of one value (StatusPair)

    def delta_tree(self):
        if self.delta is not None:
            return self.delta
        if self.key and self.base is not None:
            return ["-", ["val", self.key, self.base], ["old", self.key, self.base]]
        return None


class StatusPair:
    """Less_ByteByte of two PreviewStatus values (WPN_Crossbow "Get Reload Stat" keeps the larger one for a row
    made of two upgrade values). FormatPreviewStatus gives 1 unchanged, 2 better, 3 worse, so the larger one is
    red when either value gets worse, green when one gets better and none worse: ["worse", a, b] of the two
    changes, each signed so that > 0 is better."""

    def __init__(self, a, b):
        self.a, self.b = a, b

    def merged(self):
        parts = []
        for x in (self.a, self.b):
            t, d = x.delta_tree(), {0: 1, 1: -1}.get(x.pfmt)
            if t is None or d is None:
                return Status(self.a.key or self.b.key)
            parts.append(["*", t, d])
        return Status(None, 0, None, ["worse"] + parts)


class VText:
    """Value text of a row: formatted from an upgrade value at display time (part 0 value, 1 preview delta)."""

    def __init__(self, uv, fmt, pfmt, part=0):
        self.uv, self.fmt, self.pfmt, self.part = uv, fmt, pfmt, part

    def tree(self):
        return ["text", self.uv.key, self.uv.base, self.fmt, self.part]


class Expr:
    """A number or text computed from upgrade values at display time. tree: JSON expression (EXPR_OPS), the
    same form Web/public/js/stats.js evaluates."""

    def __init__(self, tree):
        self.tree = tree

    def __repr__(self):
        return "Expr(%r)" % (self.tree,)


class StatusIs:
    """PreviewStatus == / != a value other than Normal. Alone it reads the status as Upgraded (the path where
    the row shows); inside a Pred it is ["status", key] (1 when equipped, see EXPR_OPS)."""

    def __init__(self, key, value, equal, assumed=1):
        self.key, self.value, self.equal, self.assumed = key, value, equal, assumed

    def tree(self):
        return ["==" if self.equal else "!=", ["status", self.key], self.value]

    def __bool__(self):
        return (self.assumed == self.value) == self.equal


class NeedAssume(Exception):
    """A branch on an upgrade outside GetGearStatEntry: Interp.run_assuming repeats the run both ways."""

    def __init__(self, cond):
        Exception.__init__(self, cond.text())
        self.cond = cond


class NotConstant(Unsupported):
    def __init__(self):
        Unsupported.__init__(self, "branch on an upgrade value that differs between builds")


class Pred:
    """A comparison on upgrade values (expression tree), decided when used: it must give the same answer for
    every build (else NotConstant; GetGearStatEntry then forks on it, rows get it as a "when" condition)."""

    def __init__(self, interp, tree):
        self.interp, self.tree = interp, tree

    def __bool__(self):
        return self.interp.decide(self.tree)

    @staticmethod
    def combine(interp, a, b, is_and):
        """AND / OR of Preds, StatusIs and bools."""
        def tree(x):
            if isinstance(x, Pred):
                return x.tree
            if isinstance(x, StatusIs):
                return x.tree()
            return bool(concrete(x, "logic"))
        return Pred(interp, ["and" if is_and else "or", tree(a), tree(b)])

    def negated(self):
        return Pred(self.interp, ["not", self.tree])


class PredCond:
    """An assumed Pred on one path of a fork in GetGearStatEntry; cond text "?" + JSON tree."""

    def __init__(self, tree):
        self.tree = tree

    def text(self):
        return "?" + json.dumps(self.tree, separators=(",", ":"))

    def inverse(self):
        return [PredCond(self.tree[1] if self.tree[0] == "not" else ["not", self.tree])]


SYM = (UV, Cond, AnyCond, Status, StatusPair, VText, Expr)
CONDS = (Cond, AnyCond, PredCond)

# Expression trees (lists, first item the op); leaves are numbers and strings.
#   ["val", key, base]   value of an upgrade key in the shown build ((base + adds) x mults, 32-bit floats)
#   ["old", key, base]   the same in the build compared with (the equipped one while previewing)
#   ["text", key, base, fmt, part]  FormatValues text of that value: part 0 value, 1 preview delta
#   ["num", e]  leading number of a text (FCString::Atof)   ["inum", e]  leading integer (Atoi)
#   ["+", a, b] ["-", a, b] ["*", a, b] ["/", a, b] (0 when b is 0)   ["round", e] ["floor", e] ["trunc", e]
#   ["itext", e, sign, grouping]  integer text   ["ftext", e, sign, grouping, minFrac, maxFrac]  number text
#   ["cat", a, b, ...]  joined texts   ["fstr", e]  FString::SanitizeFloat ("450.0", "0.25")
#   ["pct", key, old]   product of the percentage amounts of that key (old: in the compared build)
#   ["worse", a, b, ...]  -1 when any is < 0, else 1 when any is > 0, else 0 (the larger of PreviewStatus values)
# Conditions (row "when"): ["<", a, b] ["<=", a, b] [">", ..] [">=", ..] ["==", ..] ["!=", ..] ["and", a, b]
#   ["or", a, b] ["not", a]   ["status", key]  PreviewStatus (EItemPreviewStatus): 0 Normal (not equipped),
#   1 Upgraded (equipped, the preview keeps it), 2 Previewed (the preview adds or changes it), 3 PreviewReduced
#   (the preview removes it); 4 UpgradedAndPreviewReduced is not modelled (the site never gives it)
EXPR_OPS = {"val", "old", "text", "num", "inum", "+", "-", "*", "/", "round", "floor", "trunc", "itext", "ftext",
            "cat", "fstr", "pct", "worse", "<", "<=", ">", ">=", "==", "!=", "and", "or", "not", "status"}
CMP = {"<": lambda x, y: x < y, "<=": lambda x, y: x <= y, ">": lambda x, y: x > y, ">=": lambda x, y: x >= y,
       "==": lambda x, y: x == y, "!=": lambda x, y: x != y}


def tree_of(v):
    """Interpreter value -> expression tree (numbers and strings stay as they are)."""
    if isinstance(v, Expr):
        return v.tree
    if isinstance(v, VText):
        return v.tree()
    if isinstance(v, UV):
        return ["val", v.key, v.base]
    if isinstance(v, bool):
        return int(v)
    if isinstance(v, (int, float, str)):
        return v
    raise Unsupported("expression of %s" % type(v).__name__)


def copy_tree(t):
    return [copy_tree(x) for x in t] if isinstance(t, list) else t


def tree_keys(t, out=None):
    out = set() if out is None else out
    if isinstance(t, list):
        if t[0] in ("val", "old", "text") and t[1]:
            out.add((t[1], t[2]))
        elif t[0] in ("status", "pct") and t[1]:
            out.add((t[1], None))
        for x in t[1:]:
            tree_keys(x, out)
    return out


def f32(x):
    return struct.unpack("<f", struct.pack("<f", x))[0]


def leading_number(s, integer=False):
    """Like C atof / atoi: the number at the start of a text ("+20%" -> 20, "1,200" -> 1), else 0."""
    m = re.match(r"\s*([+-]?(\d+" + ("" if integer else r"(\.\d*)?|\.\d+") + r"))", str(s))
    if not m:
        return 0 if integer else 0.0
    return int(m.group(1)) if integer else float(m.group(1))


def eval_tree(t, value, fmt_stat, status=None):
    """value(key, base, old) -> number; fmt_stat(fmt, base, key, part) -> text; status(key) -> 0 / 1.
    Mirrors evalExpr in stats.js."""
    if not isinstance(t, list):
        return t
    op = t[0]
    if op == "val":
        return value(t[1], t[2], False)
    if op == "old":
        return value(t[1], t[2], True)
    if op == "text":
        return fmt_stat(t[3], t[2], t[1], t[4])
    if op == "status":
        return status(t[1])
    if op == "pct":
        return value(t[1], None, t[2])  # base None: the product of the percentage amounts
    a = [eval_tree(x, value, fmt_stat, status) for x in t[1:]]
    if op == "fstr":
        return sanitize_float(a[0])
    if op in CMP:
        return CMP[op](a[0], a[1])
    if op == "and":
        return bool(a[0]) and bool(a[1])
    if op == "or":
        return bool(a[0]) or bool(a[1])
    if op == "not":
        return not a[0]
    if op == "num":
        return f32(leading_number(a[0]))
    if op == "inum":
        return leading_number(a[0], True)
    if op == "+":
        return f32(a[0] + a[1])
    if op == "-":
        return f32(a[0] - a[1])
    if op == "*":
        return f32(a[0] * a[1])
    if op == "/":
        return f32(a[0] / a[1]) if a[1] else 0.0
    if op == "round":
        return int(math.floor(a[0] + 0.5))
    if op == "floor":
        return int(math.floor(a[0]))
    if op == "trunc":
        return int(a[0])
    if op == "itext":
        return format_number(int(a[0]), a[1], a[2], 0, 0)
    if op == "ftext":
        return format_number(a[0], a[1], a[2], a[3], a[4])
    if op == "cat":
        return "".join(str(x) for x in a)
    if op == "worse":
        return -1 if any(x < 0 for x in a) else 1 if any(x > 0 for x in a) else 0
    raise Unsupported("expression op " + str(op))


def concrete(v, what="value"):
    if isinstance(v, SYM):
        raise Unsupported("symbolic %s used in %s" % (type(v).__name__, what))
    return v


# ---------------------------------------------------------------- interpreter
class Ref:
    """Assignable place: a frame variable, an object variable or a struct member."""

    def __init__(self, get, put):
        self.get, self.put = get, put


class Frame:
    def __init__(self, func, owner, this, conds, top=False):
        self.func, self.owner, self.this, self.top = func, owner, this, top
        self.locals, self.conds = {}, list(conds)


def copy_value(v):
    if isinstance(v, dict):
        return {k: copy_value(x) for k, x in v.items()}
    return v


class Interp:
    MAX_STEPS = 200000

    def __init__(self, world, item_cls, choices=(), status_as=1):
        """choices: per tier, then the overclocks: the effect lists of its options (see Stats.effects).
        status_as: how a branch on PreviewStatus reads it (1 Upgraded; 3 PreviewReduced: the texts the
        Blueprint shows for an upgrade the hovered build removes)."""
        self.world, self.item_cls, self.choices, self.status_as = world, item_cls, choices, status_as
        self.rows, self.stats_list, self.steps, self.forks = [], None, 0, 0
        self.guessed = set()
        self.assumed = []
        self.natives = Natives(self)

    def decide(self, tree):
        """A condition tree evaluated for every build of the item (one option or none per tier and for the
        overclock): the result when it is the same for all, else NotConstant."""
        keys = tree_keys(tree)  # no preview here: the compared build is the shown one
        groups = []
        for group in self.choices:
            rel = [effs for effs in group
                   if any(len(e) > 1 and any(key_matches(e[0], k) for k, _ in keys) for e in effs)]
            groups.append([[]] + rel)
        seen = set()
        for n, combo in enumerate(itertools.product(*groups)):
            if n > 50000:
                raise Unsupported("too many builds to check a branch")
            effects = [e for effs in combo for e in effs]

            def value(k, b, old):
                if k is None:
                    return 1.0 if b is None else b
                return build_value(k, b, effects)

            def active(k):
                return k is not None and any(key_matches(e[0], k) for e in effects)

            def text(fmt, b, k, part):
                if part > 1:
                    raise Unsupported("base value text")
                v = value(k, b, False)
                return format_stat(fmt, b, v, v, v, active(k))[part]
            seen.add(bool(eval_tree(tree, value, text, lambda k: 1 if active(k) else 0)))
            if len(seen) > 1:
                raise NotConstant()
        return seen.pop()

    # --- entry
    def run(self):
        func, owner = self.world.function(self.item_cls, "GetGearStatEntry")
        if not func:
            raise Unsupported("no GetGearStatEntry")
        objs = list(self.world.cdos.values()) + list(self.world.objs.values())
        self.saved_vars = {id(o): (o, copy_value(o.vars)) for o in objs}
        self.runs = 0
        self.rows = self.run_assuming(func, owner, [])
        return self.rows

    def run_assuming(self, func, owner, assumed):
        """GetGearStatEntry with these conditions taken as known. A branch on an upgrade in a function it calls
        (WPN_Crossbow GetEquippedSpecialArrow) cannot fork there: the whole run is repeated with the condition
        true and false, and the two row lists are merged like a fork."""
        self.runs += 1
        if self.runs > 32:
            raise Unsupported("too many upgrade branches in called functions")
        for o, v in self.saved_vars.values():
            o.vars = copy_value(v)
        self.assumed, self.rows, self.stats_list, self.forks = list(assumed), [], [], 0
        try:
            self.call(func, owner, self.world.cdo(self.item_cls), [None, self.stats_list], [], top=True)
            return self.rows
        except NeedAssume as e:
            c = e.cond
        ways = ([c], c.inverse())
        rows = [self.run_assuming(func, owner, assumed + list(w)) for w in ways]
        return merge_rows(rows[0], rows[1], [x.text() for x in ways[0]], [x.text() for x in ways[1]])

    # --- function calls
    def call(self, func, owner, this, args, conds, top=False):
        """Run a Blueprint function. args: values per parameter (outs may be Refs). Returns the return value."""
        frame = Frame(func, owner, this, conds, top)
        params = [p for p in func.params if "return" not in p[3]]
        for p, a in zip(params, args):
            frame.locals[p[0]] = a.get() if isinstance(a, Ref) else a
        for p in func.props:
            if p and p[0] not in frame.locals:
                frame.locals[p[0]] = default_of(p[1])
        self.execute(frame)
        for p, a in zip(params, args):
            if "out" in p[3] and isinstance(a, Ref):
                a.put(frame.locals.get(p[0]))
        ret = [p for p in func.params if "return" in p[3]]
        return frame.locals.get(ret[0][0]) if ret else None

    def execute(self, frame):
        self.run_from(frame, 0, [], [])

    def run_from(self, frame, pc, stack, depth_conds):
        func = frame.func

        def jump(target):
            if target not in func.at:
                raise Unsupported("bad jump %d in %s" % (target, func.name))
            return func.at[target]

        def pop():
            if not stack:
                return None
            # leaving the branch where these assumptions were made
            while depth_conds and depth_conds[-1][0] >= len(stack):
                depth_conds.pop()
                frame.conds.pop()
            return jump(stack.pop())

        while pc is not None and pc < len(func.stmts):
            self.steps += 1
            if self.steps > self.MAX_STEPS:
                raise Unsupported("too many steps")
            node = func.stmts[pc][1]
            pc += 1
            k = node[0]
            if k == "push":
                stack.append(node[1])
            elif k == "pop":
                pc = pop()
            elif k == "popifnot":
                c = self.truth(self.eval(node[1], frame), frame)
                if isinstance(c, CONDS):
                    depth_conds.append((len(stack), c))
                    frame.conds.append(c)
                elif not c:
                    pc = pop()
            elif k == "jump":
                pc = jump(node[1])
            elif k == "jumpifnot":
                c = self.truth(self.eval(node[2], frame), frame)
                if isinstance(c, CONDS):
                    if not frame.top:
                        if isinstance(c, (Cond, AnyCond)):
                            raise NeedAssume(c)
                        raise Unsupported("branch on an upgrade in %s" % func.name)
                    self.fork(frame, pc, jump(node[1]), stack, depth_conds, c)
                    return
                if not c:
                    pc = jump(node[1])
            elif k == "computedjump":
                pc = jump(concrete(self.eval(node[1], frame)))
            elif k in ("return", "end"):
                return
            elif k in ("nop", "endparm"):
                pass
            else:
                self.eval(node, frame)

    def fork(self, frame, pc_then, pc_else, stack, depth_conds, c):
        """If/else on an upgrade in GetGearStatEntry: run both ways, then merge the two row lists."""
        self.forks += 1
        if self.forks > 64:
            raise Unsupported("too many upgrade branches")
        saved = (copy_value(frame.locals), copy_value(frame.this.vars), list(stack), list(depth_conds),
                 list(frame.conds), len(self.rows))
        results = []
        ways = ((pc_then, [c]), (pc_else, c.inverse()))
        for pc, assumed in ways:
            locals_, vars_, st, dc, conds, n = saved
            frame.locals, frame.this.vars = copy_value(locals_), copy_value(vars_)
            frame.conds = list(conds)
            del self.rows[n:]
            st, dc = list(st), list(dc)
            for a in assumed:
                dc.append((len(st), a))
                frame.conds.append(a)
            self.run_from(frame, pc, st, dc)
            results.append(self.rows[n:])
        del self.rows[saved[5]:]
        self.rows.extend(merge_rows(results[0], results[1], [x.text() for x in ways[0][1]],
                                    [x.text() for x in ways[1][1]]))

    def truth(self, c, frame):
        """Concrete bool, or a condition still to assume. Conditions assumed on this path are known."""
        conds = frame.conds + self.assumed
        known = {x.key: not x.negate for x in conds if isinstance(x, Cond)}
        if isinstance(c, Cond):
            if c.key in known:
                return known[c.key] != c.negate
            return c
        if isinstance(c, AnyCond):
            if any(known.get(k) is True for k in c.keys):
                return True
            if any(isinstance(x, AnyCond) and set(x.keys) <= set(c.keys) for x in conds):
                return True
            keys = [k for k in c.keys if known.get(k) is not False]
            if not keys:
                return False
            return Cond(keys[0]) if len(keys) == 1 else AnyCond(keys)
        if isinstance(c, Pred):
            try:
                return c.interp.decide(c.tree)
            except NotConstant:
                if not frame.top:
                    raise
                for x in frame.conds:  # assumed on this path already
                    if isinstance(x, PredCond) and x.tree == c.tree:
                        return True
                    if isinstance(x, PredCond) and x.tree == ["not", c.tree]:
                        return False
                return PredCond(c.tree)
        return bool(concrete(c, "branch"))

    # --- expressions
    def place(self, node, frame):
        k = node[0]
        if k == "local":
            name = node[1]
            return Ref(lambda: frame.locals.get(name), lambda v: frame.locals.__setitem__(name, copy_value(v)))
        if k in ("inst", "default"):
            this = frame.this
            return Ref(lambda: self.var(this, node[1]), lambda v: this.vars.__setitem__(node[1], copy_value(v)))
        if k == "member":
            outer = self.place(node[1], frame)
            name = node[2]

            def get():
                s = outer.get()
                return s.get(name) if isinstance(s, dict) else self.member(s, name)

            def put(v):
                s = outer.get()
                if not isinstance(s, dict):
                    raise Unsupported("member write on " + type(s).__name__)
                s[name] = copy_value(v)
            return Ref(get, put)
        if k == "ctx":
            target = concrete(self.eval(node[1], frame), "context")
            obj = self.as_obj(target)
            inner = Frame(frame.func, frame.owner, obj, frame.conds)
            inner.locals = frame.locals
            return self.place(node[2], inner)
        raise Unsupported("not assignable: " + k)

    def var(self, this, name):
        if not isinstance(this, Obj):
            raise Unsupported("variable %s of %r" % (name, this))
        try:
            return self.plain(this.get(name))
        except Unsupported:
            if name in ("Name", "Description"):  # unset FText of an upgrade asset
                return ""
            raise

    def plain(self, v):
        """Property value -> interpreter value (FText -> string, struct -> dict)."""
        if isinstance(v, dict) and "kind" in v and len(v) <= 4:
            return self.world.text(v)
        if isinstance(v, list) and v and isinstance(v[0], tuple) and len(v[0]) == 2 and isinstance(v[0][0], str):
            return {k: self.plain(x) for k, x in v}
        return v

    def member(self, s, name):
        if isinstance(s, list) and s and isinstance(s[0], tuple):
            return self.plain(dict(s).get(name))
        if isinstance(s, UV):
            if name == "PreviewStatus":
                return Status(s.key, None, s.base)
            if name == "BaseValue" and s.base is not None:
                return s.base
            # CurrentValue: the shown value (FormatValues prints it). UpgradedValue / OldUpgradedValue: what the
            # upgrades add to the base, in the shown / compared build (WPN_Crossbow "Get Reload Stat" adds
            # BaseValue to it). UpgradedPercentageValue: product of the percentage upgrades (FormatStatic: 1).
            if s.base is not None and s.key is None:
                return {"CurrentValue": s.base, "UpgradedValue": 0.0, "OldUpgradedValue": 0.0,
                        "UpgradedPercentageValue": 1.0, "OldUpgradedPercentageValue": 1.0}.get(name, s.base)
            if s.base is not None and name == "CurrentValue":
                return Expr(["val", s.key, s.base])
            if s.base is not None and name == "UpgradedValue":
                return Expr(["-", ["val", s.key, s.base], s.base])
            if s.base is not None and name == "OldUpgradedValue":
                return Expr(["-", ["old", s.key, s.base], s.base])
            if name == "UpgradedPercentageValue":
                return Expr(["pct", s.key, False])
            if name == "OldUpgradedPercentageValue":
                return Expr(["pct", s.key, True])
            raise Unsupported("upgrade value member " + name)
        raise Unsupported("member %s of %s" % (name, type(s).__name__))

    def as_obj(self, v):
        if isinstance(v, Obj):
            return v
        if is_ref(v):
            return self.world.obj(v)
        if isinstance(v, str) and v.startswith("/Script/"):
            nat = object_name(v).replace("Default__", "")
            return Obj(v, "/Script/" + nat, self.world.native.defaults(nat), nat)
        raise Unsupported("not an object: %r" % (v,))

    def eval(self, node, frame):
        k = node[0]
        if k == "const":
            return node[1]
        if k == "text":
            return self.world.text(node[1])
        if k == "obj":
            return node[1]
        if k == "self":
            return frame.this
        if k in ("local", "inst", "default", "member"):
            return self.place(node, frame).get()
        if k == "let":
            self.place(node[1], frame).put(self.eval(node[2], frame))
            return None
        if k == "ctx":
            target = concrete(self.eval(node[1], frame), "context")
            if target is None:
                raise Unsupported("call on None")
            obj = self.as_obj(target)
            if node[2][0] == "call":  # arguments belong to the caller's frame
                return self.invoke(node[2], frame, obj)
            inner = Frame(frame.func, frame.owner, obj, frame.conds)
            inner.locals = frame.locals
            return self.eval(node[2], inner)
        if k == "call":
            return self.invoke(node, frame)
        if k == "switch":
            idx = self.eval(node[1], frame)
            if isinstance(idx, StatusPair):
                return idx.merged()
            if isinstance(idx, Status):
                idx = self.status_as
            idx = concrete(idx, "select")
            for key, val in node[2]:
                if self.eval(key, frame) == idx:
                    return self.eval(val, frame)
            return self.eval(node[3], frame)
        if k == "struct":
            return {"_struct": object_name(node[1]), "_items": [self.eval(x, frame) for x in node[2]]}
        if k == "array":
            return [self.eval(x, frame) for x in node[1]]
        if k == "setarray":
            self.place(node[1], frame).put([self.eval(x, frame) for x in node[2]])
            return None
        if k in ("cast", "primcast"):
            return self.eval(node[-1], frame)
        if k == "soft":
            return self.eval(node[1], frame)
        raise Unsupported("expression " + k)

    def invoke(self, node, frame, target=None):
        _, kind, func_ref, args = node
        name = func_ref.split(".")[-1].replace("export:", "")
        this = target if target is not None else frame.this
        self.natives.target = this  # the object a native member function is called on (comp.GetDamage())
        if name in Natives.INTERCEPT:
            return self.natives.call(name, args, frame, func_ref)
        target_func, owner = None, None
        if kind in ("virtual", "localvirtual"):
            cls = this.cls if isinstance(this, Obj) else self.item_cls
            target_func, owner = self.world.function(cls, name)
        elif func_ref.startswith("/Game/"):
            target_func, owner = self.world.function_at(func_ref)
        elif func_ref.startswith("export:"):
            target_func, owner = self.world.function(frame.owner, name)
        if not target_func:
            self.natives.target = this
            return self.natives.call(name, args, frame, func_ref)
        params = [p for p in target_func.params if "return" not in p[3]]
        vals = [self.arg(a, p, frame) for a, p in zip(args, params)]
        callee_this = self.world.cdo(owner) if object_name(owner).startswith("BPL_") else this
        return self.call(target_func, owner, callee_this, vals, frame.conds)

    def arg(self, node, param, frame):
        if "out" in param[3] and node[0] in ("local", "inst", "default", "member", "ctx"):
            return self.place(node, frame)
        return self.eval(node, frame)

    def add_row(self, entry, conds):
        self.rows.append((copy_value(entry), list(dict.fromkeys(c.text() for c in conds))))


def row_sig(row):
    entry = row[0]

    def sig(v):
        return repr(v.tree() if isinstance(v, VText) else v.tree if isinstance(v, Expr) else v)
    return repr((sig(entry.get("Text")), sig(entry.get("Value")), sig(entry.get("UpgradeValue")),
                 entry.get("IsPenalty")))


def merge_rows(a, b, a_conds, b_conds):
    """Rows of the 'then' (a) and 'else' (b) runs of a branch. Every row of a holds only when a_conds hold,
    every row of b only when b_conds hold (also after the branch has joined: the rest of the function ran in
    both). Rows that are the same in both, conditions included, lose the branch conditions; the others keep
    them. A row that contradicts its run (an inner branch on the same key after the condition left scope) is
    dropped."""
    import difflib

    def neg(c):
        return c[1:] if c.startswith("!") else "!" + c

    def norm(rows, own):
        out = []
        for entry, conds in rows:
            if any(neg(c) in conds for c in own):
                continue  # impossible in this run
            out.append((entry, [c for c in conds if c not in own]))
        return out

    a, b = norm(a, a_conds), norm(b, b_conds)
    sa = [row_sig(r) + repr(r[1]) for r in a]
    sb = [row_sig(r) + repr(r[1]) for r in b]
    out = []
    for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, sa, sb, autojunk=False).get_opcodes():
        if op == "equal":
            out.extend(a[i1:i2])
        else:
            out.extend((e, c + a_conds) for e, c in a[i1:i2])
            out.extend((e, c + b_conds) for e, c in b[j1:j2])
    return out


def default_of(typ):
    return {"FloatProperty": 0.0, "IntProperty": 0, "BoolProperty": False, "ByteProperty": 0,
            "EnumProperty": 0, "TextProperty": "", "StrProperty": "", "NameProperty": "None",
            "ArrayProperty": [], "StructProperty": {}}.get(typ)


# ---------------------------------------------------------------- natives
class Natives:
    INTERCEPT = {"FormatUpgradeToEntry", "FormatStatic", "Array_Add", "SetGearStatText", "FormatPreviewStatus"}

    def __init__(self, interp):
        self.i = interp
        self.w = interp.world
        self.target = None  # set by Interp.invoke before each call

    def call(self, name, args, frame, ref):
        fn = getattr(self, "n_" + name, None)
        if name in ("GetUpgradedValue", "GetGenericUpgradedValue"):
            return self.upgraded_value(name, ref.split(".")[-2], args, frame)
        if not fn:
            raise Unsupported("native " + name)
        return fn(args, frame)

    def ev(self, args, frame):
        return [self.i.eval(a, frame) for a in args]

    def out(self, node, frame, value):
        self.i.place(node, frame).put(value)

    # --- arrays
    def n_Array_Add(self, args, frame):
        arr = self.i.place(args[0], frame).get()
        item = self.i.eval(args[1], frame)
        if arr is self.i.stats_list:
            self.i.add_row(item, frame.conds)
        arr.append(copy_value(item))
        return len(arr) - 1

    def n_Array_Length(self, args, frame):
        return len(concrete(self.i.eval(args[0], frame)))

    def n_Array_Get(self, args, frame):
        arr, idx = self.ev(args[:2], frame)
        self.out(args[2], frame, copy_value(arr[idx]))

    # --- objects and classes
    def n_GetObjectClass(self, args, frame):
        o = self.i.eval(args[0], frame)
        return o.cls if isinstance(o, Obj) else concrete(o)

    def n_GetComponentFromClass(self, args, frame):
        cls, comp = self.ev(args, frame)
        return self.w.component_of(cls, comp)

    def n_IsValid(self, args, frame):
        return self.i.eval(args[0], frame) is not None

    def n_GetClassDefaultObject(self, args, frame):
        return self.w.cdo(self.i.eval(args[0], frame))

    # Pellet projectiles (Boomstick "Shaped Shells"): the same arguments as DamageUpgrade::GetUpgradedValue
    # (item, projectile class, player, EDamageUpgrade, EDamageComponentType), read from the projectile.
    def n_GetPelletProjectileDamage(self, args, frame):
        return self.upgraded_value("GetUpgradedValue", "DamageUpgrade", args, frame)

    def n_GetPelletProjectileStaggerChance(self, args, frame):
        # (item, projectile class, player): the projectile's primary StaggerChance
        return self.upgraded_value("GetUpgradedValue", "DamageUpgrade", list(args) + [("const", 2), ("const", 0)],
                                   frame)

    def n_GetStatusEffectDuration(self, args, frame):
        return self.w.cdo(self.i.eval(args[0], frame)).get("Duration")

    def n_GetDamage(self, args, frame):
        """UDamageComponent::GetDamage on a component template (Grenade_WallSaw: the saw's Damage)."""
        comp = self.target
        if not (isinstance(comp, Obj) and self.w.is_a(comp.cls, "/Script/FSD.DamageComponent")):
            raise Unsupported("GetDamage on %r" % (comp,))
        return comp.get("Damage")

    def n_GetRadialDamageMultiplier(self, args, frame):
        """USupercellPayloadUpgrade::GetRadialDamageMultiplier(upgrade asset): not in the stubs; read as the
        asset's RadialDamageMultiplier (1.2 on Bonus_SupercellPayloadCloud_PGL)."""
        upg = concrete(self.i.eval(args[0], frame))
        return float(self.w.obj(upg).get("RadialDamageMultiplier"))

    def n_GetSinusoidalDamageValue(self, args, frame):
        """USinusoidalDamageUpgrade::GetSinusoidalDamageValue(item, player, high, out has upgrade), not in the
        stubs. Read as: the item's primary Damage (with its upgrades) plus the equipped sinusoidal upgrade's
        SinusoidalDamageRangeMax (high) or SinusoidalDamageRangeMin (low; the package sets only the max, 0
        assumed). Microwave Sinusoidal Amplifier: 7 - 3 + 12 = 16 and 7 - 3 + 0 = 4 ("dips lower than base")."""
        high = bool(concrete(self.i.eval(args[2], frame)))
        d = self.upgraded_value("GetUpgradedValue", "DamageUpgrade",
                                [args[0], ("const", None), args[1], ("const", 0), ("const", 0)], frame)
        if d.base is None:
            raise Unsupported("sinusoidal damage without a base damage")
        k = "SinusoidalDamageUpgrade." + ("Max" if high else "Min")
        val, old = ["val", d.key, d.base], ["old", d.key, d.base]
        sv, so = ["val", k, 0.0], ["old", k, 0.0]
        self.out(args[3], frame, Cond(k))
        return {"BaseValue": d.base, "CurrentValue": Expr(["+", val, sv]),
                "UpgradedValue": Expr(["+", ["-", val, d.base], sv]),
                "OldUpgradedValue": Expr(["+", ["-", old, d.base], so]),
                "UpgradedPercentageValue": Expr(["pct", d.key, False]),
                "OldUpgradedPercentageValue": Expr(["pct", d.key, True]),
                "PreviewStatus": Status(k, None, None, ["+", ["-", val, old], ["-", sv, so]])}

    def n_GetUpgradedStatValue(self, args, frame):
        """UArmorStatUpgrade::GetUpgradedStatValue(player, armor class, pawn stat). Shield and Health would be
        the owner character's PlayerHealthComponent (MaxArmor 25, MaxHealth 110), resistances the stat's
        StartingValue (0). Not supported: the Carry Capacity base (PST_CarryingCapacity is an additive stat
        starting at 0; the resource cap it adds to is in neither the packages nor the stubs) and the
        Regeneration Rate row (ArmorUpgrade.RegenRate has no field: the regen is the UPlayerArmorRegenCurve)."""
        raise Unsupported("native GetUpgradedStatValue: base of Carry Capacity (PST_CarryingCapacity) and of "
                          "ArmorUpgrade.RegenRate are not in the packages or the stubs")

    def n_GetManualDetonationValues(self, args, frame):
        """UManualDetonationUpgrade (not in the stubs): the asset's DamagePerStage, TimeBetweenStages and
        RadiusPerStage, as out parameters (Bonus_ManualDetonationUpgrade_PGL: 40, 1, 50)."""
        upg = self.w.obj(concrete(self.i.eval(args[0], frame)))
        for node, prop in zip(args[1:4], ("DamagePerStage", "TimeBetweenStages", "RadiusPerStage")):
            self.out(node, frame, float(upg.get(prop)))

    def n_GetIsItemUpgradeEquipped(self, args, frame):
        """(player, item class, upgrade, character): whether that upgrade is equipped, a condition on the equipped
        build only ("^"; a hovered upgrade does not count)."""
        upg = concrete(self.i.eval(args[2], frame))
        if not is_ref(upg):
            raise Unsupported("GetIsItemUpgradeEquipped of %r" % (upg,))
        return Cond("^a:" + self.w.store.name(upg))

    def n_RoundToDecimalPlace(self, args, frame):
        v, places = self.ev(args, frame)
        scale = 10 ** concrete(places)
        v = self._num(v)
        if isinstance(v, (Expr, UV)):
            return Expr(["/", ["round", ["*", tree_of(v), scale]], scale])
        return f32(int(math.floor(f32(v * scale) + 0.5)) / scale)

    def n_FormatPreviewStatus(self, args, frame):
        """FormatPreviewStatus(format value, upgrade values, preview format): kept symbolic like the PreviewStatus
        FormatUpgradeToEntry makes for an upgrade value (the delta colour comes from that value's direction)."""
        _, values, pfmt = self.ev(args[:3], frame)
        st = values.get("PreviewStatus") if isinstance(values, dict) else None
        if not isinstance(st, Status):
            raise Unsupported("FormatPreviewStatus of %r" % (values,))
        return Status(st.key, concrete(pfmt), st.base, st.delta)

    # --- math and logic: numbers, or expressions over upgrade values (see EXPR_OPS)
    def _num(self, v):
        if isinstance(v, (Expr, UV)):
            return v
        if isinstance(v, VText):
            raise Unsupported("math on a text")
        return concrete(v, "math")

    def _num2(self, args, frame, op, sym=None, integer=False):
        a, b = [self._num(x) for x in self.ev(args, frame)]
        if isinstance(a, (Expr, UV)) or isinstance(b, (Expr, UV)):
            if not sym:
                raise Unsupported("symbolic value used in math")
            t = [sym, tree_of(a), tree_of(b)]
            return Expr(["trunc", t] if integer and sym == "/" else t)
        return op(a, b)

    def _cmp(self, args, frame, op):
        """Comparison; with upgrade values a Pred (decided over every build of the item when used)."""
        a, b = [self._num(x) for x in self.ev(args, frame)]
        if isinstance(a, (Expr, UV)) or isinstance(b, (Expr, UV)):
            return Pred(self.i, [op, tree_of(a), tree_of(b)])
        return CMP[op](a, b)

    def n_Add_FloatFloat(self, a, f): return self._num2(a, f, lambda x, y: x + y, "+")
    def n_Subtract_FloatFloat(self, a, f): return self._num2(a, f, lambda x, y: x - y, "-")
    def n_Multiply_FloatFloat(self, a, f): return self._num2(a, f, lambda x, y: x * y, "*")
    def n_Divide_FloatFloat(self, a, f): return self._num2(a, f, lambda x, y: x / y if y else 0.0, "/")
    def n_Add_IntInt(self, a, f): return self._num2(a, f, lambda x, y: x + y, "+")
    def n_Subtract_IntInt(self, a, f): return self._num2(a, f, lambda x, y: x - y, "-")
    def n_Multiply_IntInt(self, a, f): return self._num2(a, f, lambda x, y: x * y, "*")
    n_Add_Int64Int64, n_Subtract_Int64Int64, n_Multiply_Int64Int64 = n_Add_IntInt, n_Subtract_IntInt, n_Multiply_IntInt
    def n_Multiply_IntFloat(self, a, f): return self._num2(a, f, lambda x, y: x * y, "*")
    def n_Less_FloatFloat(self, a, f): return self._cmp(a, f, "<")
    def n_Greater_FloatFloat(self, a, f): return self._cmp(a, f, ">")
    def n_LessEqual_FloatFloat(self, a, f): return self._cmp(a, f, "<=")
    def n_GreaterEqual_FloatFloat(self, a, f): return self._cmp(a, f, ">=")
    def n_EqualEqual_FloatFloat(self, a, f): return self._cmp(a, f, "==")
    def n_NotEqual_FloatFloat(self, a, f): return self._cmp(a, f, "!=")
    n_Less_IntInt, n_Greater_IntInt = n_Less_FloatFloat, n_Greater_FloatFloat
    n_LessEqual_IntInt, n_GreaterEqual_IntInt = n_LessEqual_FloatFloat, n_GreaterEqual_FloatFloat
    n_EqualEqual_IntInt, n_NotEqual_IntInt = n_EqualEqual_FloatFloat, n_NotEqual_FloatFloat
    def _bool2(self, args, frame, is_and):
        """AND / OR where a side may be an upgrade condition: a concrete side decides or drops out."""
        a, b = self.ev(args, frame)
        if isinstance(a, Pred) or isinstance(b, Pred):
            if isinstance(a, CONDS) or isinstance(b, CONDS):
                raise Unsupported("upgrade condition with a value comparison")
            return Pred.combine(self.i, a, b, is_and)
        if is_and and {type(a), type(b)} == {Cond, StatusIs}:
            # PreviewStatus != 0 AND != 3 (UPGDESC_PlatformGun "Should show text"): set in the shown build
            c, s = (a, b) if isinstance(a, Cond) else (b, a)
            if c.key == s.key and not c.negate and c.key[:1] not in "=^" and s.value == 3 and not s.equal:
                return Cond("=" + c.key)
        sides = []
        for v in (a, b):
            if isinstance(v, CONDS):
                sides.append(v)
            elif bool(concrete(v, "logic")) != is_and:
                return not is_and  # False AND x, True OR x
        if not sides:
            return is_and
        if len(sides) == 1:
            return sides[0]
        if is_and:
            raise Unsupported("AND of two upgrade conditions")
        if not all(isinstance(s, AnyCond) or not s.negate for s in sides):
            raise Unsupported("OR of a missing upgrade")
        return AnyCond([k for s in sides for k in (s.keys if isinstance(s, AnyCond) else [s.key])])

    def n_BooleanAND(self, a, f): return self._bool2(a, f, True)
    def n_BooleanOR(self, a, f): return self._bool2(a, f, False)

    def n_Not_PreBool(self, args, frame):
        v = self.i.eval(args[0], frame)
        if isinstance(v, Cond):
            return Cond(v.key, not v.negate)
        if isinstance(v, AnyCond):
            raise Unsupported("negated OR of upgrade conditions")
        if isinstance(v, Pred):
            return v.negated()
        if isinstance(v, StatusIs):
            return StatusIs(v.key, v.value, not v.equal, v.assumed)
        return not concrete(v, "logic")

    def _bytecmp(self, args, frame, equal):
        a, b = self.ev(args, frame)
        if isinstance(a, Status) or isinstance(b, Status):
            st, other = (a, b) if isinstance(a, Status) else (b, a)
            other = concrete(other)
            if other == 0:
                return Cond(st.key, negate=equal)
            return StatusIs(st.key, other, equal, self.i.status_as)
        a, b = concrete(a), concrete(b)
        return (self.w.native.enum_index(a) == self.w.native.enum_index(b)) == equal

    def n_Less_ByteByte(self, args, frame):
        a, b = self.ev(args, frame)
        if isinstance(a, Status) and isinstance(b, Status):
            return StatusPair(a, b)
        a, b = concrete(a), concrete(b)
        return self.w.native.enum_index(a) < self.w.native.enum_index(b)

    def n_EqualEqual_ByteByte(self, a, f): return self._bytecmp(a, f, True)
    def n_NotEqual_ByteByte(self, a, f): return self._bytecmp(a, f, False)

    def n_FClamp(self, args, frame):
        v, lo, hi = [concrete(x) for x in self.ev(args, frame)]
        return max(lo, min(hi, v))

    def n_MakeLiteralInt(self, args, frame): return self.i.eval(args[0], frame)
    def n_MakeLiteralFloat(self, args, frame): return self.i.eval(args[0], frame)
    def n_MakeLiteralText(self, args, frame): return self.i.eval(args[0], frame)
    def n_MakeLiteralString(self, args, frame): return self.i.eval(args[0], frame)

    def n_Conv_IntToFloat(self, args, frame):
        v = self._num(self.i.eval(args[0], frame))
        return v if isinstance(v, (Expr, UV)) else float(v)

    n_Conv_Int64ToFloat = n_Conv_IntToFloat

    def n_SelectFloat(self, args, frame):
        a, b, pick = self.ev(args, frame)
        return concrete(a) if concrete(pick) else concrete(b)

    def _round(self, args, frame, op, fn):
        v = self._num(self.i.eval(args[0], frame))
        return Expr([op, tree_of(v)]) if isinstance(v, (Expr, UV)) else fn(v)

    def n_Round(self, a, f): return self._round(a, f, "round", lambda x: int(math.floor(x + 0.5)))
    n_Round64 = n_Round
    def n_FFloor(self, a, f): return self._round(a, f, "floor", lambda x: int(math.floor(x)))
    n_FFloor64 = n_FFloor
    def n_FTrunc(self, a, f): return self._round(a, f, "trunc", int)
    n_FTrunc64 = n_FTrunc

    # --- text: plain strings, or expressions (a formatted row value is a VText)
    def n_GetUpgradeName(self, args, frame):
        upg = concrete(self.i.eval(args[0], frame))
        return self.w.text(self.w.obj(upg).props.get("Name")) or "" if upg else ""

    def _text(self, v):
        if isinstance(v, (Expr, VText)):
            return v
        return concrete(v, "text")

    def n_Conv_TextToString(self, args, frame):
        return self._text(self.i.eval(args[0], frame))

    n_Conv_StringToText = n_Conv_TextToString
    n_Conv_NameToText = n_Conv_TextToString
    n_Conv_StringToName = n_Conv_TextToString

    def n_Conv_StringToFloat(self, args, frame):
        v = self._text(self.i.eval(args[0], frame))
        return Expr(["num", tree_of(v)]) if isinstance(v, (Expr, VText)) else f32(leading_number(v))

    def n_Conv_StringToInt(self, args, frame):
        v = self._text(self.i.eval(args[0], frame))
        return Expr(["inum", tree_of(v)]) if isinstance(v, (Expr, VText)) else leading_number(v, True)

    def n_Concat_StrStr(self, args, frame):
        a, b = [self._text(x) for x in self.ev(args, frame)]
        if isinstance(a, str) and isinstance(b, str):
            return a + b
        return Expr(["cat", tree_of(a), tree_of(b)])

    def n_TextIsEmpty(self, args, frame):
        return not concrete(self.i.eval(args[0], frame))

    def n_Format(self, args, frame):
        pattern, items = self.ev(args, frame)
        parts = [concrete(pattern)]
        for it in concrete(items):
            if not (isinstance(it, dict) and "_items" not in it):
                raise Unsupported("format argument")
            val = self.format_arg(it)
            hole = "{%s}" % it.get("ArgumentName")
            out = []
            for p in parts:
                if not isinstance(p, str) or hole not in p:
                    out.append(p)
                    continue
                pieces = p.split(hole)
                for n, piece in enumerate(pieces):
                    if n:
                        out.append(val)
                    out.append(piece)
            parts = out
        if all(isinstance(p, str) for p in parts):
            return "".join(parts).strip()
        parts = [p for p in parts if p != ""]
        if isinstance(parts[0], str):
            parts[0] = parts[0].lstrip()
        if isinstance(parts[-1], str):
            parts[-1] = parts[-1].rstrip()
        return Expr(["cat"] + [tree_of(p) for p in parts])

    def format_arg(self, it):
        """Text of a FFormatArgumentData: EFormatArgumentType 0 Int, 1 UInt, 2 Float, 3 Double, 4 Text, 5 Gender.
        Numbers like FText::AsNumber with the default options (grouping, up to 3 fraction digits)."""
        typ = concrete(it.get("ArgumentValueType", 4))
        if typ in (0, 1, 2, 3):
            v = self._num(it.get("ArgumentValueInt" if typ < 2 else "ArgumentValueFloat") or 0)
            maxf = 0 if typ < 2 else 3
            if isinstance(v, (Expr, UV)):
                return Expr(["ftext", tree_of(v), False, True, 0, maxf])
            return format_number(v, False, True, 0, maxf)
        if typ != 4:
            raise Unsupported("format argument type %r" % typ)
        return self._text(it.get("ArgumentValue") or "")

    def n_Conv_FloatToText(self, args, frame):
        v, mode, sign, group, mini, maxi, minf, maxf = self.ev(args, frame)
        v = self._num(v)
        if isinstance(v, (Expr, UV)):
            return Expr(["ftext", tree_of(v), bool(concrete(sign)), bool(concrete(group)), concrete(minf),
                         concrete(maxf)])
        return format_number(v, concrete(sign), concrete(group), concrete(minf), concrete(maxf))

    def n_Conv_IntToText(self, args, frame):
        v, sign, group, mini, maxi = self.ev(args, frame)
        v = self._num(v)
        if isinstance(v, (Expr, UV)):
            return Expr(["itext", tree_of(v), bool(concrete(sign)), bool(concrete(group))])
        return format_number(v, concrete(sign), concrete(group), 0, 0)

    n_Conv_Int64ToText = n_Conv_IntToText

    def n_Conv_FloatToString(self, args, frame):
        v = self._num(self.i.eval(args[0], frame))
        return Expr(["fstr", tree_of(v)]) if isinstance(v, (Expr, UV)) else sanitize_float(v)

    def n_Conv_IntToString(self, args, frame):
        v = self._num(self.i.eval(args[0], frame))
        return Expr(["itext", tree_of(v), False, False]) if isinstance(v, (Expr, UV)) else str(int(v))

    # --- gear stat helpers
    def n_SetGearStatText(self, args, frame):
        ref = self.i.place(args[0], frame)
        entry = ref.get()
        entry["Text"] = self.i.eval(args[1], frame)

    def n_FormatUpgradeToEntry(self, args, frame):
        text, uv, fmt, pfmt = self.ev(args[:4], frame)
        if isinstance(uv, dict) and isinstance(uv.get("PreviewStatus"), Status):
            # FUpgradeValues the Blueprint made from scaled members (UPGDESC_DPS: value / interval): run the
            # helper's own Blueprint, its maths stays symbolic (expression rows)
            func, owner = self.w.function_at(HELPERS + ".BPL_UpgradeHelpers_C.FormatUpgradeToEntry")
            params = [p for p in func.params if "return" not in p[3]]
            vals = [self.i.arg(a, p, frame) for a, p in zip(args, params)]
            return self.i.call(func, owner, self.w.cdo(owner), vals, frame.conds)
        if not isinstance(uv, UV):
            raise Unsupported("FormatUpgradeToEntry without upgrade value")
        fmt, pfmt = concrete(fmt), concrete(pfmt)
        entry = {"Text": self._text(text), "Value": VText(uv, fmt, pfmt), "UpgradeValue": VText(uv, fmt, pfmt, 1),
                 "IsPenalty": False, "PreviewStatus": Status(uv.key, pfmt, uv.base)}
        self.out(args[5], frame, entry)
        self.out(args[6], frame, Cond(uv.key))

    def n_FormatStatic(self, args, frame):
        text, value, fmt = self.ev(args[:3], frame)
        uv = UV(None, concrete(value, "static value"))
        entry = {"Text": self._text(text), "Value": VText(uv, concrete(fmt), 2),
                 "UpgradeValue": VText(uv, concrete(fmt), 2, 1), "IsPenalty": False, "PreviewStatus": 0}
        self.out(args[4], frame, entry)

    # --- upgrade values
    def upgraded_value(self, name, cls, args, frame):
        """Native <UpgradeClass>::GetUpgradedValue / UItemUpgrade::GetGenericUpgradedValue."""
        vals = self.ev(args, frame)
        if name == "GetGenericUpgradedValue":
            item, _, upg = vals
            upg = concrete(upg)
            return UV("a:" + (self.w.store.name(upg) if is_ref(upg) else "None"), 0)
        sig = self.w.native.sigs.get(cls)
        if not sig or len(sig) != len(vals):
            raise Unsupported("signature of %s.GetUpgradedValue" % cls)
        item = sub = enum = dct = param = None
        comp_index = 0
        for (typ, pname), v in zip(sig, vals):
            if typ.startswith("TSubclassOf") and "StatusEffect" not in typ and "Component" not in typ:
                if item is None:
                    item = v
                else:
                    sub = v
            elif typ == "EDamageComponentType":
                dct = concrete(v)
            elif typ.startswith("E"):
                names = self.w.native.enums.get(typ)
                v = concrete(v)
                if not names or not isinstance(v, int) or v >= len(names) or names[v] is None:
                    raise Unsupported("%s value %r" % (typ, v))
                enum = names[v]
                if param is None:
                    param = "%s::%s" % (typ, enum)  # matched against the asset's enum property (BONUS)
            elif typ in ("UDamageClass*", "TSubclassOf<UStatusEffect>", "FName"):
                param = concrete(v)
            elif typ == "int32":
                comp_index = concrete(v)  # CapacityUpgrade: which capacity component (C++ creation order)
        if cls in BONUS:
            prop, field = BONUS[cls]
            if param is None and prop:
                param = sub  # a projectile class argument (CryoSprayProjectileUpgrade)
            base = 0.0
            if cls in BONUS_BASE and prop:
                bonus_cls, include = BONUS_BASE[cls]
                bprop = "StatusEffects" if bonus_cls == "StatusDamageBonus" else prop
                found, count = self.w.damage_bonus(concrete(item), bonus_cls, bprop, param, field)
                if count and not include:
                    raise Unsupported("%s on %s: the item has its own %s" % (cls, object_name(str(param)),
                                                                         bonus_cls))
                base = found if include else 0.0
            return UV(bonus_key(cls, param if prop else None), base)
        if enum is None and None not in BASES.get(cls, {}):
            raise Unsupported("%s.GetUpgradedValue without an upgrade type" % cls)
        key = upgrade_key(cls, enum, dct, sub)
        if is_ref(sub):
            SUB_ANCESTORS[object_name(sub)] = {object_name(c) for c in self.w.class_chain(sub)[1:]}
            if object_name(sub) in {object_name(c) for c in self.w.class_chain(self.i.item_cls)}:
                OWN_SUBS.add(object_name(sub))  # the item itself passed as the sub item (WPN_CoilGun)
        if cls in PARAM_KEYS:
            key += "~" + (object_name(param) if is_ref(param) else "None")
        spec = BASES.get(cls, {}).get(enum)
        item = concrete(item)
        if not spec:
            # same-named property of the item (or sub item) class, else only fine while the row cannot show
            o = self.w.cdo(sub or item)
            v = next((x for k, x in o.props.items() if k.lower() == (enum or "").lower()), None)
            if isinstance(v, (int, float)) and not isinstance(v, bool):
                self.i.guessed.add(key)
                return UV(key, v)
            return UV(key, None)
        base = self.base_of(spec, item, sub, dct, comp_index)
        if base is None:
            return UV(key, None)
        if isinstance(base, bool):
            base = int(base)
        if not isinstance(base, (int, float)):
            raise Unsupported("base %s.%s = %r" % (cls, enum, base))
        return UV(key, base)

    def base_of(self, spec, item, sub=None, dct=None, comp_index=0):
        """Base value of a BASES spec on an item class (sub: the sub item class argument, if any)."""
        if spec[0] == "mult":
            return 1.0
        if spec[0] == "const":
            return spec[1]
        if spec[0] == "item":
            return self.member_path(self.w.cdo(item), spec[1])
        if spec[0] in ("plus", "times"):
            v = self.base_of(spec[2], item, sub, dct, comp_index)
            if v is None:
                return None
            return float(v) + spec[1] if spec[0] == "plus" else float(v) * spec[1]
        if spec[0] == "sub":
            if sub is None and not any(k.lower() == spec[1].lower() for k in self.w.cdo(item).props):
                # no sub item given (WPN_Crossbow without a special bolt): the spec's default, else only fine
                # while the row cannot show
                return spec[2] if len(spec) > 2 else None
            return self.w.cdo(sub or item).get(spec[1])
        if spec[0] == "via":  # a class held by an item property (the det pack charge, the sentry)
            return self.base_of(spec[2], concrete(self.w.cdo(item).get(spec[1])), None, dct)
        if spec[0] == "launcher":  # the projectile class of the item's projectile launcher component
            comp = self.w.component_of(item, "/Script/FSD.ProjectileLauncherBaseComponent")
            if not comp:
                raise Unsupported("no projectile launcher on %s" % object_name(item))
            return self.base_of(spec[1], concrete(comp.get("ProjectileClass")), None, dct)
        if spec[0] == "owner":  # the character class of the item's owner (ItemID.ItemOwner.Character)
            owner = self.w.obj(self.w.cdo(item).get("ItemID")).get("ItemOwner")
            return self.base_of(spec[1], self.w.obj(owner).get("Character"), None, dct)
        if spec[0] == "damage":
            comp = self.w.damage_component(sub or item, dct or 0)
            if len(spec) > 2:  # a default for a field the stubs do not know
                return next((v for k, v in comp.props.items() if k.lower() == spec[1].lower()), spec[2])
            return comp.get(spec[1])
        if spec[0] == "named":
            comp = self.w.components(sub or item).get(spec[1])
            if not comp:
                raise Unsupported("no component %s on %s" % (spec[1], object_name(sub or item)))
            return comp.get(spec[2])
        if spec[0] == "comp":
            if comp_index or spec[1] == "/Script/FSD.CapacityHoldingItemAggregator":  # by index, C++ order
                comps = self.w.components_of(sub or item, spec[1])
                comp = comps[comp_index] if comp_index < len(comps) else None
            else:
                comp = self.w.component_of(sub or item, spec[1])
            if not comp:
                raise Unsupported("no %s %d on %s" % (object_name(spec[1]), comp_index, object_name(item)))
            return comp.get(spec[2])
        if spec[0] == "dynste":
            stes = self.w.damage_bonus_objects(sub or item, "PushDynamicStatusEffectDamageBonus")
            if len(stes) != 1 or not stes[0].props.get("BaseSTE"):
                raise Unsupported("%d dynamic status effects on %s" % (len(stes), object_name(sub or item)))
            return self.w.cdo(stes[0].props["BaseSTE"]).get(spec[1])
        raise Unsupported("base spec %r" % (spec,))

    def member_path(self, obj, path):
        """'TrailSettings.LifeTime': a struct member of a property; a member the package leaves out has the
        C++ default of its struct (the struct type is the first part after ':', 'TrailSettings:ContrailSettings')."""
        prop, _, rest = path.partition(".")
        prop, _, struct = prop.partition(":")
        v = obj.get(prop)
        if not rest:
            return v
        members = dict(v) if isinstance(v, list) else {}
        if rest in members:
            return members[rest]
        defaults = self.w.native.ctor.get("F" + struct, {})
        if rest not in defaults:
            raise Unsupported("no member %s of %s" % (rest, path))
        return defaults[rest]


def bonus_key(cls, param):
    if param is None:
        return cls
    return "%s@%s" % (cls, object_name(param) if is_ref(param) else param)


def upgrade_key(cls, enum, dct=None, sub=None):
    """'DamageUpgrade.Damage.0@PRJ_X_C': upgrade class, type, damage component type, required sub item."""
    key = "%s.%s" % (cls, enum) if enum else cls
    if cls == "DamageUpgrade":
        key += ".%d" % (dct or 0)
    if is_ref(sub):
        key += "@" + object_name(sub)
    return key


# Required sub item classes of row keys -> names of their parent classes (filled while the rows are built).
# An upgrade whose RequiredClass is a parent applies to the child too (GooCannon: PRJ_BaseGooProjectile_C).
SUB_ANCESTORS = {}
# Sub item classes of row keys that are the item itself or a parent of it.
OWN_SUBS = set()


def key_matches(effect_key, row_key):
    """An upgrade without a required sub item applies to every sub item of its type, one for a parent class
    to its child classes. "key@" (RequiredClass is the item itself, see Stats.item_effects) applies to rows
    read without a sub item or with the item as the sub item, not to rows of other sub items."""
    if effect_key == row_key:
        return True
    base, _, sub = row_key.partition("@")
    if effect_key == base:
        return True
    ebase, _, esub = effect_key.partition("@")
    if effect_key.endswith("@"):
        return ebase == base and (not sub or sub in OWN_SUBS)
    return bool(sub and esub) and ebase == base and esub in SUB_ANCESTORS.get(sub, ())


# ---------------------------------------------------------------- number text like FText::AsNumber (en)
def format_number(v, sign=False, grouping=True, minf=0, maxf=0):
    """Same as formatNumber in Web/public/js/stats.js: half to even on the last digit, fraction scaled apart."""
    v = float(v)
    neg, a = v < 0, abs(v)
    whole, scale = int(a), 10 ** maxf
    scaled = (a - whole) * scale
    frac = int(scaled // 1)
    rest = scaled - frac
    if rest > 0.5 or (rest == 0.5 and (whole * scale + frac) % 2 == 1):
        frac += 1
    if frac >= scale:
        whole, frac = whole + 1, frac - scale
    ftext = str(frac).rjust(maxf, "0").rstrip("0") if maxf else ""
    while len(ftext) < minf:
        ftext += "0"
    itext = "{:,}".format(whole) if grouping else str(whole)
    zero = whole == 0 and frac == 0
    return ("-" if neg and not zero else "+" if sign else "") + itext + ("." + ftext if ftext else "")


def format_stat(fmt, base, cur, up, old, active=False):
    """FormatValues of BPL_UpgradeHelpers: [value text, delta text]. Same as formatStat in stats.js."""
    def fn(v, maxf=0, sign=False, grouping=True):
        return format_number(v, sign, grouping, 0, maxf)

    def div(a, b):
        return 0.0 if b == 0 else f32(a / b)

    def rint(x):
        return int(math.floor(x + 0.5))

    def dec1(x):
        return f32(rint(f32(x * 10)) / 10)

    def pct(up_, old_, cur_, f):
        v = min(1000, max(0, f32(cur_ - 1))) if f in (3, 12) else cur_
        return [fn(f32(v * 100), 0, f != 8) + "%", fn(f32(f32(up_ - old_) * 100), 0, True) + "%"]

    if fmt == 0:
        return [fn(cur), fn(rint(up) - rint(old), 0, True)]
    if fmt == 1:
        return [fn(cur, 1), fn(f32(dec1(up) - dec1(old)), 1, True)]
    if fmt in (2, 3, 8):
        return pct(up, old, cur, fmt)
    if fmt == 4:
        return [fn(f32(cur / 100), 1), fn(f32(f32(up / 100) - f32(old / 100)), 1, True)]
    if fmt == 7:
        return ["+1" if active else "0", ""]
    if fmt in (9, 10):
        d = div(1, base)
        return pct(f32(up * d), f32(old * d), f32(d * cur), 2 if fmt == 9 else 8)
    if fmt == 11:
        d = div(1, base)
        change = f32(div(1, f32(f32(d * up) - f32(d * old))) * -1)
        return [fn(div(1, cur)), fn(change, 0, True, False)]
    if fmt in (12, 13):
        sel = -1 if fmt == 13 else 1
        d = div(1, f32(base * sel))
        return pct(f32(up * d), f32(old * d), f32(f32(d * cur) - sel), 0)
    return ["", ""]


def sanitize_float(v):
    """FString::SanitizeFloat (Kismet Conv_FloatToString): "%f" without trailing zeros, at least one decimal."""
    s = "%f" % v
    s = s.rstrip("0")
    return s + "0" if s.endswith(".") else s


def build_value(key, base, effects):
    """(base + sum of added amounts) x product of percentage amounts of the effects with that key (32-bit).
    base None: only the product."""
    add, mult = 0.0, 1.0
    for e in effects:
        if len(e) < 3 or not key_matches(e[0], key):
            continue
        if e[2]:
            mult = f32(mult * e[1])
        else:
            add = f32(add + e[1])
    return mult if base is None else f32(f32(base + add) * mult)


# ---------------------------------------------------------------- export
NORMALIZED = (9, 10, 12, 13)  # shown relative to the base value
LABEL_ONLY = (5, 6)  # shown without a number


class Stats:
    """Per item: rows of the stats panel; per upgrade asset: its effects. Collects why items fail."""

    def __init__(self, store, texts):
        self.store, self.texts = store, texts
        self.native = Native()
        self.world = World(store, texts, self.native)
        self.unsupported = {}
        self.effect_cache = {}
        self.guessed = set()  # bases taken from a same-named property

    def item(self, item_id, item_cls, tiers, overclocks):
        """Stats rows for an item class, or None (reason in self.unsupported).
        tiers: per tier the asset paths of its options; overclocks: asset paths of the overclocks."""
        try:
            options = [p for t in tiers for p in t]
            fx = lambda p: self.item_effects(p, item_cls)  # noqa: E731
            effects = [e for u in options + overclocks for e in fx(u)]
            oc_keys = {e[0] for u in overclocks for e in fx(u)}
            choices = [[fx(p) for p in t] for t in tiers] + [[fx(p) for p in overclocks]]
            interp = Interp(self.world, item_cls, choices)
            out = self.rows_of(interp.run(), effects, oc_keys)
            self.guessed.update(interp.guessed)
            if any("up" in r for r in out):
                self.add_down(out, item_cls, choices, effects, oc_keys)
            return out or None
        except Unsupported as e:
            self.unsupported[item_id] = str(e)
        except (KeyError, IndexError, TypeError, ValueError, AttributeError, RecursionError) as e:
            self.unsupported[item_id] = "%s: %s" % (type(e).__name__, e)
        return None

    def rows_of(self, rows, effects, oc_keys):
        """Interpreter rows -> game.json rows (conditions on upgrades this item cannot have dropped)."""
        out = []

        def possible(k):
            return any(key_matches(e[0], k) for e in effects)

        for entry, conds in rows:
            out_conds = []
            for c in conds:
                if c.startswith("?"):
                    out_conds.append(c)
                    continue
                neg, pre, keys = cond_parts(c)
                keys = [k for k in keys if possible(k)]
                if neg:
                    if keys:
                        out_conds.append(c)
                elif not keys:
                    break  # nothing in this game can show it
                else:
                    out_conds.append(pre + "|".join(keys))
            else:
                r = self.row(entry, out_conds, effects, oc_keys)
                if r:
                    out.append(r)
        return out

    def add_down(self, out, item_cls, choices, effects, oc_keys):
        """Fixed-text rows: "down", the delta text the Blueprint shows when the hovered build removes the
        upgrade (PreviewStatus 3; WPN_SMG_OneHand: "-20" instead of "20"). Read from a second run of
        GetGearStatEntry; rows not found there get none (the site then hides them in that case)."""
        try:
            interp = Interp(self.world, item_cls, choices, status_as=3)
            other = self.rows_of(interp.run(), effects, oc_keys)
        except Unsupported:
            return
        def same(a, b):  # WPN_Gatling: "Quik-Chill Coolant  Charges" (two spaces) in the removed case
            return " ".join(a.split()) == " ".join(b.split())

        for r in out:
            if "up" not in r:
                continue
            twin = next((x for x in other if "text" in x and same(x["label"], r["label"]) and x["text"] == r["text"]
                         and x.get("if") == r.get("if") and x.get("up")), None)
            if twin:
                r["down"] = twin["up"]

    def row(self, entry, conds, effects, oc_keys):
        label = entry.get("Text") or ""
        if not isinstance(label, str):
            raise Unsupported("label computed from values")
        label = label.strip()
        val = entry.get("Value")
        up = entry.get("UpgradeValue")
        cond = {}
        if [c for c in conds if not c.startswith("?")]:
            cond["if"] = [c for c in conds if not c.startswith("?")]
        if [c for c in conds if c.startswith("?")]:
            cond["when"] = [self.tree(Expr(json.loads(c[1:])), effects) for c in conds if c.startswith("?")]
        plain_up = isinstance(val, VText) and isinstance(up, VText) and up.part == 1 and up.uv is val.uv
        if isinstance(val, Expr) or (isinstance(val, VText) and up is not None and not plain_up):
            # text computed at display time: value and preview delta as expressions
            if not label:
                raise Unsupported("row without label")
            r = {"label": label, "expr": self.tree(val, effects)}
            if up not in (None, ""):
                r["dexpr"] = self.tree(up, effects)
            # delta colour: the PreviewStatus FormatPreviewStatus made from a value, copied into this entry
            # (WPN_AutoCannon rounds the Base Spread entry and keeps its PreviewStatus)
            ps = entry.get("PreviewStatus")
            better = {0: 1, 1: -1}.get(ps.pfmt) if isinstance(ps, Status) else None
            if better and ps.delta_tree() is not None:
                r["better"] = better
                r["bdelta"] = self.tree(Expr(ps.delta_tree()), effects)
            r.update(cond)
            return r
        if isinstance(val, VText):
            uv = val.uv
            base = uv.base
            if base is None:
                mine = [e for e in effects if len(e) > 1 and key_matches(e[0], uv.key)]
                if val.fmt in NORMALIZED and all(e[2] for e in mine):
                    base = 1.0  # relative to base: the base itself cancels out
                elif val.fmt in LABEL_ONLY:
                    base = 0.0  # the game shows no number
                else:
                    raise Unsupported("base of " + uv.key)
            if not label:
                raise Unsupported("row without label")
            r = {"label": label, "fmt": val.fmt, "base": round(float(base), 5)}
            if uv.key:
                r["key"] = uv.key
                better = {0: 1, 1: -1}.get(val.pfmt)
                if better:
                    r["better"] = better
            r.update(cond)
            return r
        if isinstance(val, SYM):
            raise Unsupported("row value " + type(val).__name__)
        if val in ("", None):  # the "+ name" lines of UPGDESC_Generic
            if not label:
                return None
            by_oc = any(all(any(key_matches(o, k) for o in oc_keys) for k in cond_keys(c))
                        for c in conds if c[0] not in "!?")
            tone = "bad" if entry.get("IsPenalty") else "oc" if by_oc else "upgrade"
            return dict({"line": label, "tone": tone}, **cond)
        r = {"label": label, "text": str(val)}
        up = entry.get("UpgradeValue")
        if isinstance(up, str) and up:
            r["up"] = up
        r.update(cond)
        return r

    def tree(self, v, effects):
        """Expression tree of a row value for game.json; bases must be known, texts of base values are not."""
        t = tree_of(v)

        def check(x):
            if isinstance(x, list):
                if x[0] not in EXPR_OPS:
                    raise Unsupported("expression op %r" % x[0])
                if x[0] in ("val", "old", "text"):
                    if x[2] is None:
                        raise Unsupported("base of %s" % x[1])
                    x[2] = round(float(x[2]), 5)
                if x[0] == "text" and x[4] > 1:
                    raise Unsupported("base value text")
                for y in x[1:]:
                    check(y)
        t = copy_tree(t)
        check(t)
        return t

    def effects(self, path):
        """Effects of an upgrade or overclock asset, combined upgrades flattened:
        [key] for "this asset is equipped", [key, amount, 1 if percentage else 0] for a value change."""
        if path in self.effect_cache:
            return self.effect_cache[path]
        out, seen = [], set()

        def walk(p, depth):
            if depth > 6 or p.lower() in seen or not is_ref(p):
                return
            seen.add(p.lower())
            out.append(["a:" + self.store.name(p)])
            asset = self.store.get(p)
            e = asset and asset.export(object_name(p))
            if not e:
                return
            cls = object_name(asset.ref(e["cls"]))
            pr = self.world.merged(asset, e)
            for c in pr.get("CombinedUpgrades") or []:
                walk(c, depth + 1)
            ut = self.upgrade_type(cls, pr)
            if cls == "SinusoidalDamageUpgrade":  # not in the stubs: see Natives.n_GetSinusoidalDamageValue
                out.append(["SinusoidalDamageUpgrade.Max", round(float(pr.get("SinusoidalDamageRangeMax") or 0), 5), 0])
                out.append(["SinusoidalDamageUpgrade.Min", round(float(pr.get("SinusoidalDamageRangeMin") or 0), 5), 0])
            elif cls in BONUS:
                prop, field = BONUS[cls]
                params = pr.get(prop) if prop else None
                amount = round(float(pr.get(field) or 0.0), 5) if field else 0.0
                # IsPercentage multiplies (OC_ShieldTransferModule_NoHeat_HPC: heat x 0)
                pct = 1 if field and pr.get("IsPercentage") is True else 0
                for param in params if isinstance(params, list) else [params]:
                    out.append([bonus_key(cls, param), amount, pct])
            elif cls in BASES and None in BASES[cls]:
                out.append([cls, round(float(pr.get("Amount") or 0.0), 5), 1 if pr.get("IsPercentage") else 0])
            elif isinstance(ut, str) and "::" in ut:
                sub = pr.get("RequiredClass") or pr.get("ProjectileClass")
                classes = [t for t, _ in self.native.sigs.get(cls, []) if t.startswith("TSubclassOf")
                           and "StatusEffect" not in t and "Component" not in t]
                if cls in self.native.sigs and len(classes) < 2:  # GetUpgradedValue takes no sub item (ZiplineGunUpgrade.ProjectileClass
                    sub = None  # is the zipline the upgrade switches to)
                dct = self.native.enum_index(pr.get("DamageComponentType") or "EDamageComponentType::Primary")
                key = upgrade_key(cls, ut.split("::", 1)[1], dct, sub)
                keys = [key]
                if cls in PARAM_KEYS:
                    keys = [key + "~" + object_name(pr[p]) for p in PARAM_KEYS[cls] if is_ref(pr.get(p))]
                for k in keys:
                    out.append([k, round(float(pr.get("Amount") or 0.0), 5), 1 if pr.get("IsPercentage") else 0])
        walk(path, 0)
        self.effect_cache[path] = out
        return out

    def item_effects(self, path, item_cls):
        """effects() for one item: a RequiredClass that is the item itself (or a parent class of it) becomes
        "key@": it applies to rows read without a sub item (the native then checks the item) or with the item
        as the sub item, not to rows of another sub item (WPN_CoilGun "Shockwave": DamageUpgrade.Damage.2 read
        without one; WPN_SawedOffShotgun +90 shockwave damage not on the PRJ_HighPressureRounds_C row)."""
        own = {object_name(c) for c in self.world.class_chain(item_cls)}
        out = []
        for e in self.effects(path):
            base, _, sub = e[0].partition("@")
            out.append([base + "@"] + list(e[1:]) if sub in own else e)
        return out

    def upgrade_type(self, cls, pr):
        """'EAmmoDrivenWeapnUpgradeType::ClipSize': the upgrade type of an upgrade asset (merged with its
        native defaults), found by the enum its class's GetUpgradedValue takes; else a property UpgradeType."""
        enum = next((t for t, _ in self.native.sigs.get(cls, []) if t.startswith("E") and t != "EDamageComponentType"),
                    None)
        for k, v in pr.items():
            if isinstance(v, str) and ((enum and v.startswith(enum + "::")) or (not enum and k.lower() == "upgradetype")):
                return v
        return None

    def attach(self, item, item_cls, option_paths, oc_paths):
        """Adds item["stats"] and, for every option and overclock of the item, "effects" (only the keys the
        stats rows use). option_paths: per tier, the asset paths in option order; oc_paths: in overclock order.
        Items without reproducible stats get neither."""
        if item["id"] in ARMOR_T3:
            return armor_stats(item)
        rows = self.item(item["id"], item_cls, option_paths, oc_paths)
        if not rows:
            return
        item["stats"] = rows
        row_keys = {r["key"] for r in rows if r.get("key")}
        for r in rows:
            for t in [r.get("expr"), r.get("dexpr"), r.get("bdelta")] + r.get("when", []):
                row_keys.update(k for k, _ in tree_keys(t))
        used_conds = {k for r in rows for c in r.get("if", []) for k in cond_keys(c)}

        all_keys = row_keys | used_conds

        def concrete_keys(e):
            """Keys the site matches this effect with: its own, or for a parent sub item class (RequiredClass)
            the row keys of the child classes (stats.js matches sub items by name only)."""
            if e[0].startswith("a:"):
                return [e[0]] if e[0] in used_conds else []
            if e[0].endswith("@"):  # the item's own: "key@" for rows without a sub item, else the exact key
                return sorted({e[0] if "@" not in k else k for k in all_keys if key_matches(e[0], k)})
            direct = [k for k in all_keys if e[0] == k or e[0] == k.split("@")[0]]
            children = sorted(k for k in all_keys if k not in direct and key_matches(e[0], k))
            return ([e[0]] if direct else []) + children

        def effects(path):
            out, seen = [], set()
            for e in self.item_effects(path, item_cls):
                for k in concrete_keys(e):
                    if len(e) > 1 or k not in seen:
                        out.append([k] + list(e[1:]))
                        seen.add(k)
            return out

        for tier, paths in zip(item["tiers"], option_paths):
            for opt, path in zip(tier["options"], paths):
                opt["effects"] = effects(path)
        for oc, path in zip(item["overclocks"], oc_paths):
            oc["effects"] = effects(path)


    def warnings(self, items_total):
        out = ["stats panel for %d of %d items; the others show their description" % (
            items_total - len(self.unsupported), items_total)]
        for item_id, why in sorted(self.unsupported.items()):
            out.append("no stats for %s: %s" % (item_id, why))
        if self.guessed:
            out.append("stat bases taken from a same-named property: %s" % ", ".join(sorted(self.guessed)))
        return out


# Armor: the game computes these in native code (GetUpgradedStatValue on the pawn stats), and the bases of Carry
# Capacity and Regeneration Rate are in neither the packages nor the stubs. So the panel is written out by hand
# from the user's game screenshots (2026-11-22, all four rigs): same rows and upgrades for every class, except T3.
# Effects are by option position: tier -> option -> effects.
ARMOR_ROWS = [
    {"label": "Shield", "fmt": 0, "base": 25.0, "key": "Armor.Shield", "better": 1},
    {"label": "Health", "fmt": 0, "base": 125.0, "key": "Armor.Health", "better": 1},
    {"label": "Regeneration Delay", "fmt": 1, "base": 7.0, "key": "Armor.RegenDelay", "better": -1},
    {"label": "Regeneration Rate", "fmt": 2, "base": 1.0, "key": "Armor.RegenRate", "better": 1},
    {"label": "Revive Invulnerability", "fmt": 1, "base": 3.0, "key": "Armor.Revive", "better": 1},
    {"label": "Carry Capacity", "fmt": 0, "base": 40.0, "key": "Armor.Carry", "better": 1},
]
ARMOR_LINES = [
    {"line": "AoE Damage On Shield Break", "tone": "upgrade", "if": ["Armor.Shockwave"]},
    {"line": "AoE Stun On Shield Break", "tone": "upgrade", "if": ["Armor.Static"]},
]
ARMOR_TIERS = [
    [[["Armor.RegenDelay", -1.0, False]],                                       # Improved Generator
     [["Armor.RegenRate", 1.0, False], ["Armor.RegenDelay", 2.0, False]],       # Boosted Converter
     [["Armor.Carry", 5.0, False]]],                                            # Bigger Mineral Bag
    [[["Armor.Shield", 5.0, False]],                                            # Overcharger
     [["Armor.Health", 20.0, False]]],                                          # Healthy
    None,                                                                       # T3: ARMOR_T3
    [[["Armor.Shockwave"]],                                                     # Shockwave
     [["Armor.Static"]],                                                        # Static Discharge
     [["Armor.Revive", 3.0, False]]],                                           # Breathing Room
]
ARMOR_T3 = {  # item id: (row label, resistance)
    "Armor_Driller": ("Fire Resistance", 0.5),        # Temperature Insulation
    "Armor_Engineer": ("Poison Resistance", 0.5),     # Hazmat System
    "Armor_Gunner": ("Explosion Resistance", 0.5),    # Reactive Armor
    "Armor_Scout": ("Fall Damage Reduction", 0.33),   # Shock Absorbers
}


def armor_stats(item):
    """item["stats"] and option effects of an armor rig (see ARMOR_ROWS)."""
    label, amount = ARMOR_T3[item["id"]]
    resist = {"label": label, "fmt": 2, "base": 0.0, "key": "Armor.Resist", "better": 1, "if": ["Armor.Resist"]}
    item["stats"] = [dict(r) for r in ARMOR_ROWS] + [resist] + [dict(r) for r in ARMOR_LINES]
    tiers = list(ARMOR_TIERS)
    tiers[2] = [[["Armor.Resist", amount, False]]]
    if len(item["tiers"]) != len(tiers):
        raise ValueError("%s: %d tiers, the armor table has %d" % (item["id"], len(item["tiers"]), len(tiers)))
    for t, (tier, table) in enumerate(zip(item["tiers"], tiers)):
        if len(tier["options"]) != len(table):
            raise ValueError("%s tier %d: %d options, the armor table has %d" % (
                item["id"], t + 1, len(tier["options"]), len(table)))
        for opt, fx in zip(tier["options"], table):
            opt["effects"] = [list(e) for e in fx]
    for oc in item.get("overclocks", []):
        oc["effects"] = []


def check_stats(items):
    """Shape checks of item.stats and option/overclock effects. items: [(where, item)]. Returns (lines, errors)."""
    errors, n_items, n_rows, n_lines = [], 0, 0, 0
    for where, it in items:
        rows = it.get("stats")
        opts = [o for t in it.get("tiers", []) for o in t["options"]] + list(it.get("overclocks", []))
        if not rows:
            if any("effects" in o for o in opts):
                errors.append("%s: effects without stats" % where)
            continue
        n_items += 1
        effects = [e for o in opts for e in o.get("effects", [])]

        def ok(t):
            if not isinstance(t, list):
                return isinstance(t, (int, float, str, bool))
            return bool(t) and t[0] in EXPR_OPS and all(ok(x) for x in t[1:] if x is not None)
        for o in opts:
            for e in o.get("effects", []):
                if not (isinstance(e, list) and isinstance(e[0], str) and len(e) in (1, 3)):
                    errors.append("%s %s: bad effect %r" % (where, o["id"], e))
                elif len(e) == 3 and not (isinstance(e[1], (int, float)) and e[2] in (0, 1)):
                    errors.append("%s %s: bad effect %r" % (where, o["id"], e))
        for r in rows:
            for c in r.get("if", []):
                if not all(k and any(key_matches(e[0], k) for e in effects) for k in cond_keys(c)):
                    errors.append("%s: condition %s that no upgrade sets" % (where, c))
            if not all(ok(t) for t in r.get("when", [])):
                errors.append("%s: bad value condition %r" % (where, r))
            if "line" in r:
                n_lines += 1
                if not r["line"] or r.get("tone") not in ("upgrade", "oc", "bad"):
                    errors.append("%s: bad line %r" % (where, r))
                continue
            n_rows += 1
            if not r.get("label") or "{" in r["label"]:
                errors.append("%s: bad label %r" % (where, r))
            if "text" in r:
                if not all(isinstance(r.get(k, ""), str) for k in ("text", "up", "down")):
                    errors.append("%s: bad text row %r" % (where, r))
                continue
            if "expr" in r:
                if not all(ok(r[k]) for k in ("expr", "dexpr", "bdelta") if k in r) or \
                        ("bdelta" in r) != (r.get("better") in (1, -1)):
                    errors.append("%s: bad expression row %r" % (where, r))
                continue
            if not (isinstance(r.get("fmt"), int) and 0 <= r["fmt"] <= 13 and isinstance(r.get("base"), (int, float))):
                errors.append("%s: bad row %r" % (where, r))
    return ["stats: %d items, %d rows, %d lines" % (n_items, n_rows, n_lines)], errors


__all__ = ["Stats", "check_stats", "format_number", "Unsupported", "BASES", "key_matches"]
