// Decompiled from /Game/_DwarfBuilds/UI/DB_TakeDialog by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_TakeDialog::BndEvt__DB_TakeDialog_CancelBtn_K2Node_ComponentBoundEvent_4_OnButtonReleasedEvent__DelegateSignature() {
    CancelBtnBarL->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    CancelBtnBarR->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    CancelBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.85f, 0.76f, 0.55f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_TakeDialog::BndEvt__DB_TakeDialog_CancelBtn_K2Node_ComponentBoundEvent_3_OnButtonPressedEvent__DelegateSignature() {
    CancelBtnBarL->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    CancelBtnBarR->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    CancelBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_TakeDialog::BndEvt__DB_TakeDialog_EquipAfter_K2Node_ComponentBoundEvent_2_OnCheckBoxComponentStateChanged__DelegateSignature(bool bIsChecked) {
    SyncCheck();
    return;
}

void DB_TakeDialog::BndEvt__DB_TakeDialog_CancelBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature() {
    bool CallFunc_IsValid_ReturnValue;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Window);
    if (CallFunc_IsValid_ReturnValue) {
        Window->CloseTake();
        return;
    } else {
        SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
}

void DB_TakeDialog::BndEvt__DB_TakeDialog_SaveBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature() {
    SaveNow();
    return;
}

void DB_TakeDialog::Show(FString NewCode, FString Title) {
    bool CallFunc_CanTake_Ok;
    FString CallFunc_CanTake_Reason;
    UPlayerCharacterID* CallFunc_ClassOfCode_Class;
    bool CallFunc_ClassOfCode_Valid;
    FString CallFunc_Concat_StrStr_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue_1;
    FLinearColor CallFunc_HeaderOfCode_Color;
    UTexture2D* CallFunc_HeaderOfCode_Icon;
    FString CallFunc_HeaderOfCode_Sub;
    bool CallFunc_IsValid_ReturnValue;
    FString CallFunc_Replace_ReturnValue;
    int32 CallFunc_SlotInfo_Count;
    int32 CallFunc_SlotInfo_Current;
    FSlateColor K2Node_MakeStruct_SlateColor;
    FSlateColor K2Node_MakeStruct_SlateColor_1;
    ESlateVisibility K2Node_Select_Default;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
    if (CallFunc_IsValid_ReturnValue) {
        Code = NewCode;
        Saved = false;
        Data->ClassOfCode(NewCode, CallFunc_ClassOfCode_Class, CallFunc_ClassOfCode_Valid);
        CodeClass = CallFunc_ClassOfCode_Class;
        Data->HeaderOfCode(NewCode, CallFunc_HeaderOfCode_Sub, CallFunc_HeaderOfCode_Color, CallFunc_HeaderOfCode_Icon);
        CallFunc_Concat_StrStr_ReturnValue = UKismetStringLibrary::Concat_StrStr(Title, L" · ");
        CallFunc_Replace_ReturnValue = UKismetStringLibrary::Replace(CallFunc_HeaderOfCode_Sub, " BUILD", "", ESearchCase::IgnoreCase);
        CallFunc_Conv_StringToText_ReturnValue_1 = UKismetTextLibrary::Conv_StringToText(UKismetMathLibrary::SelectString(Title, UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue, CallFunc_Replace_ReturnValue), UKismetStringLibrary::isEmpty(CallFunc_Replace_ReturnValue)));
        ClassLine->SetText(CallFunc_Conv_StringToText_ReturnValue_1);
        K2Node_MakeStruct_SlateColor_1.SpecifiedColor = CallFunc_HeaderOfCode_Color;
        K2Node_MakeStruct_SlateColor_1.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
        ClassLine->SetColorAndOpacity(K2Node_MakeStruct_SlateColor_1);
        Data->SlotInfo(CallFunc_ClassOfCode_Class, CallFunc_SlotInfo_Count, CallFunc_SlotInfo_Current);
        SlotCount = CallFunc_SlotInfo_Count;
        CurrentSlot = CallFunc_SlotInfo_Current;
        Picked = CallFunc_SlotInfo_Current;
        Data->CanTake(CallFunc_CanTake_Ok, CallFunc_CanTake_Reason);
        CanSave = CallFunc_CanTake_Ok && CallFunc_ClassOfCode_Valid && UKismetMathLibrary::Greater_IntInt(CallFunc_SlotInfo_Count, 0);
        EquipAfter->SetIsChecked(true);
        SyncCheck();
        CancelBtnText->SetText("CANCEL");
        CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(UKismetMathLibrary::SelectString(UKismetMathLibrary::SelectString("No loadout slots found.", CallFunc_CanTake_Reason, CallFunc_CanTake_Ok), "Not a Dwarf Builds code.", CallFunc_ClassOfCode_Valid));
        StatusText->SetText(CallFunc_Conv_StringToText_ReturnValue);
        K2Node_MakeStruct_SlateColor.SpecifiedColor = FLinearColor(0.87137f, 0.16203f, 0.10702f, 1.0f);
        K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
        StatusText->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
        StatusBox->SetVisibility(!CanSave == false ? ESlateVisibility::Collapsed : !CanSave == true ? ESlateVisibility::Visible : K2Node_Select_Default);
        UpdateSlots();
        return;
    } else {
        return;
    }
}

