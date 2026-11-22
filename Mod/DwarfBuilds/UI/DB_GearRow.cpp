// Decompiled from /Game/_DwarfBuilds/UI/DB_GearRow by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_GearRow::BndEvt__DB_GearRow_PicBtn_K2Node_ComponentBoundEvent_0_OnButtonHoverEvent__DelegateSignature() {
    bool CallFunc_BooleanAND_ReturnValue;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_2;
    FText CallFunc_ItemTip_Body;
    FText CallFunc_ItemTip_Category;
    UTexture2D* CallFunc_ItemTip_Icon;
    FText CallFunc_ItemTip_Title;
    bool CallFunc_MoreInfo_On;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(GearItem), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_NotEqual_ObjectObject_ReturnValue = UKismetMathLibrary::NotEqual_ObjectObject(GearItem, ItemTipFor);
        if (CallFunc_NotEqual_ObjectObject_ReturnValue) {
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(ItemTip);
            if (!CallFunc_IsValid_ReturnValue_2) {
                CallFunc_GetOwningPlayer_ReturnValue = this->GetOwningPlayer();
                K2Node_DynamicCast_AsDB_Tip = Cast<DB_Tip>(UWidgetBlueprintLibrary::Create(this, DB_Tip::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue));
                K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Tip != nullptr;
                if (K2Node_DynamicCast_bSuccess) {
                    ItemTip = K2Node_DynamicCast_AsDB_Tip;
                    Data->RegisterTip(ItemTip);
                } else {
                    return;
                }
            }
            Data->ItemTip(GearItem, CallFunc_ItemTip_Title, CallFunc_ItemTip_Category, CallFunc_ItemTip_Body, CallFunc_ItemTip_Icon);
            ItemTip->Set(CallFunc_ItemTip_Title, CallFunc_ItemTip_Category, CallFunc_ItemTip_Body, FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_ItemTip_Icon);
            ItemTip->SetExtra(FText(), FText());
            PicBtn->SetToolTip(ItemTip);
            ItemTipFor = GearItem;
        }
        ItemTip->SetVisibility(ESlateVisibility::HitTestInvisible);
        Data->MoreInfo(CallFunc_MoreInfo_On);
        ItemTip->ShowExtra(CallFunc_MoreInfo_On);
        return;
    } else {
        return;
    }
}

