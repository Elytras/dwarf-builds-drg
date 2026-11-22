"""Mod Hub integration: DB_Hub (registers the mod) and DB_HubPage (its page: the window's hotkey).

Mod Hub is another mod. DB_Mod spawns DB_Hub only when Mod Hub's IHubMod interface can be loaded, and it loads
DB_Hub by path, so without Mod Hub nothing here is ever loaded. Mod Hub asks DB_Hub for its pages (GetModPages)
and shows DB_HubPage inside its own window.

The page: the hotkey (an InputKeySelector; keyboard keys only, no modifiers), RESET TO B, and one line that answers
every change. Mouse buttons, Escape and E are refused (the window uses Escape and E itself). A change is saved at
once (DB_Data.SetOpenKey) and the next press of the new key opens the window.
"""
from dwarfbuilds.common import (BODY, CARBON, DATA, DEFAULT_KEY, GS, HUBMOD, HUBPAGE, KEY, KML, KSTR, KTXT,
                                LIGHT_TEXT, MOD_NAME, MOD_VERSION, MODHUB, ORANGE, RIFT, UMG, DARK, Graph, box, cls,
                                font, game_button, hslot, margin, press_feedback, ref, slate, t, text, vslot, w)
from dwarfbuilds.common import _btn_brush  # noqa: PLC2701  (the game button's brush, for the key selector)
from dwarfbuilds.shell import kml, set_text, text_colour, valid, wcall

ACTOR = "/Script/Engine.Actor"
INPUTLIB = "/Script/Engine.KismetInputLibrary"
CHORD = "/Script/Slate.InputChord"
MOD_AUTHOR = "rodionmasalov"
GOOD = (0.33, 0.89, 0.0)
BAD = (1.0, 0.25, 0.15)
HINT = "Press the key box, then the new key. Escape cancels."


def iface(name):
    return MODHUB + name + "." + name + "_C"


# ============================================================================= DB_Hub

def g_hub_mod_info():
    g = Graph()
    g.flow("entry", "result")
    g.defaults.update({"result.ModName": MOD_NAME, "result.ModAuthor": MOD_AUTHOR, "result.ModVersion": MOD_VERSION})
    return g.data()


def g_hub_mod_pages():
    """The page, made on the first request."""
    g = Graph()
    valid(g, "has", g.get("page0", "Page"))
    g.call("create", UMG + "WidgetBlueprintLibrary:Create", WidgetType=cls(HUBPAGE))
    g.cast("asPage", cls(HUBPAGE), "create.ReturnValue")
    g.set("setPage", "Page", value="asPage.$cast")
    g.node("arr", "makearray", n=1)
    g.link(g.get("page1", "Page"), "arr.[0]")
    g.link("arr.Array", "result.HubPages")
    g.flow("entry", "brHas", "result")
    g.flow("brHas.else", "create", "asPage", "setPage", "result")
    g.flow("asPage.CastFailed", "result")
    return g.data()


# ============================================================================= DB_HubPage

