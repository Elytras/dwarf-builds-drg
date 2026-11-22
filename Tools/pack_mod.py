"""Cooks the project and packs one content folder into its own pak, without the Umbra configs.

Usage: python Tools/pack_mod.py <content folder> <pak name> [--no-cook]
Example: python Tools/pack_mod.py _DwarfBuilds dwarfbuilds
Result: DRGModdingAutomationScripts/Temp/<pak name>.pak. Install it with
python Tools/install_mint.py <mint exe> --pak <pak name>.

The cook is the same command as UtilityBats/CookUEProject.bat (whole project). The pak holds only
FSD/Content/<content folder>, so vanilla stubs and other mods in the project never ship with it.
"""
import os
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCRIPTS = os.path.join(ROOT, "DRGModdingAutomationScripts")
COOKED = os.path.join(ROOT, "Saved", "Cooked", "WindowsNoEditor", "FSD", "Content")


def engine_dir():
    with open(os.path.join(SCRIPTS, "Configs", "LocalConfig.ini"), encoding="utf-8") as lines:
        for line in lines:
            key, _, value = line.strip().partition("=")
            if key == "UnrealEngineLocation":
                return value
    sys.exit("UnrealEngineLocation not found in LocalConfig.ini")


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if len(args) != 2:
        sys.exit(__doc__)
    folder, pak_name = args
    engine = os.path.join(engine_dir(), "Engine", "Binaries", "Win64")

    if "--no-cook" not in sys.argv:
        log = os.path.join(ROOT, "Saved", "Logs", "Cook_%s.log" % pak_name)
        code = subprocess.call([os.path.join(engine, "UE4Editor-Cmd.exe"), os.path.join(ROOT, "FSD.uproject"),
                                "-run=cook", "-targetplatform=WindowsNoEditor", "-unattended", "-nopause",
                                "-abslog=" + log], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        errors = []
        with open(log, encoding="utf-8", errors="replace") as lines:
            errors = [line.rstrip() for line in lines if "Error:" in line and folder in line]
        print("cook exit code", code)
        for line in errors:
            print(line)
        if code != 0 or errors:
            sys.exit(1)

    source = os.path.join(COOKED, folder)
    if not os.path.isdir(source):
        sys.exit("Nothing cooked at %s" % source)
    stage = os.path.join(SCRIPTS, "Temp", "Stage_" + pak_name)
    shutil.rmtree(stage, ignore_errors=True)
    shutil.copytree(source, os.path.join(stage, "Content", folder))
    listing = os.path.join(SCRIPTS, "Temp", "Input_%s.txt" % pak_name)
    with open(listing, "w", encoding="utf-8") as out:
        out.write('"%s\\" "../../../FSD/"\n' % stage)
    pak = os.path.join(SCRIPTS, "Temp", pak_name + ".pak")
    code = subprocess.call([os.path.join(engine, "UnrealPak.exe"), pak, "-Create=" + listing],
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    count = sum(len(files) for _, _, files in os.walk(stage))
    print("packed %d files into %s (exit code %d)" % (count, pak, code))
    sys.exit(code)


if __name__ == "__main__":
    main()
