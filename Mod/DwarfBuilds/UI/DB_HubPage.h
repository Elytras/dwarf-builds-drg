// Dwarf Builds' page in a Mod Hub (Mod Hub, Elytras Mod Manager): the builds window itself (DB_Window), shown in the
// page while it is open, as Build Inspector's page shows its builds. The window has no key of its own and no window of
// its own any more; this page is where it lives.
#pragma once
#include "../DwarfBuilds.h"
namespace UI {
class DB_HubPage : public UUserWidget, public ::IHubPageWidget {
public:
    UE_CLASS("/Game/_DwarfBuilds/UI/DB_HubPage", "DB_HubPage_C");
    UE_WIDGET(UOverlay, Root);
    UE_WIDGET(UTextBlock, Hint);
    DB_Window* Win;
    DB_HubPage() {
        UOverlaySlot *Hint_Slot = Cast<UOverlaySlot>(Root->AddChild(Hint));
        Hint_Slot->Padding.Left = 24.0f;
        Hint_Slot->Padding.Top = 20.0f;
        Hint->Text = "Dwarf Builds is not running here. It shows on the Space Rig and in missions.";
        Hint->ColorAndOpacity.SpecifiedColor = FLinearColor(0.85f, 0.76f, 0.55f, 1.0f);
        Hint->Font.FontObject = &Assets::Art::Fonts::Acumin_Pro_Condensed;
        Hint->Font.TypefaceFontName = "Condensed Normal";
        Hint->Font.Size = 18;
        Hint->AutoWrapText = true;
        Hint->Visibility = ESlateVisibility::Collapsed;
    }
    void HubPageOpened();
    void HubPageClosed();
    void GetPageInfo(FText& PageName);
    void GetContainerMode(bool& bStretchToContainer);
};
}   // namespace UI