def page_tree():
    """Mod Hub draws the page inside its own window; the page keeps the game's fonts and the mod's colours."""
    selector_style = "(Normal=%s,Hovered=%s,Pressed=%s,Disabled=%s,NormalPadding=%s,PressedPadding=%s)" % (
        _btn_brush(DARK), _btn_brush((0.16, 0.08, 0.04)), _btn_brush(ORANGE), _btn_brush(DARK), margin(12, 4, 12, 4),
        margin(12, 4, 12, 4))
    selector = w("InputKeySelector", "KeyPick", True, {
        "WidgetStyle": selector_style,
        "TextStyle": "(Font=%s,ColorAndOpacity=%s)" % (font(RIFT, 24), slate(LIGHT_TEXT)),
        "KeySelectionText": "PRESS A KEY", "NoKeySpecifiedText": "NONE",
        "bAllowModifierKeys": "False", "bAllowGamepadKeys": "False",
        "Margin": margin(12, 4, 12, 4),
    })
    row = w("HorizontalBox", "KeyRow", slot=vslot(pad=(0, 18, 0, 0)), children=[
        box("KeyLabelBox", 240, slot=hslot(), child=text("KeyLabel", "OPEN THE WINDOW", CARBON, 16, LIGHT_TEXT)),
        box("KeyPickBox", 220, 48, slot=hslot(), child=selector),
        game_button("ResetBtn", "RESET TO " + DEFAULT_KEY, 20, primary=False, height=48,
                    slot=hslot(pad=(12, 0, 0, 0))),
    ])
    rows = [
        text("Title", MOD_NAME.upper(), RIFT, 34, ORANGE, slot=vslot()),
        text("Sub", "Your team's builds in game. Changes here are saved at once.", BODY, 18, LIGHT_TEXT,
             slot=vslot(pad=(0, 2, 0, 0))),
        row,
        text("KeyNote", HINT, BODY, 18, LIGHT_TEXT, var=True, wrap=True, slot=vslot(pad=(240, 10, 0, 0))),
    ]
    return w("ScrollBox", "Root", children=[
        w("VerticalBox", "Rows", slot={"Padding": margin(24, 20, 24, 20)}, children=rows)])


def g_page_find_data():
    """Data = the level's DB_Data (DB_Mod spawns it at BeginPlay)."""
    g = Graph()
    g.call("find", GS + ":GetActorOfClass", ActorClass=cls(DATA))
    g.set("set", "Data", value="find.ReturnValue")
    g.flow("entry", "find", "set")
    return g.data()


def g_page_show_key():
    """The selector shows the saved key."""
    g = Graph()
    g.call("find", "FindData")
    data = g.get("data", "Data")
    valid(g, "has", data)
    g.call("key", cls(DATA) + ":OpenKey", target=ref(data))
    g.node("chord", "makestruct", struct=CHORD)
    g.link("key.Key", "chord.Key")
    wcall(g, "show", "KeyPick", "InputKeySelector:SetSelectedKey", InSelectedKey=ref("chord.InputChord"))
    g.flow("entry", "find", "brHas", "key", "show")
    return g.data()


def g_page_note():
    """The answer line: Text in Colour. A new text each time, so a repeated answer is still seen to change."""
    g = Graph()
    set_text(g, "clear", "KeyNote", " ")
    g.call("txt", KTXT + ":Conv_StringToText", InString=ref("entry.Note"))
    wcall(g, "set", "KeyNote", "TextBlock:SetText", InText=ref("txt.ReturnValue"))
    text_colour(g, "tint", "KeyNote", "entry.Colour")
    g.flow("entry", "clear", "set", "tint")
    return g.data()


