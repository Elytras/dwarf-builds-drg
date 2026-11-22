// Decompiled from /Game/_DwarfBuilds/UI/DB_HubPage by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_HubPage::BndEvt__DB_HubPage_ResetBtn_K2Node_ComponentBoundEvent_3_OnButtonReleasedEvent__DelegateSignature() {
    ResetBtnBarL->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    ResetBtnBarR->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    ResetBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.85f, 0.76f, 0.55f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_HubPage::BndEvt__DB_HubPage_ResetBtn_K2Node_ComponentBoundEvent_2_OnButtonPressedEvent__DelegateSignature() {
    ResetBtnBarL->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    ResetBtnBarR->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    ResetBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_HubPage::BndEvt__DB_HubPage_ResetBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature() {
    bool CallFunc_IsValid_ReturnValue_1;
    FindData();
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Data);
    if (CallFunc_IsValid_ReturnValue_1) {
        Data->SetOpenKey(FKey("B"));
        ShowKey();
        Note("Reset. Press B to open the window.", FLinearColor(0.33f, 0.89f, 0.0f, 1.0f));
        return;
    } else {
        return;
    }
}

void DB_HubPage::BndEvt__DB_HubPage_KeyPick_K2Node_ComponentBoundEvent_0_OnKeySelected__DelegateSignature(FInputChord SelectedKey) {
    bool CallFunc_BooleanOR_ReturnValue;
    bool CallFunc_BooleanOR_ReturnValue_2;
    FString CallFunc_Concat_StrStr_ReturnValue_1;
    bool CallFunc_EqualEqual_KeyKey_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Key_IsMouseButton_ReturnValue;
    CallFunc_EqualEqual_KeyKey_ReturnValue = SelectedKey.Key == FKey("E");
    CallFunc_BooleanOR_ReturnValue = SelectedKey.Key == FKey("Escape") || CallFunc_EqualEqual_KeyKey_ReturnValue;
    CallFunc_Key_IsMouseButton_ReturnValue = UKismetInputLibrary::Key_IsMouseButton(SelectedKey.Key);
    CallFunc_BooleanOR_ReturnValue_2 = UKismetMathLibrary::BooleanOR(CallFunc_Key_IsMouseButton_ReturnValue || CallFunc_BooleanOR_ReturnValue, !UKismetInputLibrary::Key_IsValid(SelectedKey.Key));
    if (CallFunc_BooleanOR_ReturnValue_2) {
        CallFunc_Key_IsMouseButton_ReturnValue = UKismetInputLibrary::Key_IsMouseButton(SelectedKey.Key);
        Note(UKismetMathLibrary::SelectString("Mouse buttons cannot open the window. Pick a keyboard key.", "Escape and E are used inside the window. Pick another key.", CallFunc_Key_IsMouseButton_ReturnValue), FLinearColor(1.0f, 0.25f, 0.15f, 1.0f));
        ShowKey();
        return;
    } else {
        FindData();
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
        if (CallFunc_IsValid_ReturnValue) {
            Data->SetOpenKey(SelectedKey.Key);
            CallFunc_Concat_StrStr_ReturnValue_1 = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr("Saved. Press ", UKismetTextLibrary::Conv_TextToString(UKismetInputLibrary::Key_GetDisplayName(SelectedKey.Key))), " to open the window.");
            Note(CallFunc_Concat_StrStr_ReturnValue_1, FLinearColor(0.33f, 0.89f, 0.0f, 1.0f));
            return;
        } else {
            return;
        }
    }
}

void DB_HubPage::HubPageOpened() {
    ShowKey();
    Note("Press the key box, then the new key. Escape cancels.", FLinearColor(0.85f, 0.76f, 0.55f, 1.0f));
    return;
}

void DB_HubPage::HubPageClosed() {
    return;
}

void DB_HubPage::FindData() {
    Data = (DB_Data*)UGameplayStatics::GetActorOfClass(this, DB_Data::StaticClass());
    return;
}

void DB_HubPage::ShowKey() {
    bool CallFunc_IsValid_ReturnValue;
    FKey CallFunc_OpenKey_Key;
    FInputChord K2Node_MakeStruct_InputChord;
    FindData();
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
    if (CallFunc_IsValid_ReturnValue) {
        Data->OpenKey(CallFunc_OpenKey_Key);
        K2Node_MakeStruct_InputChord.Key = CallFunc_OpenKey_Key;
        K2Node_MakeStruct_InputChord.bShift = false;
        K2Node_MakeStruct_InputChord.bCtrl = false;
        K2Node_MakeStruct_InputChord.bAlt = false;
        K2Node_MakeStruct_InputChord.bCmd = false;
        KeyPick->SetSelectedKey(K2Node_MakeStruct_InputChord);
        return;
    } else {
        return;
    }
}

void DB_HubPage::Note(FString Note, FLinearColor Colour) {
    FText CallFunc_Conv_StringToText_ReturnValue;
    FSlateColor K2Node_MakeStruct_SlateColor;
    KeyNote->SetText(" ");
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(Note);
    KeyNote->SetText(CallFunc_Conv_StringToText_ReturnValue);
    K2Node_MakeStruct_SlateColor.SpecifiedColor = Colour;
    K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
    KeyNote->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
    return;
}

void DB_HubPage::GetPageInfo(FText& PageName) {
    PageName = "Dwarf Builds";
    return;
}

void DB_HubPage::GetContainerMode(bool& bStretchToContainer) {
    bStretchToContainer = true;
    return;
}

}   // namespace UI
