// Decompiled from /Game/_DwarfBuilds/UI/DB_SlotBtn by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_SlotBtn::BndEvt__DB_SlotBtn_Btn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature() {
    bool CallFunc_IsValid_ReturnValue;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Dialog);
    if (CallFunc_IsValid_ReturnValue) {
        Dialog->PickSlot(Index);
        return;
    } else {
        return;
    }
}

void DB_SlotBtn::Set(FString Label, bool Selected, bool Current) {
    FText CallFunc_Conv_StringToText_ReturnValue;
    FLinearColor CallFunc_SelectColor_ReturnValue_4;
    FSlateColor K2Node_MakeStruct_SlateColor;
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(Label);
    LabelText->SetText(CallFunc_Conv_StringToText_ReturnValue);
    Outer->SetBrushColor(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), FLinearColor(0.34f, 0.11f, 0.0f, 1.0f), Selected), Current));
    CallFunc_SelectColor_ReturnValue_4 = UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), FLinearColor(0.012f, 0.008f, 0.005f, 1.0f), Selected);
    Ring->SetBrushColor(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), CallFunc_SelectColor_ReturnValue_4, Current));
    CallFunc_SelectColor_ReturnValue_4 = UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), FLinearColor(0.012f, 0.008f, 0.005f, 1.0f), Selected);
    Fill->SetBrushColor(CallFunc_SelectColor_ReturnValue_4);
    K2Node_MakeStruct_SlateColor.SpecifiedColor = UKismetMathLibrary::SelectColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), FLinearColor(0.90466f, 0.83077f, 0.63076f, 1.0f), Current), Selected);
    K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
    LabelText->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
    return;
}

}   // namespace UI
