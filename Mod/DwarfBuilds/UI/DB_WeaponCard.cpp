// Decompiled from /Game/_DwarfBuilds/UI/DB_WeaponCard by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_WeaponCard::BndEvt__DB_WeaponCard_OcBtn_K2Node_ComponentBoundEvent_1_OnButtonHoverEvent__DelegateSignature() {
    bool CallFunc_BooleanAND_ReturnValue_1;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_5;
    bool CallFunc_MoreInfo_On_1;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue_1;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(CardOc), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue_1) {
        CallFunc_NotEqual_ObjectObject_ReturnValue_1 = UKismetMathLibrary::NotEqual_ObjectObject(CardOc, OcTipFor);
        if (CallFunc_NotEqual_ObjectObject_ReturnValue_1) {
            CallFunc_IsValid_ReturnValue_5 = UKismetSystemLibrary::IsValid(OcTip);
            if (!CallFunc_IsValid_ReturnValue_5) {
                CallFunc_GetOwningPlayer_ReturnValue = this->GetOwningPlayer();
                K2Node_DynamicCast_AsDB_Tip = Cast<DB_Tip>(UWidgetBlueprintLibrary::Create(this, DB_Tip::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue));
                K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Tip != nullptr;
                if (K2Node_DynamicCast_bSuccess) {
                    OcTip = K2Node_DynamicCast_AsDB_Tip;
                    Data->RegisterTip(OcTip);
                } else {
                    return;
                }
            }
            OcTip->Set(OcTitle, OcCategory, OcBody, OcColor, OcIconTex);
            OcTip->SetExtra(FText(), FText());
            OcBtn->SetToolTip(OcTip);
            OcTipFor = CardOc;
        }
        OcTip->SetVisibility(ESlateVisibility::HitTestInvisible);
        Data->MoreInfo(CallFunc_MoreInfo_On_1);
        OcTip->ShowExtra(CallFunc_MoreInfo_On_1);
        return;
    } else {
        return;
    }
}

void DB_WeaponCard::BndEvt__DB_WeaponCard_NameBtn_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature() {
    bool CallFunc_BooleanAND_ReturnValue;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    FText CallFunc_ItemTip_Body;
    FText CallFunc_ItemTip_Category;
    UTexture2D* CallFunc_ItemTip_Icon;
    FText CallFunc_ItemTip_Title;
    bool CallFunc_MoreInfo_On;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip_1;
    bool K2Node_DynamicCast_bSuccess_1;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(CardItem), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_NotEqual_ObjectObject_ReturnValue = UKismetMathLibrary::NotEqual_ObjectObject(CardItem, ItemTipFor);
        if (CallFunc_NotEqual_ObjectObject_ReturnValue) {
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(ItemTip);
            if (!CallFunc_IsValid_ReturnValue_2) {
                CallFunc_GetOwningPlayer_ReturnValue_1 = this->GetOwningPlayer();
                K2Node_DynamicCast_AsDB_Tip_1 = Cast<DB_Tip>(UWidgetBlueprintLibrary::Create(this, DB_Tip::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue_1));
                K2Node_DynamicCast_bSuccess_1 = K2Node_DynamicCast_AsDB_Tip_1 != nullptr;
                if (K2Node_DynamicCast_bSuccess_1) {
                    ItemTip = K2Node_DynamicCast_AsDB_Tip_1;
                    Data->RegisterTip(ItemTip);
                } else {
                    return;
                }
            }
            Data->ItemTip(CardItem, CallFunc_ItemTip_Title, CallFunc_ItemTip_Category, CallFunc_ItemTip_Body, CallFunc_ItemTip_Icon);
            ItemTip->Set(CallFunc_ItemTip_Title, CallFunc_ItemTip_Category, CallFunc_ItemTip_Body, FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_ItemTip_Icon);
            ItemTip->SetExtra(FText(), FText());
            NameBtn->SetToolTip(ItemTip);
            ItemTipFor = CardItem;
        }
        ItemTip->SetVisibility(ESlateVisibility::HitTestInvisible);
        Data->MoreInfo(CallFunc_MoreInfo_On);
        ItemTip->ShowExtra(CallFunc_MoreInfo_On);
        return;
    } else {
        return;
    }
}

