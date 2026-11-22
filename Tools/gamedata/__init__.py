"""Read the installed game's cooked data (read-only) and build the Dwarf Builds website data.

Modules:
  pak      UnrealPak extraction into Saved/GameData/pak, game paths, Steam build version
  assets   cooked packages with FText, TMap and GUID decoding; a cached store with lazy extraction
  text     locres, string tables, FText to English string
  texture  Texture2D to RGBA (stdlib only), PNG writer, small image helpers
  gear     classes, items, upgrade tiers, overclocks
  perks    perks with filled descriptions
  icons    icon registry: game texture -> web PNG
Entry point: Tools/dwarfbuilds_export.py
"""
