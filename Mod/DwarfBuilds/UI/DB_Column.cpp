// Decompiled from /Game/_DwarfBuilds/UI/DB_Column by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

void DB_Column::BndEvt__DB_Column_CopyBtn_K2Node_ComponentBoundEvent_3_OnButtonReleasedEvent__DelegateSignature() {
    CopyBtnBarL->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    CopyBtnBarR->SetColorAndOpacity(FLinearColor(0.2f, 0.11f, 0.055f, 1.0f));
    CopyBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.85f, 0.76f, 0.55f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_Column::BndEvt__DB_Column_CopyBtn_K2Node_ComponentBoundEvent_2_OnButtonPressedEvent__DelegateSignature() {
    CopyBtnBarL->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    CopyBtnBarR->SetColorAndOpacity(FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f));
    CopyBtnText->SetColorAndOpacity(FSlateColor(FLinearColor(0.01f, 0.01f, 0.01f, 1.0f), ESlateColorStylingMode::UseColor_Specified));
    return;
}

void DB_Column::BndEvt__DB_Column_CopyBtn_K2Node_ComponentBoundEvent_1_OnButtonClickedEvent__DelegateSignature() {
    FTimerHandle CallFunc_K2_SetTimer_ReturnValue;
    bool CallFunc_NotEqual_StrStr_ReturnValue_1;
    CallFunc_NotEqual_StrStr_ReturnValue_1 = UKismetStringLibrary::NotEqual_StrStr(Code, "");
    if (CallFunc_NotEqual_StrStr_ReturnValue_1) {
        UFrontendBlueprintLibrary::CopyStringToClipboard(Code);
        CopyBtnText->SetText("COPIED");
        CallFunc_K2_SetTimer_ReturnValue = UKismetSystemLibrary::K2_SetTimer(this, "CopyReset", 1.5f, false, 0.0f, 0.0f);
        return;
    } else {
        return;
    }
}

void DB_Column::BndEvt__DB_Column_TakeBtn_K2Node_ComponentBoundEvent_0_OnButtonClickedEvent__DelegateSignature() {
    bool CallFunc_BooleanAND_ReturnValue;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetSystemLibrary::IsValid(Window), UKismetStringLibrary::NotEqual_StrStr(Code, ""));
    if (CallFunc_BooleanAND_ReturnValue) {
        Window->OpenTake(Code, TakeTitle);
        return;
    } else {
        return;
    }
}

// Whether nothing ShowPlayer shows of Player can have changed since its last call: the same player, class, name,
// level, host and "you" flags, loadout, replicated selections, perks and stats, the same host perk table, and the hexagon
// textures loaded as before. A column is also redone every FreshFor seconds, for what no input here covers (a
// client's "waiting for the host" turning into "not available"). Records the inputs for the next call.
bool DB_Column::SameAsLast(AFSDPlayerState* Player) {
    const float FreshFor = 10.0f;
    float Now = UGameplayStatics::GetRealTimeSeconds(this);
    UPlayerCharacterID* Class = Player->GetSelectedCharacterID();
    FString Name = Player->GetPlayerName();
    int32 Level = Player->GetSelectedCharacterLevel();
    bool Host = Player->bIsServer;
    AFSDPlayerState* Local;
    Data->LocalPS(Local);
    bool You = Player == Local;
    bool Hex = UKismetSystemLibrary::IsValid(Data->HexBg);
    int32 Loadout = -1;
    UFSDSaveGame* Save = UGameFunctionLibrary::GetFSDSaveGame(this);
    if (UKismetSystemLibrary::IsValid(Save) && UKismetSystemLibrary::IsValid(Class))
        Loadout = Save->GetSelectedLoadoutIndex(Class);
    bool Same = Player == FpPlayer && Class == FpClass && Name == FpName && Level == FpLevel && Host == FpHost && You == FpYou &&
                Hex == FpHex && Loadout == FpLoadout && Now - FpAt < FreshFor;
    USaveGameStateComponent* Comp = Player->SaveGameStateComponent;
    if (UKismetSystemLibrary::IsValid(Comp)) {
        Same = Same && Comp->ItemUpgradeSelections.Identical(FpSels) && Comp->EquippedPerks.Identical(FpPerks) &&
               Comp->CharacterStats.Identical(FpStats);
        FpSels = Comp->ItemUpgradeSelections;
        FpPerks = Comp->EquippedPerks;
        FpStats = Comp->CharacterStats;
    } else {
        Same = Same && FpSels.Num() == 0 && FpPerks.Num() == 0;
        FpSels.Clear();
        FpPerks.Clear();
        FpStats.Clear();
    }
    DB_PerkShare_v1* Share = Data->PerksOf_Share;
    if (UKismetSystemLibrary::IsValid(Share)) {
        Same = Same && Share->Ids.Identical(FpShareIds) && Share->Classes.Identical(FpShareClasses) &&
               Share->Perks.Identical(FpSharePerks) && Share->Ranks.Identical(FpShareRanks) &&
               Share->Counts.Identical(FpShareCounts);
        FpShareClasses = Share->Classes;
        FpShareCounts = Share->Counts;
        FpShareIds = Share->Ids;
        FpSharePerks = Share->Perks;
        FpShareRanks = Share->Ranks;
    }
    // The player's link (DB_BuildShare_v2): its code and whether its perks count.
    DB_BuildShare_v2* Link = Data->LinkFor(Player);
    FString LinkSig = Link != nullptr ? Link->Code + (Link->PerksKnown ? "+" : "-") : FString("");
    Same = Same && LinkSig == FpLink;
    FpLink = LinkSig;
    if (!Same) {
        FpPlayer = Player;
        FpClass = Class;
        FpName = Name;
        FpLevel = Level;
        FpHost = Host;
        FpYou = You;
        FpHex = Hex;
        FpLoadout = Loadout;
        FpAt = Now;
    }
    return Same;
}

