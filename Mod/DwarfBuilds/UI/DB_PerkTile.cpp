// Decompiled from /Game/_DwarfBuilds/UI/DB_PerkTile by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_PerkTile::OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent) {
    bool CallFunc_BooleanAND_ReturnValue;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_MoreInfo_On;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue;
    FText CallFunc_PerkTip_Body;
    FText CallFunc_PerkTip_Category;
    FLinearColor CallFunc_PerkTip_Color;
    UTexture2D* CallFunc_PerkTip_Icon;
    FText CallFunc_PerkTip_Title;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(TilePerk), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_NotEqual_ObjectObject_ReturnValue = UKismetMathLibrary::NotEqual_ObjectObject(TilePerk, TipFor);
        if (CallFunc_NotEqual_ObjectObject_ReturnValue) {
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(TileTip);
            if (!CallFunc_IsValid_ReturnValue_2) {
                CallFunc_GetOwningPlayer_ReturnValue = this->GetOwningPlayer();
                K2Node_DynamicCast_AsDB_Tip = Cast<DB_Tip>(UWidgetBlueprintLibrary::Create(this, DB_Tip::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue));
                K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Tip != nullptr;
                if (K2Node_DynamicCast_bSuccess) {
                    TileTip = K2Node_DynamicCast_AsDB_Tip;
                    Data->RegisterTip(TileTip);
                } else {
                    return;
                }
            }
            Data->PerkTip(TilePerk, TileRank, CallFunc_PerkTip_Title, CallFunc_PerkTip_Category, CallFunc_PerkTip_Body, CallFunc_PerkTip_Icon, CallFunc_PerkTip_Color);
            TileTip->Set(CallFunc_PerkTip_Title, CallFunc_PerkTip_Category, CallFunc_PerkTip_Body, CallFunc_PerkTip_Color, CallFunc_PerkTip_Icon);
            TileTip->SetExtra(FText(), FText());
            TipRank = TileRank;
            this->SetToolTip(TileTip);
            TipFor = TilePerk;
        }
        TileTip->SetVisibility(ESlateVisibility::HitTestInvisible);
        Data->MoreInfo(CallFunc_MoreInfo_On);
        TileTip->ShowExtra(CallFunc_MoreInfo_On);
        return;
    } else {
        return;
    }
}

void DB_PerkTile::Set(UPerkAsset* Perk, int32 Rank) {
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_IsUsageType_ReturnValue;
    bool CallFunc_IsUsageType_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_1;
    FLinearColor CallFunc_SelectColor_ReturnValue;
    FLinearColor CallFunc_SelectColor_ReturnValue_1;
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    TilePerk = Perk;
    TileRank = Rank;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Perk);
    if (CallFunc_IsValid_ReturnValue_1) {
        CallFunc_IsUsageType_ReturnValue = Perk->IsUsageType(EPerkUsageType::Active);
        CallFunc_IsUsageType_ReturnValue_1 = Perk->IsUsageType(EPerkUsageType::Passive);
        CallFunc_SelectColor_ReturnValue = UKismetMathLibrary::SelectColor(FLinearColor(0.07053f, 0.520833f, 0.375543f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_IsUsageType_ReturnValue);
        CallFunc_SelectColor_ReturnValue_1 = UKismetMathLibrary::SelectColor(FLinearColor(0.484375f, 0.027751f, 0.07654f, 1.0f), CallFunc_SelectColor_ReturnValue, CallFunc_IsUsageType_ReturnValue_1);
        Frame->SetBrushColor(CallFunc_SelectColor_ReturnValue_1);
        CallFunc_IsUsageType_ReturnValue = Perk->IsUsageType(EPerkUsageType::Active);
        CallFunc_IsUsageType_ReturnValue_1 = Perk->IsUsageType(EPerkUsageType::Passive);
        CallFunc_SelectColor_ReturnValue = UKismetMathLibrary::SelectColor(FLinearColor(0.07053f, 0.520833f, 0.375543f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_IsUsageType_ReturnValue);
        CallFunc_SelectColor_ReturnValue_1 = UKismetMathLibrary::SelectColor(FLinearColor(0.484375f, 0.027751f, 0.07654f, 1.0f), CallFunc_SelectColor_ReturnValue, CallFunc_IsUsageType_ReturnValue_1);
        Fill->SetBrushColor(CallFunc_SelectColor_ReturnValue_1);
        Icon->SetBrushFromTexture(Perk->Icon, false);
        Temp_bool_Variable = UKismetSystemLibrary::IsValid(Perk->Icon);
        Icon->SetVisibility(Temp_bool_Variable == false ? ESlateVisibility::Collapsed : Temp_bool_Variable == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default);
    } else {
        Frame->SetBrushColor(FLinearColor(0.104167f, 0.052518f, 0.026584f, 1.0f));
        Fill->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.7f));
        Icon->SetVisibility(ESlateVisibility::Collapsed);
    }
    CallFunc_BooleanAND_ReturnValue = UKismetSystemLibrary::IsValid(TipFor) && (UKismetMathLibrary::NotEqual_ObjectObject(TipFor, Perk) || UKismetMathLibrary::NotEqual_IntInt(TipRank, Rank));
    if (CallFunc_BooleanAND_ReturnValue) {
        this->SetToolTip(nullptr);
        TipFor = nullptr;
        return;
    } else {
        return;
    }
}

}   // namespace UI