void DB_WeaponCard::Show(UItemID* Item, TArray<int32>& Choices, UOverclockUpgrade* Overclock, FString Digits) {
    UImage* CallFunc_Array_Get_Item;
    DB_UpgCell* CallFunc_Array_Get_Item_1;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue_2;
    FText CallFunc_ItemVisual_Category;
    UTexture2D* CallFunc_ItemVisual_Fill;
    UTexture2D* CallFunc_ItemVisual_Line;
    FText CallFunc_ItemVisual_Name;
    bool CallFunc_Less_IntInt_ReturnValue;
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    EnsureCells();
    CardItem = Item;
    CardOc = Overclock;
    CallFunc_BooleanAND_ReturnValue_2 = UKismetSystemLibrary::IsValid(ItemTipFor) && UKismetMathLibrary::NotEqual_ObjectObject(ItemTipFor, Item);
    if (CallFunc_BooleanAND_ReturnValue_2) {
        NameBtn->SetToolTip(nullptr);
        ItemTipFor = nullptr;
    }
    CallFunc_BooleanAND_ReturnValue_1 = UKismetSystemLibrary::IsValid(OcTipFor) && UKismetMathLibrary::NotEqual_ObjectObject(OcTipFor, Overclock);
    if (CallFunc_BooleanAND_ReturnValue_1) {
        OcBtn->SetToolTip(nullptr);
        OcTipFor = nullptr;
    }
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(Item), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        Data->ItemVisual(Item, CallFunc_ItemVisual_Name, CallFunc_ItemVisual_Category, CallFunc_ItemVisual_Line, CallFunc_ItemVisual_Fill);
        NameText->SetText(CallFunc_ItemVisual_Name);
        NameText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
        CatText->SetText(CallFunc_ItemVisual_Category);
        SilFill->SetBrushFromTexture(CallFunc_ItemVisual_Fill, true);
        SilLine->SetBrushFromTexture(CallFunc_ItemVisual_Line, true);
        Temp_byte_Variable = ESlateVisibility::HitTestInvisible;
        Temp_byte_Variable_1 = ESlateVisibility::Hidden;
        Temp_bool_Variable = UKismetSystemLibrary::IsValid(CallFunc_ItemVisual_Line);
        SilBox->SetVisibility(Temp_bool_Variable == false ? Temp_byte_Variable_1 : Temp_bool_Variable == true ? Temp_byte_Variable : K2Node_Select_Default);
        OcBtn->SetVisibility(ESlateVisibility::Visible);
        ShowOverclock();
        ShowGrid(Item, Choices);
        return;
    } else {
        NameText->SetText("UNKNOWN");
        NameText->SetColorAndOpacity(FSlateColor(FLinearColor(0.36625f, 0.30947f, 0.19462f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
        CatText->SetText(FText());
        SilBox->SetVisibility(ESlateVisibility::Hidden);
        OcBtn->SetVisibility(ESlateVisibility::Hidden);
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, CellList.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                CellList.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                CallFunc_Array_Get_Item_1->Data = Data;
                CellList.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                CallFunc_Array_Get_Item_1->SetVisibility(ESlateVisibility::Visible);
                LinkList.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                CallFunc_Array_Get_Item->SetVisibility(ESlateVisibility::Hidden);
                CellList.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                CallFunc_Array_Get_Item_1->Set(nullptr, false);
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                return;
            }
        }
    }
}

void DB_WeaponCard::EnsureCells() {
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    TArray<UImage*> K2Node_MakeArray_Array;
    TArray<DB_UpgCell*> K2Node_MakeArray_Array_1;
    CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CellList.Num(), 0);
    if (CallFunc_EqualEqual_IntInt_ReturnValue) {
        K2Node_MakeArray_Array_1 = {C00, C01, C02, C10, C11, C12, C20, C21, C22, C30, C31, C32, C40, C41, C42};
        CellList = K2Node_MakeArray_Array_1;
        K2Node_MakeArray_Array = {L00, L01, L02, L10, L11, L12, L20, L21, L22, L30, L31, L32, L40, L41, L42};
        LinkList = K2Node_MakeArray_Array;
        return;
    } else {
        return;
    }
}