void DB_Column::ShowPlayer(AFSDPlayerState* Player) {
    int32 CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    bool CallFunc_BooleanAND_ReturnValue;
    FString CallFunc_CodeOf_Code;
    FString CallFunc_Concat_StrStr_ReturnValue;
    FString CallFunc_Conv_IntToString_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    bool CallFunc_EqualEqual_StrStr_ReturnValue;
    FLinearColor CallFunc_HeaderOf_Color;
    UTexture2D* CallFunc_HeaderOf_Icon;
    bool CallFunc_HeaderOf_IsHost;
    bool CallFunc_HeaderOf_IsYou;
    FString CallFunc_HeaderOf_Name;
    FString CallFunc_HeaderOf_Sub;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_NotEqual_StrStr_ReturnValue;
    bool CallFunc_Not_PreBool_ReturnValue;
    TArray<UPerkAsset*> CallFunc_PerksOf_Perks;
    TArray<int32> CallFunc_PerksOf_Ranks;
    int32 CallFunc_PerksOf_State;
    ESlateVisibility K2Node_Select_Default;
    ESlateVisibility K2Node_Select_Default_1;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    ESlateVisibility Temp_byte_Variable_2;
    ESlateVisibility Temp_byte_Variable_3;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Player);
    CallFunc_BooleanAND_ReturnValue = UKismetSystemLibrary::IsValid(Data) && CallFunc_IsValid_ReturnValue;
    if (CallFunc_BooleanAND_ReturnValue) {
        if (SameAsLast(Player)) return;
        Data->CodeOf(Player, CallFunc_CodeOf_Code);
        Data->HeaderOf(Player, CallFunc_HeaderOf_Name, CallFunc_HeaderOf_Sub, CallFunc_HeaderOf_Color, CallFunc_HeaderOf_Icon, CallFunc_HeaderOf_IsYou, CallFunc_HeaderOf_IsHost);
        Data->PerksOf(Player, CallFunc_PerksOf_Perks, CallFunc_PerksOf_Ranks, CallFunc_PerksOf_State);
        ColPerks = CallFunc_PerksOf_Perks;
        ColRanks = CallFunc_PerksOf_Ranks;
        SigWork = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr("P|", CallFunc_CodeOf_Code), "|"), CallFunc_HeaderOf_Name), "|"), CallFunc_HeaderOf_Sub), "|"), UKismetStringLibrary::Conv_BoolToString(CallFunc_HeaderOf_IsYou)), UKismetStringLibrary::Conv_BoolToString(CallFunc_HeaderOf_IsHost)), "|"), UKismetStringLibrary::Conv_IntToString(CallFunc_PerksOf_State)), "|"), UKismetStringLibrary::Conv_BoolToString(UKismetSystemLibrary::IsValid(Data->HexBg)));
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ColPerks.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                ColRanks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                CallFunc_Conv_IntToString_ReturnValue = UKismetStringLibrary::Conv_IntToString(CallFunc_Array_Get_Item);
                CallFunc_Concat_StrStr_ReturnValue = UKismetStringLibrary::Concat_StrStr(SigWork, "|");
                ColPerks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                SigWork = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue, UKismetSystemLibrary::GetObjectName(CallFunc_Array_Get_Item_1)), ":"), CallFunc_Conv_IntToString_ReturnValue);
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                CallFunc_EqualEqual_StrStr_ReturnValue = UKismetStringLibrary::EqualEqual_StrStr(SigWork, LastSig);
                if (CallFunc_EqualEqual_StrStr_ReturnValue) {
                    return;
                } else {
                    LastSig = SigWork;
                    Code = CallFunc_CodeOf_Code;
                    TakeTitle = CallFunc_HeaderOf_Name;
                    PushData();
                    ShowHeader(CallFunc_HeaderOf_Name, CallFunc_HeaderOf_Sub, CallFunc_HeaderOf_Color, CallFunc_HeaderOf_Icon, CallFunc_HeaderOf_IsYou, CallFunc_HeaderOf_IsHost);
                    ShowBuild(CallFunc_CodeOf_Code);
                    CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerksOf_State, 0);
                    if (CallFunc_EqualEqual_IntInt_ReturnValue_1) {
                        ShowPerks();
                    } else {
                        ShowNote(CallFunc_PerksOf_State == 1 ? FString("Not available. The host does not run this mod.") :
                                 CallFunc_PerksOf_State == 3 ? FString("Not available. The game does not share this player's perks.") :
                                 FString("Waiting for the host..."));
                    }
                    Temp_byte_Variable_2 = ESlateVisibility::SelfHitTestInvisible;
                    Temp_byte_Variable_3 = ESlateVisibility::Collapsed;
                    CallFunc_Not_PreBool_ReturnValue = !CallFunc_HeaderOf_IsYou;
                    TakeBtnSize->SetVisibility(CallFunc_Not_PreBool_ReturnValue == false ? Temp_byte_Variable_3 : CallFunc_Not_PreBool_ReturnValue == true ? Temp_byte_Variable_2 : K2Node_Select_Default_1);
                    Temp_byte_Variable = ESlateVisibility::Visible;
                    Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
                    CallFunc_Not_PreBool_ReturnValue = !CallFunc_HeaderOf_IsYou;
                    TakeGap->SetVisibility(CallFunc_Not_PreBool_ReturnValue == false ? Temp_byte_Variable_1 : CallFunc_Not_PreBool_ReturnValue == true ? Temp_byte_Variable : K2Node_Select_Default);
                    CallFunc_NotEqual_StrStr_ReturnValue = UKismetStringLibrary::NotEqual_StrStr(CallFunc_CodeOf_Code, "");
                    TakeBtnWrap->SetIsEnabled(CallFunc_NotEqual_StrStr_ReturnValue);
                    CallFunc_NotEqual_StrStr_ReturnValue = UKismetStringLibrary::NotEqual_StrStr(CallFunc_CodeOf_Code, "");
                    CopyBtnWrap->SetIsEnabled(CallFunc_NotEqual_StrStr_ReturnValue);
                    HasButtons = true;
                    ApplyButtons();
                    Body->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
                    return;
                }
            }
        }
    } else {
        Clear();
        return;
    }
}