void DB_TakeDialog::PickSlot(int32 SlotIndex) {
    if (Saved) {
        return;
    } else {
        Picked = SlotIndex;
        UpdateSlots();
        return;
    }
}

void DB_TakeDialog::UpdateSlots() {
    DB_SlotBtn* CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue_1;
    FText CallFunc_Conv_StringToText_ReturnValue_2;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    DB_SlotBtn* CallFunc_GetButton_Button;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    FString CallFunc_SlotLabel_Label;
    FString CallFunc_SlotLabel_Label_1;
    FString CallFunc_SlotLabel_Label_2;
    int32 Temp_int_Variable;
    int32 Temp_int_Variable_1;
    Temp_int_Variable = 0;
    while (true) {
        CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, UKismetMathLibrary::Subtract_IntInt(SlotCount, 1));
        if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
            GetButton(Temp_int_Variable, CallFunc_GetButton_Button);
            SlotLabel(Temp_int_Variable, CallFunc_SlotLabel_Label);
            CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(Temp_int_Variable, CurrentSlot);
            CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(Temp_int_Variable, Picked);
            CallFunc_GetButton_Button->Set(CallFunc_SlotLabel_Label, CallFunc_EqualEqual_IntInt_ReturnValue_1, CallFunc_EqualEqual_IntInt_ReturnValue);
            CallFunc_GetButton_Button->SetVisibility(ESlateVisibility::Visible);
            Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
        } else {
            Temp_int_Variable_1 = SlotCount;
            while (true) {
                CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_1, UKismetMathLibrary::Subtract_IntInt(Buttons.Num(), 1));
                if (CallFunc_LessEqual_IntInt_ReturnValue) {
                    Buttons.Get(Temp_int_Variable_1, CallFunc_Array_Get_Item);
                    CallFunc_Array_Get_Item->SetVisibility(ESlateVisibility::Collapsed);
                    Temp_int_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1);
                } else {
                    SlotLabel(Picked, CallFunc_SlotLabel_Label_2);
                    SlotLabel(CurrentSlot, CallFunc_SlotLabel_Label_1);
                    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(UKismetStringLibrary::Concat_StrStr(CallFunc_SlotLabel_Label_1, " IS EQUIPPED NOW"));
                    NowText->SetText(CallFunc_Conv_StringToText_ReturnValue);
                    CallFunc_Conv_StringToText_ReturnValue_2 = UKismetTextLibrary::Conv_StringToText(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr("Slot ", CallFunc_SlotLabel_Label_2), " will be overwritten."));
                    Note->SetText(CallFunc_Conv_StringToText_ReturnValue_2);
                    CallFunc_Conv_StringToText_ReturnValue_1 = UKismetTextLibrary::Conv_StringToText(UKismetStringLibrary::Concat_StrStr("SAVE TO SLOT ", CallFunc_SlotLabel_Label_2));
                    SaveBtnText->SetText(CallFunc_Conv_StringToText_ReturnValue_1);
                    CallFunc_BooleanAND_ReturnValue = CanSave && !Saved;
                    SaveBtnWrap->SetIsEnabled(CallFunc_BooleanAND_ReturnValue);
                    return;
                }
            }
        }
    }
}

