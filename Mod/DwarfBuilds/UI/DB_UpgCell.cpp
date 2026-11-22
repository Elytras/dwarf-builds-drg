// Decompiled from /Game/_DwarfBuilds/UI/DB_UpgCell by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_UpgCell::OnMouseEnter(FGeometry MyGeometry, const FPointerEvent& MouseEvent) {
    bool CallFunc_BooleanAND_ReturnValue;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_MoreInfo_On;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue;
    FText CallFunc_UpgradeTip_Body;
    FText CallFunc_UpgradeTip_Category;
    FText CallFunc_UpgradeTip_Extra;
    FText CallFunc_UpgradeTip_ExtraTitle;
    UTexture2D* CallFunc_UpgradeTip_Icon;
    FText CallFunc_UpgradeTip_Title;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(CellUpgrade), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_NotEqual_ObjectObject_ReturnValue = UKismetMathLibrary::NotEqual_ObjectObject(CellUpgrade, TipFor);
        if (CallFunc_NotEqual_ObjectObject_ReturnValue) {
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(CellTip);
            if (!CallFunc_IsValid_ReturnValue_2) {
                CallFunc_GetOwningPlayer_ReturnValue = this->GetOwningPlayer();
                K2Node_DynamicCast_AsDB_Tip = Cast<DB_Tip>(UWidgetBlueprintLibrary::Create(this, DB_Tip::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue));
                K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Tip != nullptr;
                if (K2Node_DynamicCast_bSuccess) {
                    CellTip = K2Node_DynamicCast_AsDB_Tip;
                    Data->RegisterTip(CellTip);
                } else {
                    return;
                }
            }
            Data->UpgradeTip(CellUpgrade, CallFunc_UpgradeTip_Title, CallFunc_UpgradeTip_Category, CallFunc_UpgradeTip_Body, CallFunc_UpgradeTip_Icon, CallFunc_UpgradeTip_ExtraTitle, CallFunc_UpgradeTip_Extra);
            CellTip->Set(CallFunc_UpgradeTip_Title, CallFunc_UpgradeTip_Category, CallFunc_UpgradeTip_Body, FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_UpgradeTip_Icon);
            CellTip->SetExtra(CallFunc_UpgradeTip_ExtraTitle, CallFunc_UpgradeTip_Extra);
            this->SetToolTip(CellTip);
            TipFor = CellUpgrade;
        }
        CellTip->SetVisibility(ESlateVisibility::HitTestInvisible);
        Data->MoreInfo(CallFunc_MoreInfo_On);
        CellTip->ShowExtra(CallFunc_MoreInfo_On);
        return;
    } else {
        return;
    }
}

void DB_UpgCell::Set(UItemUpgrade* Upgrade, bool Selected) {
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue_2;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_4;
    bool CallFunc_IsValid_ReturnValue_5;
    bool CallFunc_IsValid_ReturnValue_8;
    bool CallFunc_Not_PreBool_ReturnValue;
    FText CallFunc_UpgradeTip_Body;
    FText CallFunc_UpgradeTip_Category;
    FText CallFunc_UpgradeTip_Extra;
    FText CallFunc_UpgradeTip_ExtraTitle;
    UTexture2D* CallFunc_UpgradeTip_Icon;
    FText CallFunc_UpgradeTip_Title;
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    CellUpgrade = Upgrade;
    CellSelected = Selected;
    CallFunc_Not_PreBool_ReturnValue = !HexDone;
    if (CallFunc_Not_PreBool_ReturnValue) {
        CallFunc_IsValid_ReturnValue_8 = UKismetSystemLibrary::IsValid(Data);
        if (CallFunc_IsValid_ReturnValue_8) {
            CallFunc_BooleanAND_ReturnValue_2 = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(Data->HexBg), UKismetSystemLibrary::IsValid(Data->HexOutline));
            if (CallFunc_BooleanAND_ReturnValue_2) {
                HexBg->SetBrushFromTexture(Data->HexBg, false);
                HexLine->SetBrushFromTexture(Data->HexOutline, false);
                HexLine->SetVisibility(ESlateVisibility::HitTestInvisible);
                HexDone = true;
            }
        }
    }
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Upgrade);
    CallFunc_BooleanAND_ReturnValue = CallFunc_IsValid_ReturnValue_1 && Selected;
    HexBg->SetColorAndOpacity(UKismetMathLibrary::SelectColor(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.645482f, 0.0f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_BooleanAND_ReturnValue), FLinearColor(0.0f, 0.0f, 0.0f, 0.45f), CallFunc_IsValid_ReturnValue_1));
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Upgrade);
    CallFunc_BooleanAND_ReturnValue = CallFunc_IsValid_ReturnValue_1 && Selected;
    HexLine->SetColorAndOpacity(UKismetMathLibrary::SelectColor(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_BooleanAND_ReturnValue), FLinearColor(0.104167f, 0.052518f, 0.026584f, 1.0f), CallFunc_IsValid_ReturnValue_1));
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Upgrade);
    if (CallFunc_IsValid_ReturnValue_1) {
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Upgrade->Category);
        if (CallFunc_IsValid_ReturnValue) {
            IconTex = Upgrade->Category->CategoryIcon;
        } else {
            IconTex = nullptr;
        }
        CallFunc_IsValid_ReturnValue_5 = UKismetSystemLibrary::IsValid(IconTex);
        if (!CallFunc_IsValid_ReturnValue_5) {
            CallFunc_IsValid_ReturnValue_4 = UKismetSystemLibrary::IsValid(Data);
            if (CallFunc_IsValid_ReturnValue_4) {
                Data->UpgradeTip(Upgrade, CallFunc_UpgradeTip_Title, CallFunc_UpgradeTip_Category, CallFunc_UpgradeTip_Body, CallFunc_UpgradeTip_Icon, CallFunc_UpgradeTip_ExtraTitle, CallFunc_UpgradeTip_Extra);
                IconTex = CallFunc_UpgradeTip_Icon;
            }
        }
        Icon->SetBrushFromTexture(IconTex, false);
        Icon->SetBrushSize(UKismetMathLibrary::MakeVector2D(IconPx, IconPx));
        Icon->SetColorAndOpacity(UKismetMathLibrary::SelectColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), FLinearColor(0.0f, 0.0f, 0.0f, 1.0f), Selected));
        Temp_byte_Variable = ESlateVisibility::HitTestInvisible;
        Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
        Temp_bool_Variable = UKismetSystemLibrary::IsValid(IconTex);
        Icon->SetVisibility(Temp_bool_Variable == false ? Temp_byte_Variable_1 : Temp_bool_Variable == true ? Temp_byte_Variable : K2Node_Select_Default);
    } else {
        Icon->SetVisibility(ESlateVisibility::Collapsed);
    }
    CallFunc_BooleanAND_ReturnValue_1 = UKismetSystemLibrary::IsValid(TipFor) && UKismetMathLibrary::NotEqual_ObjectObject(TipFor, Upgrade);
    if (CallFunc_BooleanAND_ReturnValue_1) {
        this->SetToolTip(nullptr);
        TipFor = nullptr;
        return;
    } else {
        return;
    }
}

}   // namespace UI