void DB_WeaponCard::ShowOverclock() {
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    FText CallFunc_OverclockTip_Body;
    FText CallFunc_OverclockTip_Category;
    FLinearColor CallFunc_OverclockTip_Color;
    UTexture2D* CallFunc_OverclockTip_Frame;
    UTexture2D* CallFunc_OverclockTip_Icon;
    FText CallFunc_OverclockTip_Title;
    FLinearColor CallFunc_SelectColor_ReturnValue;
    FSlateColor K2Node_MakeStruct_SlateColor;
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetMathLibrary::EqualEqual_ObjectObject(CardOc, OcShown), UKismetSystemLibrary::IsValid(OcFrameTex));
    if (CallFunc_BooleanAND_ReturnValue) {
        return;
    } else {
        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(Data);
        CallFunc_BooleanAND_ReturnValue_1 = UKismetSystemLibrary::IsValid(CardOc) && CallFunc_IsValid_ReturnValue_2;
        if (CallFunc_BooleanAND_ReturnValue_1) {
            Data->OverclockTip(CardOc, CallFunc_OverclockTip_Title, CallFunc_OverclockTip_Category, CallFunc_OverclockTip_Body, CallFunc_OverclockTip_Icon, CallFunc_OverclockTip_Color, CallFunc_OverclockTip_Frame);
            OcTitle = CallFunc_OverclockTip_Title;
            OcCategory = CallFunc_OverclockTip_Category;
            OcBody = CallFunc_OverclockTip_Body;
            OcIconTex = CallFunc_OverclockTip_Icon;
            OcColor = CallFunc_OverclockTip_Color;
            OcFrameTex = CallFunc_OverclockTip_Frame;
            OcShown = CardOc;
            OcName->SetText(OcTitle);
            OcName->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
            OcType->SetText(OcCategory);
            K2Node_MakeStruct_SlateColor.SpecifiedColor = OcColor;
            K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
            OcType->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
            OcTypeBox->SetVisibility(ESlateVisibility::HitTestInvisible);
            OcFrame->SetBrushFromTexture(OcFrameTex, false);
            OcFrame->SetColorAndOpacity(OcColor);
            OcIcon->SetBrushFromTexture(OcIconTex, false);
            CallFunc_SelectColor_ReturnValue = UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), FLinearColor(0.00857f, 0.007f, 0.00402f, 1.0f), UKismetSystemLibrary::IsValid(OcFrameTex));
            OcIcon->SetColorAndOpacity(CallFunc_SelectColor_ReturnValue);
            Temp_byte_Variable = ESlateVisibility::HitTestInvisible;
            Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
            Temp_bool_Variable = UKismetSystemLibrary::IsValid(OcIconTex);
            OcIcon->SetVisibility(Temp_bool_Variable == false ? Temp_byte_Variable_1 : Temp_bool_Variable == true ? Temp_byte_Variable : K2Node_Select_Default);
            return;
        } else {
            OcName->SetText("NO OVERCLOCK");
            OcName->SetColorAndOpacity(FSlateColor(FLinearColor(0.36625f, 0.30947f, 0.19462f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
            OcType->SetText(FText());
            OcTypeBox->SetVisibility(ESlateVisibility::Collapsed);
            OcFrame->SetBrushFromTexture(nullptr, false);
            OcFrame->SetColorAndOpacity(FLinearColor(0.104167f, 0.052518f, 0.026584f, 0.6f));
            OcIcon->SetVisibility(ESlateVisibility::Collapsed);
            OcShown = nullptr;
            OcFrameTex = nullptr;
            return;
        }
    }
}

void DB_WeaponCard::ShowGrid(UItemID* GridItem, TArray<int32>& GridChoices) {
    int32 CallFunc_Add_IntInt_ReturnValue_2;
    int32 CallFunc_Array_Get_Item;
    UItemUpgrade* CallFunc_Array_Get_Item_1;
    UImage* CallFunc_Array_Get_Item_2;
    DB_UpgCell* CallFunc_Array_Get_Item_3;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    int32 CallFunc_Multiply_IntInt_ReturnValue;
    TArray<UItemUpgrade*> CallFunc_TierOptions_Upgrades;
    int32 Temp_int_Variable;
    int32 Temp_int_Variable_1;
    Temp_int_Variable = 0;
    while (true) {
        CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 4);
        if (CallFunc_LessEqual_IntInt_ReturnValue) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Variable, GridChoices.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Data->TierOptions(GridItem, Temp_int_Variable, CallFunc_TierOptions_Upgrades);
                TierOpts = CallFunc_TierOptions_Upgrades;
            } else {
                TierOpts.Clear();
            }
            Temp_int_Variable_1 = 0;
            while (true) {
                CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_1, 2);
                if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
                    CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Variable_1, TierOpts.Num());
                    if (CallFunc_Less_IntInt_ReturnValue_1) {
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        CellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                        CallFunc_Array_Get_Item_3->Data = Data;
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        CellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                        CallFunc_Array_Get_Item_3->SetVisibility(ESlateVisibility::Visible);
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        LinkList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_2);
                        CallFunc_Array_Get_Item_2->SetVisibility(ESlateVisibility::HitTestInvisible);
                        GridChoices.Get(Temp_int_Variable, CallFunc_Array_Get_Item);
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        TierOpts.Get(Temp_int_Variable_1, CallFunc_Array_Get_Item_1);
                        CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_Array_Get_Item, UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1));
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        CellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                        CallFunc_Array_Get_Item_3->Set(CallFunc_Array_Get_Item_1, CallFunc_EqualEqual_IntInt_ReturnValue);
                    } else {
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        CellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                        CallFunc_Array_Get_Item_3->SetVisibility(ESlateVisibility::Hidden);
                        CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                        CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                        LinkList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_2);
                        CallFunc_Array_Get_Item_2->SetVisibility(ESlateVisibility::Hidden);
                    }
                    Temp_int_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1);
                } else {
                    Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                    break;
                }
            }
        } else {
            return;
        }
    }
}

}   // namespace UI
