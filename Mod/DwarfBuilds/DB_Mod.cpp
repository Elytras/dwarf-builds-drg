// DB_Mod: spawning the mod's actors, opening the window, and finding the class selection screen.
#include "DwarfBuilds.h"

namespace {
const float PollSeconds = 0.5f;
const float PollWindow = 60.0f;   // how long SelectPoll looks after BeginPlay or a selection start,
const float OpenWindowFor = 3.0f; // and after a game window opens (the selection screen opens like one)
const float StopCheck = 2.0f;     // one look this long after a selection stop: a mission's screen is gone by then
const int32 WindowLayer = 60;
}

// First we keep one DB_Mod per world: InitSpacerig and InitCave can both be spawned, and a mod list may hold the
// pak twice. Then the world's actors, then the selection events once the local player controller exists.
void DB_Mod::ReceiveBeginPlay() {
    TArray<AActor*> Mods;
    UGameplayStatics::GetAllActorsOfClass(this, DB_Mod::StaticClass(), Mods);
    if (Mods.Num() > 1 && Mods[0] != this) {
        K2_DestroyActor();
        return;
    }

    // DB_Hub implements Mod Hub's interfaces, so it is loaded by path only when those are there.
    if (UKismetSystemLibrary::IsValidClass(UKismetSystemLibrary::LoadClassAsset_Blocking(
            UKismetSystemLibrary::MakeSoftClassPath("/Game/_ModHub/IHubMod.IHubMod_C")))) {
        UClass* Hub = UKismetSystemLibrary::LoadClassAsset_Blocking(
            UKismetSystemLibrary::MakeSoftClassPath("/Game/_DwarfBuilds/DB_Hub.DB_Hub_C"));
        if (UKismetMathLibrary::ClassIsChildOf(Hub, AActor::StaticClass())) {
            FTransform At = UKismetMathLibrary::MakeTransform(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f),
                                                              FVector(1.0f, 1.0f, 1.0f));
            UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(
                this, TSubclassOf<AActor>(Hub), At, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this), At);
        }
    }
    FTransform Origin = UKismetMathLibrary::MakeTransform(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f),
                                                          FVector(1.0f, 1.0f, 1.0f));
    if (!UKismetSystemLibrary::IsValid(UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass())))
        UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(
            this, DB_Data::StaticClass(), Origin, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this), Origin);
    // The perk table replicates from the host; a client reads the host's.
    if (UKismetSystemLibrary::IsServer(this) &&
        !UKismetSystemLibrary::IsValid(UGameplayStatics::GetActorOfClass(this, DB_PerkShare_v1::StaticClass())))
        UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(
            this, DB_PerkShare_v1::StaticClass(), Origin, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, nullptr),
            Origin);

    while (!UKismetSystemLibrary::IsValid(UGameplayStatics::GetPlayerController(this, 0)))
        UKismetSystemLibrary::Delay(this, 0.5f);
    UFSDGameInstance* GI = UGameFunctionLibrary::GetFSDGameInstance(this);
    if (UKismetSystemLibrary::IsValid(GI)) {
        GI->OnCharacterSelectionStart.Add(this, &DB_Mod::OnSelectionStart);
        GI->OnCharacterSelectionStop.Add(this, &DB_Mod::OnSelectionStop);
    }
    UWindowManager* WM = UGameFunctionLibrary::GetWindowManager(this);
    if (UKismetSystemLibrary::IsValid(WM)) WM->OnFirstWindowOpened.Add(this, &DB_Mod::OnWindowOpened);
    StartPoll(PollWindow);
}

// The game instance outlives the level: unbind from it, and leave the screen so the old one can be freed.
void DB_Mod::ReceiveEndPlay(TEnum<EEndPlayReason> EndPlayReason) {
    UFSDGameInstance* GI = UGameFunctionLibrary::GetFSDGameInstance(this);
    if (UKismetSystemLibrary::IsValid(GI)) {
        GI->OnCharacterSelectionStart.Remove(this, &DB_Mod::OnSelectionStart);
        GI->OnCharacterSelectionStop.Remove(this, &DB_Mod::OnSelectionStop);
    }
    UWindowManager* WM = UGameFunctionLibrary::GetWindowManager(this);
    if (UKismetSystemLibrary::IsValid(WM)) WM->OnFirstWindowOpened.Remove(this, &DB_Mod::OnWindowOpened);
    UKismetSystemLibrary::K2_ClearTimer(this, "SelectPoll");
    if (UKismetSystemLibrary::IsValid(SelectUI)) SelectUI->Detach();
}