void DB_Column::ShowCode(FString NewCode, FString Title) {
    bool CallFunc_BooleanAND_ReturnValue;
    FString CallFunc_Concat_StrStr_ReturnValue;
    FString CallFunc_Concat_StrStr_ReturnValue_1;
    FString CallFunc_Concat_StrStr_ReturnValue_2;
    FString CallFunc_Concat_StrStr_ReturnValue_3;
    FString CallFunc_Concat_StrStr_ReturnValue_4;
    FString CallFunc_Conv_BoolToString_ReturnValue;
    FLinearColor CallFunc_HeaderOfCode_Color;
    UTexture2D* CallFunc_HeaderOfCode_Icon;
    FString CallFunc_HeaderOfCode_Sub;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_PerksOfCode_Found;
    TArray<UPerkAsset*> CallFunc_PerksOfCode_Perks;
    TArray<int32> CallFunc_PerksOfCode_Ranks;
    CallFunc_Concat_StrStr_ReturnValue = UKismetStringLibrary::Concat_StrStr("C|", NewCode);
    CallFunc_Concat_StrStr_ReturnValue_1 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue, "|");
    CallFunc_Concat_StrStr_ReturnValue_2 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_1, Title);
    CallFunc_Concat_StrStr_ReturnValue_3 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_2, "|");
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Data->HexBg);
    CallFunc_Conv_BoolToString_ReturnValue = UKismetStringLibrary::Conv_BoolToString(CallFunc_IsValid_ReturnValue_1);
    CallFunc_Concat_StrStr_ReturnValue_4 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_3, CallFunc_Conv_BoolToString_ReturnValue);
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(CallFunc_IsValid_ReturnValue, UKismetStringLibrary::NotEqual_StrStr(CallFunc_Concat_StrStr_ReturnValue_4, LastSig));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_Concat_StrStr_ReturnValue = UKismetStringLibrary::Concat_StrStr("C|", NewCode);
        CallFunc_Concat_StrStr_ReturnValue_1 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue, "|");
        CallFunc_Concat_StrStr_ReturnValue_2 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_1, Title);
        CallFunc_Concat_StrStr_ReturnValue_3 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_2, "|");
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Data->HexBg);
        CallFunc_Conv_BoolToString_ReturnValue = UKismetStringLibrary::Conv_BoolToString(CallFunc_IsValid_ReturnValue_1);
        CallFunc_Concat_StrStr_ReturnValue_4 = UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue_3, CallFunc_Conv_BoolToString_ReturnValue);
        LastSig = CallFunc_Concat_StrStr_ReturnValue_4;
        FpPlayer = nullptr;
        Code = NewCode;
        TakeTitle = Title;
        PushData();
        Data->HeaderOfCode(NewCode, CallFunc_HeaderOfCode_Sub, CallFunc_HeaderOfCode_Color, CallFunc_HeaderOfCode_Icon);
        ShowHeader(Title, CallFunc_HeaderOfCode_Sub, CallFunc_HeaderOfCode_Color, CallFunc_HeaderOfCode_Icon, false, false);
        ShowBuild(NewCode);
        Data->PerksOfCode(NewCode, CallFunc_PerksOfCode_Perks, CallFunc_PerksOfCode_Ranks, CallFunc_PerksOfCode_Found);
        ColPerks = CallFunc_PerksOfCode_Perks;
        ColRanks = CallFunc_PerksOfCode_Ranks;
        if (CallFunc_PerksOfCode_Found) {
            ShowPerks();
        } else {
            ShowNote("No perks in this code.");
        }
        HasButtons = false;
        ApplyButtons();
        Body->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        return;
    } else {
        return;
    }
}