def g_page_events():
    g = Graph()
    # Opened: the saved key and the hint.
    g.node("opened", "event", name="HubPageOpened", **{"class": iface("IHubPageWidget")})
    g.call("showOpen", "ShowKey")
    g.call("hintOpen", "Note", Note=HINT, Colour="(R=0.85,G=0.76,B=0.55,A=1.0)")
    g.flow("opened", "showOpen", "hintOpen")

    # A key was picked: refuse mouse buttons, Escape, E and nothing; else save it.
    g.node("picked", "boundevent", widget="KeyPick", name="OnKeySelected")
    g.node("chord", "breakstruct", struct=CHORD)
    g.link("picked.SelectedKey", "chord.InputChord")
    key = "chord.Key"
    g.call("isValid", INPUTLIB + ":Key_IsValid", Key=ref(key))
    g.call("isMouse", INPUTLIB + ":Key_IsMouseButton", Key=ref(key))
    g.call("isEsc", INPUTLIB + ":EqualEqual_KeyKey", A=ref(key), B="Escape")
    g.call("isE", INPUTLIB + ":EqualEqual_KeyKey", A=ref(key), B="E")
    used = kml(g, "used", "BooleanOR", A=ref("isEsc.ReturnValue"), B=ref("isE.ReturnValue"))
    bad = kml(g, "bad", "BooleanOR", A=ref("isMouse.ReturnValue"), B=ref(used))
    bad_or_none = kml(g, "badOrNone", "BooleanOR", A=ref(bad),
                      B=ref(kml(g, "none", "Not_PreBool", A=ref("isValid.ReturnValue"))))
    g.branch("brBad", bad_or_none)
    g.call("why", KML + ":SelectString", A="Mouse buttons cannot open the window. Pick a keyboard key.",
           B="Escape and E are used inside the window. Pick another key.", bPickA=ref("isMouse.ReturnValue"))
    g.call("refuse", "Note", Note=ref("why.ReturnValue"), Colour="(R=1.0,G=0.25,B=0.15,A=1.0)")
    g.call("back", "ShowKey")
    g.flow("picked", "brBad", "refuse", "back")
    g.call("find", "FindData")
    data = g.get("data", "Data")
    valid(g, "has", data)
    g.call("save", cls(DATA) + ":SetOpenKey", target=ref(data), Key=ref(key))
    g.call("name", INPUTLIB + ":Key_GetDisplayName", Key=ref(key))
    g.call("nameStr", KTXT + ":Conv_TextToString", InText=ref("name.ReturnValue"))
    g.call("msg", KSTR + ":Concat_StrStr", A="Saved. Press ", B=ref("nameStr.ReturnValue"))
    g.call("msg2", KSTR + ":Concat_StrStr", A=ref("msg.ReturnValue"), B=" to open the window.")
    g.call("ok", "Note", Note=ref("msg2.ReturnValue"), Colour="(R=0.33,G=0.89,B=0.0,A=1.0)")
    g.flow("brBad.else", "find", "brHas", "save", "ok")

    # RESET TO B.
    g.node("reset", "boundevent", widget="ResetBtn", name="OnClicked")
    g.call("findR", "FindData")
    data_r = g.get("dataR", "Data")
    valid(g, "hasR", data_r)
    g.call("saveR", cls(DATA) + ":SetOpenKey", target=ref(data_r), Key=DEFAULT_KEY)
    g.call("showR", "ShowKey")
    g.call("okR", "Note", Note="Reset. Press %s to open the window." % DEFAULT_KEY,
           Colour="(R=0.33,G=0.89,B=0.0,A=1.0)")
    g.flow("reset", "findR", "brHasR", "saveR", "showR", "okR")
    press_feedback(g, "ResetBtn")
    return g.data()


def g_page_info():
    g = Graph()
    g.flow("entry", "result")
    g.defaults["result.PageName"] = MOD_NAME
    return g.data()


def g_page_mode():
    g = Graph()
    g.flow("entry", "result")
    g.defaults["result.bStretchToContainer"] = "true"
    return g.data()


# ============================================================================= module parts

def trees():
    return {HUBPAGE: page_tree()}


def members():
    return {
        HUBMOD: {"variables": [t("Page", "object", cls(HUBPAGE))], "functions": []},
        HUBPAGE: {"variables": [t("Data", "object", cls(DATA))],
                  "functions": [{"name": "FindData"}, {"name": "ShowKey"},
                                {"name": "Note", "inputs": [t("Note", "string"),
                                                            t("Colour", "struct", "/Script/CoreUObject.LinearColor")]}]},
    }


def graphs():
    return {
        HUBMOD: {"GetModInfo": g_hub_mod_info(), "GetModPages": g_hub_mod_pages()},
        HUBPAGE: {"FindData": g_page_find_data(), "ShowKey": g_page_show_key(), "Note": g_page_note(),
                  "EventGraph": g_page_events(), "GetPageInfo": g_page_info(), "GetContainerMode": g_page_mode()},
    }