void DB_GearRow::Show(FString Label, UItemID* Item, TArray<int32>& Choices) {
    int32 CallFunc_Add_IntInt_ReturnValue_2;
    int32 CallFunc_Array_Get_Item;
    UItemUpgrade* CallFunc_Array_Get_Item_1;
    UImage* CallFunc_Array_Get_Item_2;
    DB_UpgCell* CallFunc_Array_Get_Item_3;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    FText CallFunc_Conv_StringToText_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    bool CallFunc_Greater_IntInt_ReturnValue;
    FText CallFunc_ItemVisual_Category;
    UTexture2D* CallFunc_ItemVisual_Fill;
    UTexture2D* CallFunc_ItemVisual_Line;
    FText CallFunc_ItemVisual_Name;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    int32 CallFunc_Multiply_IntInt_ReturnValue;
    TArray<UItemUpgrade*> CallFunc_TierOptions_Upgrades;
    TArray<UImage*> K2Node_MakeArray_Array;
    TArray<DB_UpgCell*> K2Node_MakeArray_Array_1;
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    int32 Temp_int_Variable;
    int32 Temp_int_Variable_1;
    CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(GearCellList.Num(), 0);
    if (CallFunc_EqualEqual_IntInt_ReturnValue_1) {
        K2Node_MakeArray_Array_1 = {G00, G01, G02, G10, G11, G12, G20, G21, G22, G30, G31, G32};
        GearCellList = K2Node_MakeArray_Array_1;
        K2Node_MakeArray_Array = {GL00, GL01, GL02, GL10, GL11, GL12, GL20, GL21, GL22, GL30, GL31, GL32};
        GearLinkList = K2Node_MakeArray_Array;
    }
    GearItem = Item;
    CallFunc_BooleanAND_ReturnValue_1 = UKismetSystemLibrary::IsValid(ItemTipFor) && UKismetMathLibrary::NotEqual_ObjectObject(ItemTipFor, Item);
    if (CallFunc_BooleanAND_ReturnValue_1) {
        PicBtn->SetToolTip(nullptr);
        ItemTipFor = nullptr;
    }
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(Label);
    LabelText->SetText(CallFunc_Conv_StringToText_ReturnValue);
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(Item), UKismetSystemLibrary::IsValid(Data));
    if (CallFunc_BooleanAND_ReturnValue) {
        Data->ItemVisual(Item, CallFunc_ItemVisual_Name, CallFunc_ItemVisual_Category, CallFunc_ItemVisual_Line, CallFunc_ItemVisual_Fill);
        NameText->SetText(CallFunc_ItemVisual_Name);
        NameText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
        CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(Choices.Num(), 0);
        if (CallFunc_Greater_IntInt_ReturnValue) {
            PicBtn->SetVisibility(ESlateVisibility::Collapsed);
            Grid->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
            Temp_int_Variable = 0;
            while (true) {
                CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 3);
                if (CallFunc_LessEqual_IntInt_ReturnValue) {
                    CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Variable, Choices.Num());
                    if (CallFunc_Less_IntInt_ReturnValue) {
                        Data->TierOptions(Item, Temp_int_Variable, CallFunc_TierOptions_Upgrades);
                        GearOpts = CallFunc_TierOptions_Upgrades;
                    } else {
                        GearOpts.Clear();
                    }
                    Temp_int_Variable_1 = 0;
                    while (true) {
                        CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_1, 2);
                        if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
                            CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Variable_1, GearOpts.Num());
                            if (CallFunc_Less_IntInt_ReturnValue_1) {
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearCellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                                CallFunc_Array_Get_Item_3->Data = Data;
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearCellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                                CallFunc_Array_Get_Item_3->SetVisibility(ESlateVisibility::Visible);
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearLinkList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_2);
                                CallFunc_Array_Get_Item_2->SetVisibility(ESlateVisibility::HitTestInvisible);
                                Choices.Get(Temp_int_Variable, CallFunc_Array_Get_Item);
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                GearOpts.Get(Temp_int_Variable_1, CallFunc_Array_Get_Item_1);
                                CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_Array_Get_Item, UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1));
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearCellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                                CallFunc_Array_Get_Item_3->Set(CallFunc_Array_Get_Item_1, CallFunc_EqualEqual_IntInt_ReturnValue);
                            } else {
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearCellList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_3);
                                CallFunc_Array_Get_Item_3->SetVisibility(ESlateVisibility::Hidden);
                                CallFunc_Multiply_IntInt_ReturnValue = UKismetMathLibrary::Multiply_IntInt(Temp_int_Variable, 3);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(CallFunc_Multiply_IntInt_ReturnValue, Temp_int_Variable_1);
                                GearLinkList.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_2);
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
        } else {
            Grid->SetVisibility(ESlateVisibility::Hidden);
            PicFill->SetBrushFromTexture(CallFunc_ItemVisual_Fill, true);
            PicLine->SetBrushFromTexture(CallFunc_ItemVisual_Line, true);
            Temp_byte_Variable = ESlateVisibility::Visible;
            Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
            Temp_bool_Variable = UKismetSystemLibrary::IsValid(CallFunc_ItemVisual_Line);
            PicBtn->SetVisibility(Temp_bool_Variable == false ? Temp_byte_Variable_1 : Temp_bool_Variable == true ? Temp_byte_Variable : K2Node_Select_Default);
            return;
        }
    } else {
        NameText->SetText("UNKNOWN");
        NameText->SetColorAndOpacity(FSlateColor(FLinearColor(0.36625f, 0.30947f, 0.19462f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
        Grid->SetVisibility(ESlateVisibility::Hidden);
        PicBtn->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
}

}   // namespace UI
