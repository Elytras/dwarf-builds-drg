// Decompiled from /Game/_DwarfBuilds/UI/DB_UpgCell by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#pragma once
#include "../DwarfBuilds.h"
namespace UI {
class DB_UpgCell : public UUserWidget {
public:
    UE_WIDGET(UOverlay, Root);
    UE_WIDGET(UImage, HexBg);
    UE_WIDGET(UImage, HexLine);
    UE_WIDGET(UImage, Icon);
    DB_Data* Data;
    UItemUpgrade* CellUpgrade;
    bool CellSelected;
    bool HexDone;
    float IconPx = 21.0f;
    UTexture2D* IconTex;
    DB_Tip* CellTip;
    UItemUpgrade* TipFor;
    DB_UpgCell() {
        UOverlaySlot *HexBg_Slot = Cast<UOverlaySlot>(Root->AddChild(HexBg));
        HexBg_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        HexBg_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;
        HexBg->ColorAndOpacity = FLinearColor(0.0f, 0.0f, 0.0f, 0.45f);
        HexBg->Visibility = ESlateVisibility::HitTestInvisible;
        UOverlaySlot *HexLine_Slot = Cast<UOverlaySlot>(Root->AddChild(HexLine));
        HexLine_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        HexLine_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;
        HexLine->ColorAndOpacity = FLinearColor(0.104167f, 0.052518f, 0.026584f, 1.0f);
        HexLine->Visibility = ESlateVisibility::Collapsed;
        UOverlaySlot *Icon_Slot = Cast<UOverlaySlot>(Root->AddChild(Icon));
        Icon_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
        Icon_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
        Icon->Brush.ImageSize = FVector2D(21.0f, 21.0f);
        Icon->ColorAndOpacity = FLinearColor(0.00857f, 0.007f, 0.00402f, 1.0f);
        Icon->Visibility = ESlateVisibility::Collapsed;
    }
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Set(UItemUpgrade* Upgrade, bool Selected);
};
}   // namespace UI
