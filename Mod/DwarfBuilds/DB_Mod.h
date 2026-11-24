// The mod's actor. The game spawns InitSpacerig on the Space Rig and InitCave in a mission; both are this class.
// BeginPlay spawns the world's DB_Data (and, on the host, the replicated perk table DB_PerkShare_v1, and DB_Hub
// when Elytras Mod Manager is installed) and hooks the overlay onto the class selection screen. The builds are
// a page in EMM's hub (DB_Window).
#pragma once
#include "DwarfBuilds.h"

class DB_Mod : public AActor {
public:
    // The overlay on the class selection screen, made once per level, and that screen's classes (loaded once).
    DB_Select* SelectUI;
    TSubclassOf<UUserWidget> ScreenClass;
    TSubclassOf<UUserWidget> AnchorClass;
    bool ClassesTried;
    UUserWidget* ScreenTmp;
    UCanvasPanel* CanvasTmp;

    // SelectPoll looks for the screen until it finds it, or until this time: 60 s after BeginPlay or after the
    // game's selection start, 3 s after a game window opens. Once the overlay is attached the overlay refreshes
    // itself (DB_Select::Tick).
    float PollUntil;

    void ReceiveBeginPlay();
    void ReceiveEndPlay(TEnum<EEndPlayReason> EndPlayReason);

    void OnSelectionStart(ECharselectionCameraLocation selectionLocation);
    void OnSelectionStop();
    void OnWindowOpened();
    void StartPoll(float Seconds);
    void SelectPoll();
    void TryShow();
    void FindScreen();
};
