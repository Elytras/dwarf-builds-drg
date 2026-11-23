// DB_HubPage: the OPEN button and the Mod Hub page interface.
#include "../DwarfBuilds.h"

namespace UI {

void DB_HubPage::OpenReleased() {
    OpenBtnBarL->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    OpenBtnBarR->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    OpenBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.85f, 0.76f, 0.55f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
}

void DB_HubPage::OpenPressed() {
    OpenBtnBarL->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    OpenBtnBarR->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    OpenBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
}

// The window opens over the hub; closing it shows the hub again. No DB_Mod in this world (it is spawned by the game
// on the Space Rig and in missions): the hint says so.
void DB_HubPage::OpenClicked() {
    DB_Mod* Mod = (DB_Mod*)UGameplayStatics::GetActorOfClass(this, DB_Mod::StaticClass());
    if (!UKismetSystemLibrary::IsValid(Mod)) {
        Hint->SetText("Dwarf Builds is not running here. It opens on the Space Rig and in missions.");
        return;
    }
    Mod->OpenWindow();
}

void DB_HubPage::HubPageOpened() {
    Hint->SetText("Escape or the X closes the window again.");
}

void DB_HubPage::HubPageClosed() {
}

void DB_HubPage::GetPageInfo(FText& PageName) {
    PageName = "Dwarf Builds";
}

void DB_HubPage::GetContainerMode(bool& bStretchToContainer) {
    bStretchToContainer = true;
}

}   // namespace UI
