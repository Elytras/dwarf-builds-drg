// Decompiled from /Game/_DwarfBuilds/UI/DB_PerkTile by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#pragma once
#include "../DwarfBuilds.h"
namespace UI {
class DB_PerkTile : public UUserWidget {
public:
    UE_WIDGET(USizeBox, Root);
    UE_WIDGET(UBorder, Frame);
    UE_WIDGET(UBorder, Fill);
    UE_WIDGET(UImage, Icon);
    DB_Data* Data;
    UPerkAsset* TilePerk;
    int32 TileRank;
    DB_Tip* TileTip;
    UPerkAsset* TipFor;
    int32 TipRank;
    DB_PerkTile() {
        Root->WidthOverride = 40.0f;
        Root->HeightOverride = 40.0f;
        Root->bOverride_WidthOverride = true;
        Root->bOverride_HeightOverride = true;
        Root->AddChild(Frame);
        Frame->Padding.Left = 1.0f;
        Frame->Padding.Top = 1.0f;
        Frame->Padding.Right = 1.0f;
        Frame->Padding.Bottom = 1.0f;
        Frame->BrushColor = FLinearColor(0.104167f, 0.052518f, 0.026584f, 1.0f);
        UBorderSlot *Fill_Slot = Cast<UBorderSlot>(Frame->AddChild(Fill));
        Fill_Slot->Padding.Left = 1.0f;
        Fill_Slot->Padding.Top = 1.0f;
        Fill_Slot->Padding.Right = 1.0f;
        Fill_Slot->Padding.Bottom = 1.0f;
        Fill->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
        Fill->VerticalAlignment = EVerticalAlignment::VAlign_Center;
        Fill->Padding.Left = 0.0f;
        Fill->Padding.Top = 0.0f;
        Fill->Padding.Right = 0.0f;
        Fill->Padding.Bottom = 0.0f;
        Fill->BrushColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.7f);
        UBorderSlot *Icon_Slot = Cast<UBorderSlot>(Fill->AddChild(Icon));
        Icon_Slot->Padding.Left = 0.0f;
        Icon_Slot->Padding.Top = 0.0f;
        Icon_Slot->Padding.Right = 0.0f;
        Icon_Slot->Padding.Bottom = 0.0f;
        Icon_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
        Icon_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
        Icon->Brush.ImageSize = FVector2D(28.0f, 28.0f);
        Icon->ColorAndOpacity = FLinearColor(0.00857f, 0.007f, 0.00402f, 1.0f);
        Icon->Visibility = ESlateVisibility::Collapsed;
    }
    void OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
    void Set(UPerkAsset* Perk, int32 Rank);
};
}   // namespace UI