// From the hub page's OPEN button. Through the game's window manager when it has one: the hub is a window of its own
// there, so ours goes on top and closing ours shows the hub again. Else by hand. Open already: nothing.
void DB_Mod::OpenWindow() {
    DB_Data* Data = (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass());
    TArray<UUserWidget*> Shown;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Shown, DB_Window::StaticClass(), true);
    if (Shown.Num() > 0) return;
    UWindowManager* WM = UGameFunctionLibrary::GetWindowManager(this);
    if (UKismetSystemLibrary::IsValid(WM)) {
        DB_Window* Win = Cast<DB_Window>(WM->OpenWindowFromClass(DB_Window::StaticClass(), WindowLayer, true));
        if (Win != nullptr) {
            Win->Manual = false;
            Win->Data = Data;
            Win->Open();
            return;
        }
    }
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    DB_Window* Win = Cast<DB_Window>(UWidgetBlueprintLibrary::Create(this, DB_Window::StaticClass(), PC));
    if (Win == nullptr) return;
    Win->Manual = true;
    Win->AddToViewport(WindowLayer);
    Win->Data = Data;
    UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, Win, EMouseLockMode::DoNotLock);
    PC->bShowMouseCursor = true;
    Win->Open();
}

// Only the selection proper (not the forge, the customization or the end screen) shows the class selection screen.
void DB_Mod::OnSelectionStart(ECharselectionCameraLocation selectionLocation) {
    if (selectionLocation != ECharselectionCameraLocation::Selection) return;
    StartPoll(PollWindow);
}

// The overlay stays with the screen. With no search running, one look a moment later lets go of a mission's screen,
// which has left the UI by then.
void DB_Mod::OnSelectionStop() {
    if (UKismetSystemLibrary::IsValid(SelectUI)) SelectUI->OnStop();
    if (!UKismetSystemLibrary::K2_IsTimerActive(this, "SelectPoll"))
        UKismetSystemLibrary::K2_SetTimer(this, "SelectPoll", StopCheck, false, 0.0f, 0.0f);
}

void DB_Mod::OnWindowOpened() {
    StartPoll(OpenWindowFor);
}

// A longer search that is already running keeps its end.
void DB_Mod::StartPoll(float Seconds) {
    PollUntil = UKismetMathLibrary::FMax(PollUntil, UGameplayStatics::GetRealTimeSeconds(this) + Seconds);
    UKismetSystemLibrary::K2_ClearTimer(this, "SelectPoll");
    UKismetSystemLibrary::K2_SetTimer(this, "SelectPoll", PollSeconds, true, 0.0f, 0.0f);
    SelectPoll();
}

// The poll stops as soon as the overlay sits in a live screen's canvas, or when its window is over. In the fallback
// (the overlay in the viewport, not in the screen) it goes on while attached: the overlay's visibility then follows
// the screen's through this poll, since a collapsed overlay does not tick.
void DB_Mod::SelectPoll() {
    TryShow();
    bool Up = false;
    if (UKismetSystemLibrary::IsValid(SelectUI)) SelectUI->IsAttached(Up);
    bool Fallback = Up && !UKismetSystemLibrary::IsValid(SelectUI->Canvas);
    if ((Up && !Fallback) || (!Up && UGameplayStatics::GetRealTimeSeconds(this) >= PollUntil))
        UKismetSystemLibrary::K2_ClearTimer(this, "SelectPoll");
}

