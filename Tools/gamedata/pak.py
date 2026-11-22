"""Extract files from the installed game pak with UnrealPak (read-only), and read the Steam build id."""
import datetime
import os
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

TOOLS = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.dirname(TOOLS)


def _default_game():
    """The game folder: SteamInstall from DRGModdingAutomationScripts/Configs/LocalConfig.ini (the template's own
    config; it points at the game's folder), else Steam's default library. Pass --game to use another one."""
    ini = os.path.join(ROOT, "DRGModdingAutomationScripts", "Configs", "LocalConfig.ini")
    if os.path.isfile(ini):
        with open(ini, encoding="utf-8") as lines:
            for line in lines:
                key, _, value = line.strip().partition("=")
                if key == "SteamInstall" and value.strip().strip('"'):
                    return value.strip().strip('"')
    return "C:/Program Files (x86)/Steam/steamapps/common/Deep Rock Galactic"


DEFAULT_GAME = _default_game()
APP_ID = "548430"


def unrealpak_exe():
    """UnrealPak.exe of the engine named in LocalConfig.ini (the path is never printed)."""
    if TOOLS not in sys.path:
        sys.path.insert(0, TOOLS)
    from pack_mod import engine_dir
    exe = os.path.join(engine_dir(), "Engine", "Binaries", "Win64", "UnrealPak.exe")
    if not os.path.isfile(exe):
        sys.exit("UnrealPak.exe not found under the engine folder from LocalConfig.ini")
    return exe


def game_pak(game_dir):
    return os.path.join(game_dir, "FSD", "Content", "Paks", "FSD-WindowsNoEditor.pak")


def steam_version(game_dir):
    """'<date> <branch> <buildid>'. Reads only the buildid and BetaKey lines of the app manifest."""
    library = os.path.dirname(os.path.dirname(os.path.normpath(game_dir)))  # .../steamapps
    manifest = os.path.join(library, "appmanifest_%s.acf" % APP_ID)
    build, branch = "unknown", "public"
    try:
        with open(manifest, encoding="utf-8", errors="replace") as f:
            for line in f:
                m = re.match(r'\s*"(buildid|BetaKey)"\s+"([^"]*)"', line)
                if m and m.group(1) == "buildid":
                    build = m.group(2)
                elif m and m.group(2):
                    branch = m.group(2)
    except OSError:
        print("warning: Steam app manifest not found, version has no build id")
    return "%s %s %s" % (datetime.date.today().isoformat(), branch, build)


class Extractor:
    """Runs UnrealPak -Extract into <out>/FSD/Content/... . Never writes into the game folder."""

    def __init__(self, pak, out, exe=None, jobs=8):
        self.pak, self.out, self.exe, self.jobs = pak, out, exe or unrealpak_exe(), jobs
        self.calls = 0

    def clear(self):
        shutil.rmtree(self.out, ignore_errors=True)
        os.makedirs(self.out, exist_ok=True)

    def _one(self, flt):
        self.calls += 1
        res = subprocess.run([self.exe, self.pak, "-Extract", self.out, "-Filter=" + flt],
                             stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace")
        return sum(1 for line in res.stdout.splitlines() if "Extracted " in line)

    def extract(self, filters):
        """Filters are pak paths with wildcards, like FSD/Content/WeaponsNTools/*/UPG_*. Returns file count."""
        filters = list(filters)
        if not filters:
            return 0
        with ThreadPoolExecutor(self.jobs) as pool:
            return sum(pool.map(self._one, filters))


def package_filter(game_path):
    """/Game/A/B.B -> FSD/Content/A/B.* (matches .uasset, .uexp, .ubulk)."""
    pkg = game_path.split(".")[0]
    return "FSD/Content/" + pkg[len("/Game/"):] + ".*"