void DB_Column::Clear() {
    Body->SetVisibility(ESlateVisibility::Hidden);
    FpPlayer = nullptr;
    LastSig = "";
    Code = "";
    TakeTitle = "";
    return;
}

void DB_Column::SetCompact(bool On) {
    Compact = On;
    Root->SetHeightOverride(UKismetMathLibrary::SelectFloat(940.0f, 990.0f, On));
    ApplyButtons();
    return;
}

void DB_Column::PushData() {
    Card1->Data = Data;
    Card2->Data = Data;
    Gear1->Data = Data;
    Gear2->Data = Data;
    Gear3->Data = Data;
    Gear4->Data = Data;
    P1->Data = Data;
    P2->Data = Data;
    P3->Data = Data;
    A1->Data = Data;
    A2->Data = Data;
    F1->Data = Data;
    return;
}

void DB_Column::ApplyButtons() {
    ESlateVisibility K2Node_Select_Default;
    ESlateVisibility K2Node_Select_Default_1;
    BtnRow->SetVisibility(Compact == false ? (HasButtons == false ? ESlateVisibility::Hidden : HasButtons == true ? ESlateVisibility::SelfHitTestInvisible : K2Node_Select_Default) : Compact == true ? ESlateVisibility::Collapsed : K2Node_Select_Default_1);
    return;
}

