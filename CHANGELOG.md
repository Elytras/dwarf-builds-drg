# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/2.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

Based on upstream 0.2.0, whose network surface it keeps: a lobby can mix this build, 0.2.0, 0.1.0 and players
without the mod.

### Changed

- The window lives in the mod's page in Mod Hub (or Elytras Mod Manager): opening the hub on Dwarf Builds shows
  the builds at once. The B key, its setting and the key picker are gone.
- From upstream 0.2.0: a build is sent when it changes (the game's own events), not on a timer; the host keeps the
  0.1.0 perk table up only while a player without this version is in the lobby; the class selection screen is
  searched for only a short while after the level starts, a selection starts or a game window opens.

- Built from C++ with AssetGen instead of the BPForge editor spec; no Unreal editor needed (README, NOTES.md).
  Network-compatible with 0.1.0.
- Every player's build and perks reach every client when the host runs this build: each player's mod sends its
  own build to the host, which shares it with everyone (`DB_BuildShare_v2`, next to 0.1.0's unchanged perk table).
- Perks the game does not share show as not available instead of an empty row.
- The team window and the class selection overlay redraw only what changed, and the class selection screen is
  no longer polled for the whole level.

### Fixed

- From upstream 0.2.0: TAKE BUILD cleared the slot's overclock or upgrade when the code named one this game version
  does not have. It now keeps the slot's own and says so; the build shows UNKNOWN OVERCLOCK.
- A code sent through the relay with a character no build code has is ignored.
- The pak no longer ships its own copies of Mod Hub's interfaces (`/Game/_ModHub`), which could stand in for the
  hub's real ones.

## [0.1.0] - 2026-11-22

### Added

- The Dwarf Builds window (B): one column per player with weapons, upgrade tiers, overclocks, equipment and
  perks, with tooltips in the game's style.
- Your team's builds on the class selection screen, next to your own.
- Build codes (DB1): COPY CODE under any player, PASTE FROM CLIPBOARD on the BUILD CODE tab with a preview.
- TAKE BUILD: saves a build into one of your loadout slots, using only what you own.
- BROWSE, PUBLISH MY BUILD and OPEN IN BROWSER for the Dwarf Builds website.
- Lobby-size mods: a roster strip, class filters and pages of 4 for more than 4 players.
- Mod Hub page to change the key that opens the window.
- Works on the release game and on the experimental branch.

[unreleased]: https://github.com/byrodi/dwarf-builds-drg/compare/v0.1.0...HEAD
[0.1.0]: https://github.com/byrodi/dwarf-builds-drg/releases/tag/v0.1.0