void DB_TakeDialog::SlotLabel(int32 SlotIndex, FString& Label) {
    Label = UKismetMathLibrary::SelectString(UKismetStringLibrary::GetSubstring("ABCDEFGHIJKLMNOPQRSTUVWXYZ", SlotIndex, 1), UKismetStringLibrary::Conv_IntToString(UKismetMathLibrary::Add_IntInt(SlotIndex, 1)), UKismetMathLibrary::Less_IntInt(SlotIndex, 26));
    return;
}

void DB_TakeDialog::GetButton(int32 ButtonIndex, DB_SlotBtn*& Button) {
    UUniformGridSlot* CallFunc_AddChildToUniformGrid_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue;
    DB_SlotBtn* CallFunc_Array_Get_Item;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue;
    DB_SlotBtn* K2Node_DynamicCast_AsDB_Slot_Btn;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_GreaterEqual_IntInt_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(ButtonIndex, Buttons.Num());
    if (CallFunc_GreaterEqual_IntInt_ReturnValue) {
        CallFunc_GetOwningPlayer_ReturnValue = GetOwningPlayer();
        K2Node_DynamicCast_AsDB_Slot_Btn = Cast<DB_SlotBtn>(UWidgetBlueprintLibrary::Create(this, DB_SlotBtn::StaticClass(), CallFunc_GetOwningPlayer_ReturnValue));
        K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Slot_Btn != nullptr;
        if (K2Node_DynamicCast_bSuccess) {
            K2Node_DynamicCast_AsDB_Slot_Btn->Dialog = this;
            K2Node_DynamicCast_AsDB_Slot_Btn->Index = ButtonIndex;
            CallFunc_AddChildToUniformGrid_ReturnValue = Grid->AddChildToUniformGrid(K2Node_DynamicCast_AsDB_Slot_Btn, UKismetMathLibrary::Divide_IntInt(ButtonIndex, 9), UKismetMathLibrary::Percent_IntInt(ButtonIndex, 9));
            CallFunc_Array_Add_ReturnValue = Buttons.Add(K2Node_DynamicCast_AsDB_Slot_Btn);
        }
    }
    Buttons.Get(ButtonIndex, CallFunc_Array_Get_Item);
    Button = CallFunc_Array_Get_Item;
    return;
}

void DB_TakeDialog::SyncCheck() {
    ESlateVisibility K2Node_Select_Default;
    bool Temp_bool_Variable;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    Temp_byte_Variable = ESlateVisibility::Visible;
    Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
    Temp_bool_Variable = EquipAfter->IsChecked();
    CheckMark->SetVisibility(Temp_bool_Variable == false ? Temp_byte_Variable_1 : Temp_bool_Variable == true ? Temp_byte_Variable : K2Node_Select_Default);
    return;
}

void DB_TakeDialog::SaveNow() {
    int32 CallFunc_ApplyCode_Applied;
    FString CallFunc_ApplyCode_Message;
    int32 CallFunc_ApplyCode_Skipped;
    bool CallFunc_BooleanAND_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue;
    bool CallFunc_IsChecked_ReturnValue;
    int32 CallFunc_SlotInfo_Count;
    int32 CallFunc_SlotInfo_Current;
    FSlateColor K2Node_MakeStruct_SlateColor;
    CallFunc_BooleanAND_ReturnValue = CanSave && !Saved;
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_IsChecked_ReturnValue = EquipAfter->IsChecked();
        Data->ApplyCode(Code, Picked, CallFunc_IsChecked_ReturnValue, CallFunc_ApplyCode_Applied, CallFunc_ApplyCode_Skipped, CallFunc_ApplyCode_Message);
        Saved = true;
        CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(CallFunc_ApplyCode_Message);
        StatusText->SetText(CallFunc_Conv_StringToText_ReturnValue);
        K2Node_MakeStruct_SlateColor.SpecifiedColor = UKismetMathLibrary::SelectColor(FLinearColor(0.19807f, 0.58408f, 0.15896f, 1.0f), FLinearColor(0.90466f, 0.83077f, 0.63076f, 1.0f), UKismetMathLibrary::Greater_IntInt(CallFunc_ApplyCode_Applied, 0));
        K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
        StatusText->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
        StatusBox->SetVisibility(ESlateVisibility::Visible);
        Data->SlotInfo(CodeClass, CallFunc_SlotInfo_Count, CallFunc_SlotInfo_Current);
        CurrentSlot = CallFunc_SlotInfo_Current;
        UpdateSlots();
        CancelBtnText->SetText("CLOSE");
        return;
    } else {
        return;
    }
}

}   // namespace UI
