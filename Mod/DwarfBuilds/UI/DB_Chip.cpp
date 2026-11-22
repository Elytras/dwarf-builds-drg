// Decompiled from /Game/_DwarfBuilds/UI/DB_Chip by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_Chip::BndEvt__DB_Chip_Btn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature() {
    bool CallFunc_IsValid_ReturnValue;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Window);
    if (CallFunc_IsValid_ReturnValue) {
        Window->SelectPlayer(Index);
        return;
    } else {
        return;
    }
}

void DB_Chip::Set(FString PlayerName, FLinearColor Color, bool Selected, bool IsYou) {
    FText CallFunc_Conv_StringToText_ReturnValue;
    bool CallFunc_EqualEqual_StrStr_ReturnValue;
    FSlateColor K2Node_MakeStruct_SlateColor;
    CallFunc_EqualEqual_StrStr_ReturnValue = UKismetStringLibrary::EqualEqual_StrStr(UKismetTextLibrary::Conv_TextToString(NameText->GetText()), PlayerName);
    if (!CallFunc_EqualEqual_StrStr_ReturnValue) {
        CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(PlayerName);
        NameText->SetText(CallFunc_Conv_StringToText_ReturnValue);
        CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(PlayerName);
        Btn->SetToolTipText(CallFunc_Conv_StringToText_ReturnValue);
    }
    IconImage->SetColorAndOpacity(UKismetMathLibrary::SelectColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), Color, Selected));
    Bg->SetBrushColor(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), FLinearColor(0.0f, 0.0f, 0.0f, 0.4f), Selected));
    Bar->SetColorAndOpacity(UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.645482f, 0.0f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), Selected));
    K2Node_MakeStruct_SlateColor.SpecifiedColor = UKismetMathLibrary::SelectColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), UKismetMathLibrary::SelectColor(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), FLinearColor(0.90466f, 0.83077f, 0.63076f, 1.0f), IsYou), Selected);
    K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
    NameText->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
    return;
}

void DB_Chip::SetIcon(UTexture2D* Texture) {
    bool CallFunc_IsValid_ReturnValue;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Texture);
    if (CallFunc_IsValid_ReturnValue) {
        IconImage->SetBrushFromTexture(Texture, false);
        return;
    } else {
        IconImage->SetBrushFromTexture(nullptr, false);
        return;
    }
}

void DB_Chip::SetWidth(float Width) {
    Box->SetWidthOverride(Width);
    return;
}

}   // namespace UI