void DB_Column::ShowHeader(FString HeadName, FString HeadSub, FLinearColor HeadColor, UTexture2D* HeadIcon, bool HeadYou, bool HeadHost) {
    FText CallFunc_Conv_StringToText_ReturnValue;
    FText CallFunc_Conv_StringToText_ReturnValue_1;
    FSlateColor K2Node_MakeStruct_SlateColor;
    ESlateVisibility K2Node_Select_Default;
    ESlateVisibility K2Node_Select_Default_1;
    ESlateVisibility K2Node_Select_Default_2;
    bool Temp_bool_Variable_2;
    CallFunc_Conv_StringToText_ReturnValue_1 = UKismetTextLibrary::Conv_StringToText(HeadName);
    NameText->SetText(CallFunc_Conv_StringToText_ReturnValue_1);
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(HeadSub);
    SubText->SetText(CallFunc_Conv_StringToText_ReturnValue);
    K2Node_MakeStruct_SlateColor.SpecifiedColor = HeadColor;
    K2Node_MakeStruct_SlateColor.ColorUseRule = ESlateColorStylingMode::UseColor_Specified;
    SubText->SetColorAndOpacity(K2Node_MakeStruct_SlateColor);
    ClassRule->SetColorAndOpacity(HeadColor);
    ClassIcon->SetBrushFromTexture(HeadIcon, false);
    ClassIcon->SetColorAndOpacity(HeadColor);
    Temp_bool_Variable_2 = UKismetSystemLibrary::IsValid(HeadIcon);
    ClassIcon->SetVisibility(Temp_bool_Variable_2 == false ? ESlateVisibility::Hidden : Temp_bool_Variable_2 == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default_2);
    TagYou->SetVisibility(HeadYou == false ? ESlateVisibility::Collapsed : HeadYou == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default_1);
    TagHost->SetVisibility(HeadHost == false ? ESlateVisibility::Collapsed : HeadHost == true ? ESlateVisibility::HitTestInvisible : K2Node_Select_Default);
    return;
}

