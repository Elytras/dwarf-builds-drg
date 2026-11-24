// DB_Window: Dwarf Builds' hub page. The builds come from DB_Data (DB_Column draws one player's); this file is the
// page around them: its tabs, filters and code tools, the hub's look, and the once-a-second refresh.
#include "../DwarfBuilds.h"

namespace UI {

// --- The hub page ---------------------------------------------------------------------------------------------------

// Each showing finds this world's DB_Data (DB_Mod spawns it on the Space Rig and in missions) and opens the page on
// it, so the page shows every time the hub shows it. Without DB_Data the count line says why it is empty.
void DB_Window::HubPageOpened() {
    HubShown = true;
    Data = (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass());
    if (!UKismetSystemLibrary::IsValid(Data)) {
        CountText->SetText("Dwarf Builds is not running here. It works on the Space Rig and in missions.");
        return;
    }
    Open();
}

// The refresh stops while the page is not shown; tooltips go with it.
void DB_Window::HubPageClosed() {
    HubShown = false;
    Stop();
    HideAllTips();
}

void DB_Window::GetPageInfo(FText& PageName) {
    PageName = "Dwarf Builds";
}

// The page fills the pane: the columns scale to its height and scroll sideways.
void DB_Window::GetContainerMode(bool& bStretchToContainer) {
    bStretchToContainer = true;
}

/* The hub calls this before it shows the page, with its look. The code box takes the hub's field style, and its text
   colour (the palette's Text) is the page's labels' colour; the labels take the hub's font. Columns, the preview and
   the take dialog keep their game icons and class colours, in the hub's font, fields and buttons (StyleColumn). */
void DB_Window::ApplyLook(Widget_C* Service) {
    Look = Service;
    Look->StyleTextBox(CodeBox);
    FLinearColor Ink = CodeBox->WidgetStyle.ForegroundColor.SpecifiedColor;
    FSlateColor InkColor = FSlateColor(Ink, ESlateColorStylingMode::UseColor_Specified);
    Look->ApplyFont(Body);
    CountText->SetColorAndOpacity(InkColor);
    CodeHint->SetColorAndOpacity(InkColor);
    CopyNote->SetColorAndOpacity(InkColor);
    if (!CodeValid && CodeText == "") CodeStatus->SetColorAndOpacity(InkColor);
    Look->StyleScrollBox(TeamScroll, FLinearColor(Ink.R, Ink.G, Ink.B, 0.5f));
    for (DB_Column* Column : Columns) StyleColumn(Column);
    StyleColumn(Preview);
    Look->StyleField(Take->Body);
    Look->RestyleForeign(Take->SaveBtn);
    Look->RestyleForeign(Take->CancelBtn);
    Take->SaveBtnBarL->SetVisibility(ESlateVisibility::Collapsed);
    Take->SaveBtnBarR->SetVisibility(ESlateVisibility::Collapsed);
    Take->CancelBtnBarL->SetVisibility(ESlateVisibility::Collapsed);
    Take->CancelBtnBarR->SetVisibility(ESlateVisibility::Collapsed);
    Look->RestyleForeign(Take->Root);   // its labels' font (and its check box); its buttons are flat already
}

// One column in the hub's look: its panels as the hub's fields, its two buttons flat in the palette (their bars
// gone), every label in the hub's font at its own size and colour.
void DB_Window::StyleColumn(DB_Column* Column) {
    if (Look == nullptr || Column == nullptr) return;
    Look->StyleField(Column->HeadBg);
    Look->StyleField(Column->PerkBg);
    Look->StyleField(Column->Card1->Bg);
    Look->StyleField(Column->Card2->Bg);
    Look->StyleField(Column->Gear1->Bg);
    Look->StyleField(Column->Gear2->Bg);
    Look->StyleField(Column->Gear3->Bg);
    Look->StyleField(Column->Gear4->Bg);
    Look->RestyleForeign(Column->TakeBtn);
    Look->RestyleForeign(Column->CopyBtn);
    Column->TakeBtnBarL->SetVisibility(ESlateVisibility::Collapsed);
    Column->TakeBtnBarR->SetVisibility(ESlateVisibility::Collapsed);
    Column->CopyBtnBarL->SetVisibility(ESlateVisibility::Collapsed);
    Column->CopyBtnBarR->SetVisibility(ESlateVisibility::Collapsed);
    Look->ApplyFont(Column->Root);
    Look->ApplyFont(Column->Card1->Root);
    Look->ApplyFont(Column->Card2->Root);
    Look->ApplyFont(Column->Gear1->Root);
    Look->ApplyFont(Column->Gear2->Root);
    Look->ApplyFont(Column->Gear3->Root);
    Look->ApplyFont(Column->Gear4->Root);
}

// --- Opening and refreshing -----------------------------------------------------------------------------------------

void DB_Window::Open() {
    Preview->Window = this;
    Preview->Data = Data;
    Take->Window = this;
    Take->Data = Data;
    for (DB_Column* Column : Columns) Column->Data = Data;
    if (!Inited) {
        InitOnce();
        Inited = true;
    }
    CloseTake();
    Refresh();
    UKismetSystemLibrary::K2_ClearTimer(this, "TimerRefresh");
    UKismetSystemLibrary::K2_SetTimer(this, "TimerRefresh", 1.0f, true, 0.0f, 0.0f);
    SetKeyboardFocus();
}

void DB_Window::InitOnce() {
    Preview->Clear();
    UpdateCode("");
}

void DB_Window::Refresh() {
    if (!UKismetSystemLibrary::IsValid(Data)) return;
    Collect();
    ShowPage();
    UpdateHeader();
}

void DB_Window::TimerRefresh() {
    bool bOpen;
    IsOpenNow(bOpen);
    if (bOpen) {
        Refresh();
    } else {
        HideAllTips();
        Stop();
    }
}

void DB_Window::IsOpenNow(bool& bOpen) {
    bOpen = HubShown;
}

void DB_Window::Stop() {
    UKismetSystemLibrary::K2_ClearTimer(this, "TimerRefresh");
}

void DB_Window::HideAllTips() {
    if (UKismetSystemLibrary::IsValid(Data)) Data->HideTips();
}

// The players, each one's class, how many of each, and the ones the filter lets through. Four players or fewer show
// all, so the filter goes back to everyone.
void DB_Window::Collect() {
    Data->GetPlayers(Players);
    PlayerClass.Clear();
    ClassCounts.Clear();
    for (int32 I = 0; I < 5; I++) ClassCounts.Add(0);
    ClassCounts[0] = Players.Num();
    if (Players.Num() <= 4) Filter = 0;
    for (AFSDPlayerState* Player : Players) {
        int32 Class;
        ClassIndex(Player, Class);
        PlayerClass.Add(Class);
        if (Class > 0) ClassCounts[Class] = ClassCounts[Class] + 1;
    }
    Shown.Clear();
    for (int32 I = 0; I < PlayerClass.Num(); I++)
        if (Filter == 0 || PlayerClass[I] == Filter) Shown.Add(I);
}

// The class by the first letter of its ID's name: D(riller) E(ngineer) G(unner) S(cout) give 1-4, anything else 0.
void DB_Window::ClassIndex(AFSDPlayerState* Player, int32& Index) {
    UPlayerCharacterID* ID = Player->GetSelectedCharacterID();
    Index = UKismetStringLibrary::FindSubstring("DEGS", UKismetStringLibrary::GetSubstring(UKismetSystemLibrary::GetObjectName(ID), 0, 1), true, false, -1) + 1;
}

// One column per shown player, made the first time it is needed; columns past them are cleared and hidden. A column
// skips its redraw when its player looks as it last drew them (DB_Column::SameAsLast).
void DB_Window::ShowPage() {
    if (Tab != 0) return;
    for (int32 I = 0; I < Shown.Num(); I++) {
        DB_Column* Column = ColumnAt(I);
        Column->ShowPlayer(Players[Shown[I]]);
        Column->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
    for (int32 I = Shown.Num(); I < Columns.Num(); I++) {
        Columns[I]->Clear();
        Columns[I]->SetVisibility(ESlateVisibility::Collapsed);
    }
}

DB_Column* DB_Window::ColumnAt(int32 Index) {
    while (Columns.Num() <= Index) {
        DB_Column* Column = Cast<DB_Column>(UWidgetBlueprintLibrary::Create(this, DB_Column::StaticClass(), GetOwningPlayer()));
        Column->Window = this;
        Column->Data = Data;
        TeamWrap->AddChildToWrapBox(Column);
        StyleColumn(Column);
        Columns.Add(Column);
    }
    return Columns[Index];
}

// The count, the tabs and filters as selected, and the filter buttons' counts. A button's text or selection is set
// only when it changes: setting it measures the button again.
void DB_Window::UpdateHeader() {
    FString Count = UKismetStringLibrary::Conv_IntToString(Players.Num()) + (Players.Num() == 1 ? " player" : " players");
    CountText->SetText(UKismetTextLibrary::Conv_StringToText(Count));
    bool bFilters = Tab == 0 && Players.Num() > 4;
    Filters->SetVisibility(bFilters ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    TArray<UBasicButton*> Buttons = {Filter0, Filter1, Filter2, Filter3, Filter4};
    TArray<FString> Names = {"All", "Driller", "Engineer", "Gunner", "Scout"};
    for (int32 I = 0; I < 5; I++) {
        FString Label = Names[I] + " " + UKismetStringLibrary::Conv_IntToString(ClassCounts[I]);
        if (Buttons[I]->TextString != Label) Buttons[I]->SetText(Label);
        if (Buttons[I]->bSelected != (Filter == I)) Buttons[I]->SetSelected(Filter == I);
    }
    if (TabTeam->bSelected != (Tab == 0)) TabTeam->SetSelected(Tab == 0);
    if (TabCode->bSelected != (Tab == 1)) TabCode->SetSelected(Tab == 1);
}

void DB_Window::SetFilter(int32 NewFilter) {
    Filter = NewFilter;
    Refresh();
}

void DB_Window::SetTab(int32 NewTab) {
    Tab = NewTab;
    Pages->SetActiveWidgetIndex(NewTab);
    HideAllTips();
    Refresh();
}

// --- Buttons --------------------------------------------------------------------------------------------------------

void DB_Window::OnButton(UBasicButton* Button) {
    if (Button == TabTeam) SetTab(0);
    else if (Button == TabCode) SetTab(1);
    else if (Button == BrowseBtn) OpenSite("https://dwarfbuilds.pages.dev");
    else if (Button == Filter0) SetFilter(0);
    else if (Button == Filter1) SetFilter(1);
    else if (Button == Filter2) SetFilter(2);
    else if (Button == Filter3) SetFilter(3);
    else if (Button == Filter4) SetFilter(4);
    else if (Button == PasteBtn) PasteCode();
    else if (Button == TakeCodeBtn) {
        if (CodeValid) OpenTake(CodeText, "PASTED BUILD");
    } else if (Button == CopyMineBtn) CopyMine();
    else if (Button == PublishBtn) PublishMine();
    else if (Button == SiteBtn) UFrontendBlueprintLibrary::OpenURLInExternalBrowser("https://dwarfbuilds.pages.dev");
}

// --- The take dialog ------------------------------------------------------------------------------------------------

void DB_Window::OpenTake(FString NewCode, FString Title) {
    Take->Show(NewCode, Title);
    Take->SetVisibility(ESlateVisibility::Visible);
    TakeOpen = true;
}

void DB_Window::CloseTake() {
    Take->SetVisibility(ESlateVisibility::Collapsed);
    TakeOpen = false;
    SetKeyboardFocus();
}

// --- Build codes ----------------------------------------------------------------------------------------------------

void DB_Window::OnCodeChanged(FText Text) {
    UpdateCode(UKismetTextLibrary::Conv_TextToString(Text));
}

// A code typed or pasted: valid, it shows in the preview and can be taken; else the status says what is wrong.
void DB_Window::UpdateCode(FString NewText) {
    if (!UKismetSystemLibrary::IsValid(Data)) return;
    FString Code = UKismetStringLibrary::TrimTrailing(UKismetStringLibrary::Trim(NewText));
    if (Inited && Code == CodeText) return;
    CodeText = Code;
    UPlayerCharacterID* Class;
    Data->ClassOfCode(CodeText, Class, CodeValid);
    if (CodeValid) {
        Preview->ShowCode(CodeText, "PASTED BUILD");
        FString Sub;
        FLinearColor Color;
        UTexture2D* Icon;
        Data->HeaderOfCode(CodeText, Sub, Color, Icon);
        CodeStatus->SetText(UKismetTextLibrary::Conv_StringToText("Valid code: " + Sub));
        CodeStatus->SetColorAndOpacity(FSlateColor(FLinearColor(0.19807f, 0.58408f, 0.15896f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    } else {
        Preview->Clear();
        bool Empty = UKismetStringLibrary::isEmpty(CodeText);
        CodeStatus->SetText(Empty ? FText() : FText("Not a Dwarf Builds code"));
        if (!Empty)
            CodeStatus->SetColorAndOpacity(FSlateColor(FLinearColor(0.87137f, 0.16203f, 0.10702f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    }
    TakeCodeBtn->SetIsEnabled(CodeValid);
}

void DB_Window::PasteCode() {
    FString Clip;
    UFrontendBlueprintLibrary::GetStringFromClipboard(Clip);
    FString Code = UKismetStringLibrary::TrimTrailing(UKismetStringLibrary::Trim(Clip));
    CodeBox->SetText(UKismetTextLibrary::Conv_StringToText(Code));
    UpdateCode(Code);
}

void DB_Window::CopyMine() {
    if (!UKismetSystemLibrary::IsValid(Data)) return;
    FString Code;
    Data->LocalCode(Code);
    if (UKismetStringLibrary::isEmpty(Code)) {
        CopyNote->SetText("You have no build yet.");
        return;
    }
    UFrontendBlueprintLibrary::CopyStringToClipboard(Code);
    CopyNote->SetText("Your build code is in the clipboard.");
}

void DB_Window::PublishMine() {
    if (!UKismetSystemLibrary::IsValid(Data)) return;
    FString Code;
    Data->LocalCode(Code);
    if (UKismetStringLibrary::isEmpty(Code)) {
        CopyNote->SetText("You have no build yet.");
        return;
    }
    OpenSite("https://dwarfbuilds.pages.dev/new?code=" + Code);
}

void DB_Window::OpenSite(FString URL) {
    UFSDOnlineSystemUtils::OpenURLInOverlay(URL, {this, &DB_Window::OnOverlayClosed});
}

void DB_Window::OnOverlayClosed(FString LastURL) {
    SetKeyboardFocus();
}

// --- Focus and keys -------------------------------------------------------------------------------------------------

void DB_Window::OnRemovedFromFocusPath(FFocusEvent InFocusEvent) {
    HideAllTips();
}

void DB_Window::OnFocusLost(FFocusEvent InFocusEvent) {
    HideAllTips();
}

void DB_Window::Destruct() {
    HideAllTips();
    Stop();
}

// Escape closes the take dialog, else goes on to the hub; E flips "more info" unless the code box is being typed in.
FEventReply DB_Window::OnKeyUp(FGeometry MyGeometry, FKeyEvent InKeyEvent) {
    FKey Key = UKismetInputLibrary::GetKey(InKeyEvent);
    bool Typing = CodeBox->HasKeyboardFocus();
    if (Key == FKey("Escape")) {
        if (!TakeOpen) return UWidgetBlueprintLibrary::Unhandled();
        CloseTake();
        return UWidgetBlueprintLibrary::Handled();
    }
    if (Key == FKey("E") && !Typing && UKismetSystemLibrary::IsValid(Data)) {
        Data->ToggleMoreInfo();
        return UWidgetBlueprintLibrary::Handled();
    }
    return Typing ? UWidgetBlueprintLibrary::Handled() : UWidgetBlueprintLibrary::Unhandled();
}

FEventReply DB_Window::OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent) {
    if (CodeBox->HasKeyboardFocus() || UKismetInputLibrary::GetKey(InKeyEvent) == FKey("E"))
        return UWidgetBlueprintLibrary::Handled();
    return UWidgetBlueprintLibrary::Unhandled();
}

FEventReply DB_Window::OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent) {
    SetKeyboardFocus();
    return UWidgetBlueprintLibrary::Unhandled();   // the hub's frame may want it (a drag that moves the window)
}

}   // namespace UI
