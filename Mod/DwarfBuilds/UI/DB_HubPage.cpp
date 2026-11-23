// DB_HubPage: the builds window inside the page, and the Mod Hub page interface.
#include "../DwarfBuilds.h"

namespace UI {

// First we make the window, once, filling the page. Each showing hands it this world's DB_Data and opens it, which
// starts its once-a-second refresh; with no DB_Data (DB_Mod spawns it on the Space Rig and in missions) the hint says
// why the page is empty.
void DB_HubPage::HubPageOpened() {
    DB_Data* Data = (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass());
    bool Running = UKismetSystemLibrary::IsValid(Data);
    Hint->SetVisibility(Running ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
    if (!Running) return;
    if (Win == nullptr) {
        Win = Cast<DB_Window>(UWidgetBlueprintLibrary::Create(this, DB_Window::StaticClass(), GetOwningPlayer()));
        if (Win == nullptr) return;
        UOverlaySlot* WinSlot = Root->AddChildToOverlay(Win);
        WinSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
        WinSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
    }
    Win->HubShown = true;
    Win->Data = Data;
    Win->Open();
}

// The refresh stops while the page is not shown; tooltips go with it.
void DB_HubPage::HubPageClosed() {
    if (Win == nullptr) return;
    Win->HubShown = false;
    Win->Stop();
    Win->HideAllTips();
}

void DB_HubPage::GetPageInfo(FText& PageName) {
    PageName = "Dwarf Builds";
}

void DB_HubPage::GetContainerMode(bool& bStretchToContainer) {
    bStretchToContainer = true;
}

}   // namespace UI
