"""Imports the Dwarf Builds art (Tools/dwarfbuilds/art/*.png) as UI textures into /Game/_DwarfBuilds/Art.

Runs inside the editor (UE Python), started by Tools/dwarfbuilds_build.py before BPForge:
  UE4Editor-Cmd FSD.uproject -run=pythonscript -script=Tools/dwarfbuilds_art.py -EnablePlugins=PythonScriptPlugin
Each <name>.png becomes T_DB_<Name> (logo.png -> T_DB_Logo): UserInterface2D compression, no mips, UI group,
never streamed. Re-imported every run (cheap), so the PNG is the source of truth.
"""
import os

import unreal

HERE = os.path.dirname(os.path.abspath(__file__))
SRC = os.path.join(HERE, "dwarfbuilds", "art")
DEST = "/Game/_DwarfBuilds/Art"


def asset_name(png):
    base = os.path.splitext(os.path.basename(png))[0]
    return "T_DB_" + "".join(part.capitalize() for part in base.replace("-", "_").split("_"))


def main():
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    pngs = sorted(os.path.join(SRC, f) for f in os.listdir(SRC) if f.lower().endswith(".png"))
    for png in pngs:
        name = asset_name(png)
        task = unreal.AssetImportTask()
        task.set_editor_property("filename", png)
        task.set_editor_property("destination_path", DEST)
        task.set_editor_property("destination_name", name)
        task.set_editor_property("replace_existing", True)
        task.set_editor_property("automated", True)
        task.set_editor_property("save", False)
        tools.import_asset_tasks([task])
        path = DEST + "/" + name
        tex = unreal.load_asset(path)
        if not tex:
            raise RuntimeError("import failed: " + png)
        tex.set_editor_property("compression_settings", unreal.TextureCompressionSettings.TC_EDITOR_ICON)
        tex.set_editor_property("lod_group", unreal.TextureGroup.TEXTUREGROUP_UI)
        tex.set_editor_property("mip_gen_settings", unreal.TextureMipGenSettings.TMGS_NO_MIPMAPS)
        tex.set_editor_property("never_stream", True)
        tex.set_editor_property("srgb", True)
        if not unreal.EditorAssetLibrary.save_asset(path, only_if_is_dirty=False):
            raise RuntimeError("save failed: " + path)
        unreal.log("DwarfBuildsArt: imported %s -> %s" % (os.path.basename(png), path))
    unreal.log("DwarfBuildsArt: done, %d textures" % len(pngs))


main()
