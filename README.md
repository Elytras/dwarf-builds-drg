# Dwarf Builds

[![Latest release](https://img.shields.io/github/v/release/byrodi/dwarf-builds-drg)](https://github.com/byrodi/dwarf-builds-drg/releases/latest)
[![License](https://img.shields.io/github/license/byrodi/dwarf-builds-drg?cacheSeconds=3600)](LICENSE)

Deep Rock Galactic mod: see your whole team's builds in game and share them as build codes.

Builds are shared on the Dwarf Builds website, [byrodi/dwarf-builds-web](https://github.com/byrodi/dwarf-builds-web),
included here as the `Web` submodule.

![The class selection screen with Dwarf Builds: the team's builds on the left, your own on the right](.github/images/cover.jpg)

## Features

- Press B to see every player's weapons, upgrades, overclocks, equipment and perks. With
  [Mod Hub](https://mod.io/g/drg/m/mod-hub) the key can be changed on the mod's page there.
- The class selection screen shows your team's builds next to yours.
- Copy any build as a short code, or paste one to preview it.
- TAKE BUILD saves a code into one of your loadouts, using only what you own.
- BROWSE opens the build website in the Steam overlay.
- Client-side: other players do not need the mod.

Other players' perks show when the host runs the mod too; the game does not send them to clients.

## Screenshots

**The window (B).** One column per player: weapons with every upgrade tier, overclocks, equipment and perks,
with tooltips in the game's style.

![The Dwarf Builds window with four players: weapons, overclocks, equipment and perks of everyone in the lobby](.github/images/team-window.jpg)

**TAKE BUILD.** Save a teammate's build into one of your loadout slots; only what you own is applied.

![The TAKE BUILD dialog: loadout slots A to Z and the option to equip the loadout after saving](.github/images/take-build.jpg)

**Build codes.** Paste a code on the BUILD CODE tab, check the preview, then take it.

![The BUILD CODE tab: a pasted code shown as a full build, with TAKE BUILD](.github/images/build-code.jpg)

**The website.** Find and share builds on the Dwarf Builds website ([source](https://github.com/byrodi/dwarf-builds-web)); BROWSE opens it
in the Steam overlay.

![The Dwarf Builds website: builds shared by players, filtered by class, weapon and overclock](.github/images/website.jpg)

## Installation

Download `dwarf-builds-0.1.0.zip` from the [latest release](https://github.com/byrodi/dwarf-builds-drg/releases/latest)
and add it to [mint](https://github.com/trumank/mint) as a local mod, then install the profile. Or build it from
source (below).

## Usage

| Key or button | What it does |
| --- | --- |
| B (or your key from Mod Hub) | Opens the window (when no other game window is open). The same key or Escape closes it. |
| TEAM | Everyone in the lobby, one column per player. |
| BUILD CODE | Paste a code, preview it, take it into a loadout, copy or publish your own. |
| BROWSE | The website in the Steam overlay. |

Copy a code on the website, then press PASTE FROM CLIPBOARD on the BUILD CODE tab.

## Building from source

The mod is C++ compiled to cooked Blueprints by [AssetGen](https://github.com/Elytras/CPP-to-blueprint-compiler)
(the sources are in `Mod/DwarfBuilds`, see [NOTES.md](NOTES.md)). No Unreal editor or project is needed. You need:

- AssetGen, built as its README says (Windows: `AssetGen.sln`, Release x64; Linux: CMake), with LLVM clang 20 on
  `PATH` and Python 3 with `pyyaml`.
- [DRG-Blueprint-Cpp-SDK](https://github.com/Elytras/DRG-Blueprint-Cpp-SDK): the game's API as C++ headers. Use its
  `experimental/UeApi`; the release `UeApi` lacks `UProficiencyPerkAsset`, which the perk tiles read.
- `UnrealPak.exe` from Unreal Engine 4.27 (on `PATH` or in `UNREALPAK`) to pack the pak.

```
git clone https://github.com/Elytras/CPP-to-blueprint-compiler.git AssetGen
git clone https://github.com/Elytras/DRG-Blueprint-Cpp-SDK.git
git clone <this repository> dwarf-builds-drg
python AssetGen/tools/bpbuild.py dwarf-builds-drg/Mod DRG-Blueprint-Cpp-SDK/experimental/UeApi AssetGen/x64/Release/assetgen.exe
```

The pak lands in `Mod/out/DwarfBuilds_P.pak`; add it to [mint](https://github.com/trumank/mint) as a local mod.
The build refuses to pack when the replicated surface differs from the one 0.1.0 shipped (`Mod/netcontract.json`),
so a lobby can mix this build and the mod.io release.

The logo texture ships as the one cooked for 0.1.0 (`Mod/Bundle`), since AssetGen does not write textures. Its
source is `Tools/dwarfbuilds/art`.

The website's game data comes from the installed game: `python Tools/dwarfbuilds_export.py --game <DRG folder>`
reads the game's pak with UnrealPak (set `UNREALPAK`) and writes `Web/public/data` and `Web/public/img`.

## Credits

The idea and the window layout come from [Build Inspector](https://mod.io/g/drg/m/build-inspector) by
AssemblyStorm. Dwarf Builds is a remake with its own assets and code.

## License

[MIT](LICENSE)
