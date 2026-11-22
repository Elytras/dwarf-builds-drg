// The mod's actor. The game spawns InitSpacerig on the Space Rig and InitCave in a mission; both are this class.
// BeginPlay spawns the world's DB_Data (and, on the host, the replicated perk table DB_PerkShare_v1, and DB_Hub
// when Mod Hub is installed), takes the open key, and hooks the overlay onto the class selection screen.
#pragma once
#include "DwarfBuilds.h"

class DB_Mod : public AActor {
public:
    // The open key: armed by a press the game saw with no game window open, acted on at the release.
    bool KeyArmed;
    float KeyArmedAt;

    // The overlay on the class selection screen, made once per level, and that screen's classes (loaded once).
    DB_Select* SelectUI;
    TSubclassOf<UUserWidget> ScreenClass;
    TSubclassOf<UUserWidget> AnchorClass;
    bool ClassesTried;
    UUserWidget* ScreenTmp;
    UCanvasPanel* CanvasTmp;

    // SelectPoll looks for the screen until it finds it, or until this time: 60 s after BeginPlay or after the
    // game's selection start. Once the overlay is attached the overlay refreshes itself (DB_Select::Tick).
    float PollUntil;

    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnum<EEndPlayReason> EndPlayReason);

    UE_INPUT_KEY("AnyKey", Pressed, NoConsume) void KeyDown(FKey Key);
    UE_INPUT_KEY("AnyKey", Released, NoConsume) void KeyUp(FKey Key);
    void ToggleWindow();

    void OnSelectionStart(ECharselectionCameraLocation selectionLocation);
    void OnSelectionStop();
    void StartPoll();
    void SelectPoll();
    void TryShow();
    void FindScreen();
    bool IsOpenKey(FKey Key);
};