// Attaches the overlay to the class selection screen when it is not attached to a live one. The screen's classes are
// loaded by path once; they are in memory already.
void DB_Mod::TryShow() {
    if (!ClassesTried) {
        ClassesTried = true;
        UClass* Screen = UKismetSystemLibrary::LoadClassAsset_Blocking(UKismetSystemLibrary::MakeSoftClassPath(
            "/Game/UI/CharacterSelectionMK2/SCREEN_CharacterSelection_Base.SCREEN_CharacterSelection_Base_C"));
        if (UKismetMathLibrary::ClassIsChildOf(Screen, UUserWidget::StaticClass()))
            ScreenClass = TSubclassOf<UUserWidget>(Screen);
        UClass* Anchor = UKismetSystemLibrary::LoadClassAsset_Blocking(UKismetSystemLibrary::MakeSoftClassPath(
            "/Game/UI/CharacterSelectionMK2/Lobby_BarTop_Countdown.Lobby_BarTop_Countdown_C"));
        if (UKismetMathLibrary::ClassIsChildOf(Anchor, UUserWidget::StaticClass()))
            AnchorClass = TSubclassOf<UUserWidget>(Anchor);
    }
    if (UKismetSystemLibrary::IsValid(SelectUI)) {
        bool Up = false;
        SelectUI->IsAttached(Up);
        if (Up) {
            if (!UKismetSystemLibrary::IsValid(SelectUI->Canvas)) {
                bool Shown;
                bool Became;
                SelectUI->Update(Shown, Became);
            }
            return;
        }
        SelectUI->Detach();
    }
    FindScreen();
    if (!UKismetSystemLibrary::IsValid(ScreenTmp)) return;
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!UKismetSystemLibrary::IsValid(SelectUI)) {
        SelectUI = Cast<DB_Select>(UWidgetBlueprintLibrary::Create(this, DB_Select::StaticClass(), PC));
        if (SelectUI == nullptr) return;
    }
    // The viewport layer for the fallback without the screen's canvas: just above the screen.
    AFSDPlayerController* FPC = Cast<AFSDPlayerController>(PC);
    int32 Layer = FPC != nullptr && FPC->IsOnSpaceRig ? 13 : 100;
    SelectUI->AttachTo(ScreenTmp, CanvasTmp, (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass()),
                       Layer);
}

// ScreenTmp and CanvasTmp: the class selection screen and its full-screen canvas, or None. No selection screen widget
// in this world at all: nothing to find, after this one scan (the usual case while the poll waits). Else every anchor
// widget whose owner is a live selection screen (in the viewport or in a parent), its parent being the canvas; a
// shown screen wins over a hidden one. Without a canvas, a selection screen in the viewport, for the viewport
// fallback, taken from the first scan.
void DB_Mod::FindScreen() {
    ScreenTmp = nullptr;
    CanvasTmp = nullptr;
    if (!UKismetSystemLibrary::IsValidClass(ScreenClass)) return;
    TArray<UUserWidget*> Screens;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Screens, ScreenClass, false);
    if (Screens.Num() == 0) return;
    if (UKismetSystemLibrary::IsValidClass(AnchorClass)) {
        TArray<UUserWidget*> Anchors;
        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Anchors, AnchorClass, false);
        for (UUserWidget* Anchor : Anchors) {
            UObject* Owner = UKismetSystemLibrary::GetOuterObject(UKismetSystemLibrary::GetOuterObject(Anchor));
            if (!UKismetMathLibrary::ClassIsChildOf(UGameplayStatics::GetObjectClass(Owner), (UClass*)ScreenClass))
                continue;
            UUserWidget* Screen = Cast<UUserWidget>(Owner);
            if (Screen == nullptr) continue;
            bool Live = Screen->IsInViewport() || UKismetSystemLibrary::IsValid(Screen->GetParent());
            bool Better = !UKismetSystemLibrary::IsValid(ScreenTmp) || Screen->IsVisible();
            UCanvasPanel* Canvas = Cast<UCanvasPanel>(Anchor->GetParent());
            if (Live && Better && Canvas != nullptr) {
                ScreenTmp = Screen;
                CanvasTmp = Canvas;
            }
        }
    }
    if (UKismetSystemLibrary::IsValid(ScreenTmp)) return;
    for (UUserWidget* Screen : Screens)
        if (Screen->IsInViewport() && (!UKismetSystemLibrary::IsValid(ScreenTmp) || Screen->IsVisible()))
            ScreenTmp = Screen;
}