void DB_Column::ShowBuild(FString BuildCode) {
    TArray<int32> CallFunc_DecodeSlot_Choices;
    TArray<int32> CallFunc_DecodeSlot_Choices_1;
    TArray<int32> CallFunc_DecodeSlot_Choices_2;
    TArray<int32> CallFunc_DecodeSlot_Choices_3;
    TArray<int32> CallFunc_DecodeSlot_Choices_4;
    TArray<int32> CallFunc_DecodeSlot_Choices_5;
    FString CallFunc_DecodeSlot_Digits;
    FString CallFunc_DecodeSlot_Digits_1;
    FString CallFunc_DecodeSlot_Digits_2;
    FString CallFunc_DecodeSlot_Digits_3;
    FString CallFunc_DecodeSlot_Digits_4;
    FString CallFunc_DecodeSlot_Digits_5;
    bool CallFunc_DecodeSlot_Found;
    bool CallFunc_DecodeSlot_Found_1;
    bool CallFunc_DecodeSlot_Found_2;
    bool CallFunc_DecodeSlot_Found_3;
    bool CallFunc_DecodeSlot_Found_4;
    bool CallFunc_DecodeSlot_Found_5;
    UItemID* CallFunc_DecodeSlot_Item;
    UItemID* CallFunc_DecodeSlot_Item_1;
    UItemID* CallFunc_DecodeSlot_Item_2;
    UItemID* CallFunc_DecodeSlot_Item_3;
    UItemID* CallFunc_DecodeSlot_Item_4;
    UItemID* CallFunc_DecodeSlot_Item_5;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock_1;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock_2;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock_3;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock_4;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock_5;
    bool CallFunc_DecodeSlot_UnknownOc;
    bool CallFunc_DecodeSlot_UnknownOc_1;
    bool CallFunc_DecodeSlot_UnknownOc_2;
    bool CallFunc_DecodeSlot_UnknownOc_3;
    bool CallFunc_DecodeSlot_UnknownOc_4;
    bool CallFunc_DecodeSlot_UnknownOc_5;
    UItemID* K2Node_Select_Default;
    UItemID* K2Node_Select_Default_1;
    UItemID* K2Node_Select_Default_2;
    UItemID* K2Node_Select_Default_3;
    UItemID* K2Node_Select_Default_4;
    UItemID* K2Node_Select_Default_5;
    Data->DecodeSlot(BuildCode, 0, CallFunc_DecodeSlot_Item_5, CallFunc_DecodeSlot_Choices_5, CallFunc_DecodeSlot_Overclock_5, CallFunc_DecodeSlot_Digits_5, CallFunc_DecodeSlot_Found_5, CallFunc_DecodeSlot_UnknownOc_5);
    Card1->Show(CallFunc_DecodeSlot_Found_5 == false ? nullptr : CallFunc_DecodeSlot_Found_5 == true ? CallFunc_DecodeSlot_Item_5 : K2Node_Select_Default_5, CallFunc_DecodeSlot_Choices_5, CallFunc_DecodeSlot_Overclock_5, "", CallFunc_DecodeSlot_UnknownOc_5);
    Data->DecodeSlot(BuildCode, 1, CallFunc_DecodeSlot_Item_4, CallFunc_DecodeSlot_Choices_4, CallFunc_DecodeSlot_Overclock_4, CallFunc_DecodeSlot_Digits_4, CallFunc_DecodeSlot_Found_4, CallFunc_DecodeSlot_UnknownOc_4);
    Card2->Show(CallFunc_DecodeSlot_Found_4 == false ? nullptr : CallFunc_DecodeSlot_Found_4 == true ? CallFunc_DecodeSlot_Item_4 : K2Node_Select_Default_4, CallFunc_DecodeSlot_Choices_4, CallFunc_DecodeSlot_Overclock_4, "", CallFunc_DecodeSlot_UnknownOc_4);
    Data->DecodeSlot(BuildCode, 2, CallFunc_DecodeSlot_Item_3, CallFunc_DecodeSlot_Choices_3, CallFunc_DecodeSlot_Overclock_3, CallFunc_DecodeSlot_Digits_3, CallFunc_DecodeSlot_Found_3, CallFunc_DecodeSlot_UnknownOc_3);
    Gear1->Show("TRAVERSAL", CallFunc_DecodeSlot_Found_3 == false ? nullptr : CallFunc_DecodeSlot_Found_3 == true ? CallFunc_DecodeSlot_Item_3 : K2Node_Select_Default_3, CallFunc_DecodeSlot_Choices_3);
    Data->DecodeSlot(BuildCode, 3, CallFunc_DecodeSlot_Item_2, CallFunc_DecodeSlot_Choices_2, CallFunc_DecodeSlot_Overclock_2, CallFunc_DecodeSlot_Digits_2, CallFunc_DecodeSlot_Found_2, CallFunc_DecodeSlot_UnknownOc_2);
    Gear2->Show("SUPPORT", CallFunc_DecodeSlot_Found_2 == false ? nullptr : CallFunc_DecodeSlot_Found_2 == true ? CallFunc_DecodeSlot_Item_2 : K2Node_Select_Default_2, CallFunc_DecodeSlot_Choices_2);
    Data->DecodeSlot(BuildCode, 4, CallFunc_DecodeSlot_Item_1, CallFunc_DecodeSlot_Choices_1, CallFunc_DecodeSlot_Overclock_1, CallFunc_DecodeSlot_Digits_1, CallFunc_DecodeSlot_Found_1, CallFunc_DecodeSlot_UnknownOc_1);
    Gear3->Show("ARMOR", CallFunc_DecodeSlot_Found_1 == false ? nullptr : CallFunc_DecodeSlot_Found_1 == true ? CallFunc_DecodeSlot_Item_1 : K2Node_Select_Default_1, CallFunc_DecodeSlot_Choices_1);
    Data->DecodeSlot(BuildCode, 5, CallFunc_DecodeSlot_Item, CallFunc_DecodeSlot_Choices, CallFunc_DecodeSlot_Overclock, CallFunc_DecodeSlot_Digits, CallFunc_DecodeSlot_Found, CallFunc_DecodeSlot_UnknownOc);
    Gear4->Show("GRENADE", CallFunc_DecodeSlot_Found == false ? nullptr : CallFunc_DecodeSlot_Found == true ? CallFunc_DecodeSlot_Item : K2Node_Select_Default, CallFunc_DecodeSlot_Choices);
    return;
}

