// Decompiled from /Game/_DwarfBuilds/UI/DB_Tip by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_Tip::Set(FText Title, FText Category, FText Body, FLinearColor Accent, UTexture2D* Icon) {
    bool CallFunc_IsValid_ReturnValue;
    ESlateVisibility K2Node_Select_Default;
    TitleText->SetText(Title);
    CategoryText->SetText(Category);
    BodyText->SetText(Body);
    BodyText->SetVisibility(!UKismetTextLibrary::TextIsEmpty(Body) == false ? ESlateVisibility::Collapsed : !UKismetTextLibrary::TextIsEmpty(Body) == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Icon);
    if (CallFunc_IsValid_ReturnValue) {
        TipIcon->SetBrushFromTexture(Icon, true);
        TileBg->SetColorAndOpacity(Accent);
        TileBox->SetVisibility(ESlateVisibility::HitTestInvisible);
        return;
    } else {
        TileBox->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
}

void DB_Tip::SetExtra(FText ExtraTitle, FText Extra) {
    ESlateVisibility K2Node_Select_Default;
    HasExtra = !UKismetTextLibrary::TextIsEmpty(Extra);
    ExtraTitleText->SetText(ExtraTitle);
    ExtraTitleText->SetVisibility(!UKismetTextLibrary::TextIsEmpty(ExtraTitle) == false ? ESlateVisibility::Collapsed : !UKismetTextLibrary::TextIsEmpty(ExtraTitle) == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default);
    ExtraBodyText->SetText(Extra);
    ApplyExtra();
    return;
}

void DB_Tip::ShowExtra(bool On) {
    ExtraOn = On;
    ApplyExtra();
    return;
}

void DB_Tip::ApplyExtra() {
    FText CallFunc_Conv_StringToText_ReturnValue;
    ESlateVisibility K2Node_Select_Default;
    ESlateVisibility K2Node_Select_Default_1;
    ExtraBox->SetVisibility((HasExtra && ExtraOn) == false ? ESlateVisibility::Collapsed : (HasExtra && ExtraOn) == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default_1);
    Footer->SetVisibility(HasExtra == false ? ESlateVisibility::Collapsed : HasExtra == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default);
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(UKismetMathLibrary::SelectString("LESS INFO", "MORE INFO", ExtraOn));
    MoreText->SetText(CallFunc_Conv_StringToText_ReturnValue);
    return;
}

}   // namespace UI