void DB_Column::ShowPerks() {
    DB_PerkTile* CallFunc_Array_Get_Item;
    int32 CallFunc_Array_Get_Item_1;
    int32 CallFunc_Array_Get_Item_10;
    UPerkAsset* CallFunc_Array_Get_Item_11;
    UPerkAsset* CallFunc_Array_Get_Item_2;
    DB_PerkTile* CallFunc_Array_Get_Item_3;
    int32 CallFunc_Array_Get_Item_4;
    UPerkAsset* CallFunc_Array_Get_Item_5;
    DB_PerkTile* CallFunc_Array_Get_Item_6;
    DB_PerkTile* CallFunc_Array_Get_Item_7;
    DB_PerkTile* CallFunc_Array_Get_Item_8;
    DB_PerkTile* CallFunc_Array_Get_Item_9;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_IsUsageType_ReturnValue;
    bool CallFunc_IsUsageType_ReturnValue_2;
    bool CallFunc_IsUsageType_ReturnValue_4;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    bool CallFunc_LessEqual_IntInt_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue_3;
    bool CallFunc_Less_IntInt_ReturnValue_4;
    bool CallFunc_Less_IntInt_ReturnValue_5;
    TArray<DB_PerkTile*> K2Node_MakeArray_Array;
    TArray<DB_PerkTile*> K2Node_MakeArray_Array_1;
    TArray<DB_PerkTile*> K2Node_MakeArray_Array_2;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Array_Index_Variable_2;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    int32 Temp_int_Loop_Counter_Variable_2;
    int32 Temp_int_Variable;
    int32 Temp_int_Variable_1;
    int32 Temp_int_Variable_2;
    CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(Tiles0.Num(), 0);
    if (CallFunc_EqualEqual_IntInt_ReturnValue) {
        K2Node_MakeArray_Array_2 = {P1, P2, P3};
        Tiles0 = K2Node_MakeArray_Array_2;
        K2Node_MakeArray_Array_1 = {A1, A2};
        Tiles1 = K2Node_MakeArray_Array_1;
        K2Node_MakeArray_Array = {F1};
        Tiles2 = K2Node_MakeArray_Array;
    }
    N0 = 0;
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable_2 = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue_5 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ColPerks.Num());
        if (CallFunc_Less_IntInt_ReturnValue_5) {
            Temp_int_Array_Index_Variable_2 = Temp_int_Loop_Counter_Variable;
            ColPerks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_11);
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_11);
            if (CallFunc_IsValid_ReturnValue_2) {
                ColPerks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_11);
                CallFunc_IsUsageType_ReturnValue_4 = CallFunc_Array_Get_Item_11->IsUsageType(EPerkUsageType::Passive);
                if (CallFunc_IsUsageType_ReturnValue_4) {
                    CallFunc_Less_IntInt_ReturnValue_4 = UKismetMathLibrary::Less_IntInt(N0, 3);
                    if (CallFunc_Less_IntInt_ReturnValue_4) {
                        Tiles0.Get(N0, CallFunc_Array_Get_Item_8);
                        ColRanks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_10);
                        ColPerks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_11);
                        CallFunc_Array_Get_Item_8->Set(CallFunc_Array_Get_Item_11, CallFunc_Array_Get_Item_10);
                        N0 = UKismetMathLibrary::Add_IntInt(N0, 1);
                    }
                }
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Temp_int_Variable = N0;
            while (true) {
                CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 2);
                if (CallFunc_LessEqual_IntInt_ReturnValue) {
                    Tiles0.Get(Temp_int_Variable, CallFunc_Array_Get_Item_6);
                    CallFunc_Array_Get_Item_6->Set(nullptr, 0);
                    Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                } else {
                    N1 = 0;
                    Temp_int_Loop_Counter_Variable_2 = 0;
                    Temp_int_Array_Index_Variable = 0;
                    while (true) {
                        CallFunc_Less_IntInt_ReturnValue_3 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_2, ColPerks.Num());
                        if (CallFunc_Less_IntInt_ReturnValue_3) {
                            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable_2;
                            ColPerks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_5);
                            CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_5);
                            if (CallFunc_IsValid_ReturnValue_1) {
                                ColPerks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_5);
                                CallFunc_IsUsageType_ReturnValue_2 = CallFunc_Array_Get_Item_5->IsUsageType(EPerkUsageType::Active);
                                CallFunc_BooleanAND_ReturnValue_1 = !CallFunc_Array_Get_Item_5->IsUsageType(EPerkUsageType::Passive) && CallFunc_IsUsageType_ReturnValue_2;
                                if (CallFunc_BooleanAND_ReturnValue_1) {
                                    CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(N1, 2);
                                    if (CallFunc_Less_IntInt_ReturnValue_2) {
                                        Tiles1.Get(N1, CallFunc_Array_Get_Item_3);
                                        ColRanks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_4);
                                        ColPerks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_5);
                                        CallFunc_Array_Get_Item_3->Set(CallFunc_Array_Get_Item_5, CallFunc_Array_Get_Item_4);
                                        N1 = UKismetMathLibrary::Add_IntInt(N1, 1);
                                    }
                                }
                            }
                            Temp_int_Loop_Counter_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_2, 1);
                        } else {
                            Temp_int_Variable_2 = N1;
                            while (true) {
                                CallFunc_LessEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_2, 1);
                                if (CallFunc_LessEqual_IntInt_ReturnValue_2) {
                                    Tiles1.Get(Temp_int_Variable_2, CallFunc_Array_Get_Item_9);
                                    CallFunc_Array_Get_Item_9->Set(nullptr, 0);
                                    Temp_int_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Variable_2, 1);
                                } else {
                                    N2 = 0;
                                    Temp_int_Loop_Counter_Variable_1 = 0;
                                    Temp_int_Array_Index_Variable_1 = 0;
                                    while (true) {
                                        CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, ColPerks.Num());
                                        if (CallFunc_Less_IntInt_ReturnValue_1) {
                                            Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable_1;
                                            ColPerks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_2);
                                            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_2);
                                            if (CallFunc_IsValid_ReturnValue) {
                                                ColPerks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_2);
                                                CallFunc_IsUsageType_ReturnValue = CallFunc_Array_Get_Item_2->IsUsageType(EPerkUsageType::Active);
                                                CallFunc_BooleanAND_ReturnValue = !CallFunc_Array_Get_Item_2->IsUsageType(EPerkUsageType::Passive) && !CallFunc_IsUsageType_ReturnValue;
                                                if (CallFunc_BooleanAND_ReturnValue) {
                                                    CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(N2, 1);
                                                    if (CallFunc_Less_IntInt_ReturnValue) {
                                                        Tiles2.Get(N2, CallFunc_Array_Get_Item);
                                                        ColRanks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                                                        ColPerks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_2);
                                                        CallFunc_Array_Get_Item->Set(CallFunc_Array_Get_Item_2, CallFunc_Array_Get_Item_1);
                                                        N2 = UKismetMathLibrary::Add_IntInt(N2, 1);
                                                    }
                                                }
                                            }
                                            Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
                                        } else {
                                            Temp_int_Variable_1 = N2;
                                            while (true) {
                                                CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_1, 0);
                                                if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
                                                    Tiles2.Get(Temp_int_Variable_1, CallFunc_Array_Get_Item_7);
                                                    CallFunc_Array_Get_Item_7->Set(nullptr, 0);
                                                    Temp_int_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1);
                                                } else {
                                                    PerkTiles->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
                                                    PerkNote->SetVisibility(ESlateVisibility::Collapsed);
                                                    return;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void DB_Column::ShowNote(FString Note) {
    FText CallFunc_Conv_StringToText_ReturnValue;
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(Note);
    PerkNote->SetText(CallFunc_Conv_StringToText_ReturnValue);
    PerkNote->SetVisibility(ESlateVisibility::HitTestInvisible);
    PerkTiles->SetVisibility(ESlateVisibility::Collapsed);
    return;
}

void DB_Column::CopyReset() {
    CopyBtnText->SetText("COPY CODE");
    return;
}

}   // namespace UI
