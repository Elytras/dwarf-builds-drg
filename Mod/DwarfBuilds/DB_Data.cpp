// Decompiled from /Game/_DwarfBuilds/DB_Data by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "DwarfBuilds.h"


void DB_Data::OnOcFrameUnstableLoaded(UObject* Loaded) {
    UTexture2D* K2Node_DynamicCast_AsTexture_2D;
    bool K2Node_DynamicCast_bSuccess;
    K2Node_DynamicCast_AsTexture_2D = Cast<UTexture2D>(Loaded);
    K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsTexture_2D != nullptr;
    if (K2Node_DynamicCast_bSuccess) {
        OcFrameUnstable = K2Node_DynamicCast_AsTexture_2D;
        return;
    } else {
        return;
    }
}

void DB_Data::OnOcFrameBalancedLoaded(UObject* Loaded) {
    UTexture2D* K2Node_DynamicCast_AsTexture_2D_4;
    bool K2Node_DynamicCast_bSuccess_4;
    K2Node_DynamicCast_AsTexture_2D_4 = Cast<UTexture2D>(Loaded);
    K2Node_DynamicCast_bSuccess_4 = K2Node_DynamicCast_AsTexture_2D_4 != nullptr;
    if (K2Node_DynamicCast_bSuccess_4) {
        OcFrameBalanced = K2Node_DynamicCast_AsTexture_2D_4;
        return;
    } else {
        return;
    }
}

void DB_Data::OnOcFrameCleanLoaded(UObject* Loaded) {
    UTexture2D* K2Node_DynamicCast_AsTexture_2D_3;
    bool K2Node_DynamicCast_bSuccess_3;
    K2Node_DynamicCast_AsTexture_2D_3 = Cast<UTexture2D>(Loaded);
    K2Node_DynamicCast_bSuccess_3 = K2Node_DynamicCast_AsTexture_2D_3 != nullptr;
    if (K2Node_DynamicCast_bSuccess_3) {
        OcFrameClean = K2Node_DynamicCast_AsTexture_2D_3;
        return;
    } else {
        return;
    }
}

void DB_Data::OnHexOutlineLoaded(UObject* Loaded) {
    UTexture2D* K2Node_DynamicCast_AsTexture_2D_2;
    bool K2Node_DynamicCast_bSuccess_2;
    K2Node_DynamicCast_AsTexture_2D_2 = Cast<UTexture2D>(Loaded);
    K2Node_DynamicCast_bSuccess_2 = K2Node_DynamicCast_AsTexture_2D_2 != nullptr;
    if (K2Node_DynamicCast_bSuccess_2) {
        HexOutline = K2Node_DynamicCast_AsTexture_2D_2;
        return;
    } else {
        return;
    }
}

void DB_Data::OnHexBgLoaded(UObject* Loaded) {
    UTexture2D* K2Node_DynamicCast_AsTexture_2D_1;
    bool K2Node_DynamicCast_bSuccess_1;
    K2Node_DynamicCast_AsTexture_2D_1 = Cast<UTexture2D>(Loaded);
    K2Node_DynamicCast_bSuccess_1 = K2Node_DynamicCast_AsTexture_2D_1 != nullptr;
    if (K2Node_DynamicCast_bSuccess_1) {
        HexBg = K2Node_DynamicCast_AsTexture_2D_1;
        return;
    } else {
        return;
    }
}

void DB_Data::ReceiveBeginPlay() {
    TSoftObjectPtr<UObject> CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue;
    TSoftObjectPtr<UObject> CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_1;
    TSoftObjectPtr<UObject> CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_2;
    TSoftObjectPtr<UObject> CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_3;
    TSoftObjectPtr<UObject> CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_4;
    FSoftObjectPath CallFunc_MakeSoftObjectPath_ReturnValue;
    FSoftObjectPath CallFunc_MakeSoftObjectPath_ReturnValue_1;
    FSoftObjectPath CallFunc_MakeSoftObjectPath_ReturnValue_2;
    FSoftObjectPath CallFunc_MakeSoftObjectPath_ReturnValue_3;
    FSoftObjectPath CallFunc_MakeSoftObjectPath_ReturnValue_4;
    LoadSettings();
    ShareScan();
    ShareCheck();
    CallFunc_MakeSoftObjectPath_ReturnValue = UKismetSystemLibrary::MakeSoftObjectPath("/Game/UI/Menu_Loadout/Assets/Upgrade_BG.Upgrade_BG");
    CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue = UKismetSystemLibrary::Conv_SoftObjPathToSoftObjRef(CallFunc_MakeSoftObjectPath_ReturnValue);
    UKismetSystemLibrary::LoadAsset(this, CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue, {this, &DB_Data::OnHexBgLoaded});
    CallFunc_MakeSoftObjectPath_ReturnValue_1 = UKismetSystemLibrary::MakeSoftObjectPath("/Game/UI/Menu_Loadout/Assets/Upgrade_Outline.Upgrade_Outline");
    CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_1 = UKismetSystemLibrary::Conv_SoftObjPathToSoftObjRef(CallFunc_MakeSoftObjectPath_ReturnValue_1);
    UKismetSystemLibrary::LoadAsset(this, CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_1, {this, &DB_Data::OnHexOutlineLoaded});
    CallFunc_MakeSoftObjectPath_ReturnValue_2 = UKismetSystemLibrary::MakeSoftObjectPath("/Game/UI/Art/Icons/Icons_Overclocks/Rewards/RewardFrame_OverclockClean.RewardFrame_OverclockClean");
    CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_2 = UKismetSystemLibrary::Conv_SoftObjPathToSoftObjRef(CallFunc_MakeSoftObjectPath_ReturnValue_2);
    UKismetSystemLibrary::LoadAsset(this, CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_2, {this, &DB_Data::OnOcFrameCleanLoaded});
    CallFunc_MakeSoftObjectPath_ReturnValue_3 = UKismetSystemLibrary::MakeSoftObjectPath("/Game/UI/Art/Icons/Icons_Overclocks/Rewards/RewardFrame_OverclockBalanced.RewardFrame_OverclockBalanced");
    CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_3 = UKismetSystemLibrary::Conv_SoftObjPathToSoftObjRef(CallFunc_MakeSoftObjectPath_ReturnValue_3);
    UKismetSystemLibrary::LoadAsset(this, CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_3, {this, &DB_Data::OnOcFrameBalancedLoaded});
    CallFunc_MakeSoftObjectPath_ReturnValue_4 = UKismetSystemLibrary::MakeSoftObjectPath("/Game/UI/Art/Icons/Icons_Overclocks/Rewards/RewardFrame_OverclockUnstable.RewardFrame_OverclockUnstable");
    CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_4 = UKismetSystemLibrary::Conv_SoftObjPathToSoftObjRef(CallFunc_MakeSoftObjectPath_ReturnValue_4);
    UKismetSystemLibrary::LoadAsset(this, CallFunc_Conv_SoftObjPathToSoftObjRef_ReturnValue_4, {this, &DB_Data::OnOcFrameUnstableLoaded});
    return;
}

void DB_Data::GetPlayers(TArray<AFSDPlayerState*>& Players) {
    bool CallFunc_IsValid_ReturnValue;
    GetPlayers_State = UGameFunctionLibrary::GetFSDGameState(this);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(GetPlayers_State);
    if (CallFunc_IsValid_ReturnValue) {
        GetPlayers_Players = GetPlayers_State->GetNetworkSortedPlayerArray();
    } else {
        GetPlayers_Players.Clear();
    }
    Players = GetPlayers_Players;
    return;
}

void DB_Data::CodeOf(AFSDPlayerState* Player, FString& Code) {
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    bool CallFunc_Greater_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    AFSDPlayerState* CallFunc_LocalPS_PS;
    TArray<UPerkAsset*> CallFunc_PerksOf_Perks;
    TArray<int32> CallFunc_PerksOf_Ranks;
    int32 CallFunc_PerksOf_State;
    FString CallFunc_PerksPart_Part;
    FString CallFunc_RemoteCode_Code;
    FString CallFunc_SaveCode_Code;
    CodeOf_Code = "";
    DB_BuildShare_v2* Link = LinkFor(Player);
    if (Link != nullptr) {
        Code = Link->Code;
        return;
    }
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Player);
    if (CallFunc_IsValid_ReturnValue) {
        CodeOf_Class = Player->GetSelectedCharacterID();
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CodeOf_Class);
        if (CallFunc_IsValid_ReturnValue_1) {
            LocalPS(CallFunc_LocalPS_PS);
            CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(Player, CallFunc_LocalPS_PS);
            if (CallFunc_EqualEqual_ObjectObject_ReturnValue) {
                SaveCode(CodeOf_Class, CallFunc_SaveCode_Code);
                CodeOf_Code = CallFunc_SaveCode_Code;
            } else {
                RemoteCode(Player, CodeOf_Class, CallFunc_RemoteCode_Code);
                CodeOf_Code = CallFunc_RemoteCode_Code;
            }
            CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Len(CodeOf_Code), 0);
            if (CallFunc_Greater_IntInt_ReturnValue) {
                PerksOf(Player, CallFunc_PerksOf_Perks, CallFunc_PerksOf_Ranks, CallFunc_PerksOf_State);
                CodeOf_Perks = CallFunc_PerksOf_Perks;
                CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerksOf_State, 0);
                if (CallFunc_EqualEqual_IntInt_ReturnValue) {
                    PerksPart(CodeOf_Perks, CallFunc_PerksPart_Part);
                    CodeOf_Code = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(CodeOf_Code, ":"), CallFunc_PerksPart_Part);
                } else {
                    CodeOf_Code = UKismetStringLibrary::Concat_StrStr(CodeOf_Code, ":");
                }
            }
        }
    }
    Code = CodeOf_Code;
    return;
}

void DB_Data::LocalCode(FString& Code) {
    FString CallFunc_CodeOf_Code;
    bool CallFunc_IsValid_ReturnValue;
    AFSDPlayerState* CallFunc_LocalPS_PS;
    LocalPS(CallFunc_LocalPS_PS);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_LocalPS_PS);
    if (CallFunc_IsValid_ReturnValue) {
        CodeOf(CallFunc_LocalPS_PS, CallFunc_CodeOf_Code);
        LocalCode_Code = CallFunc_CodeOf_Code;
    } else {
        LocalCode_Code = "";
    }
    Code = LocalCode_Code;
    return;
}

void DB_Data::HeaderOf(AFSDPlayerState* Player, FString& Name, FString& Sub, FLinearColor& Color, UTexture2D*& Icon, bool& IsYou, bool& IsHost) {
    FString CallFunc_ClassLetter_Letter;
    FString CallFunc_ClassLetter_Name;
    FCharacterProgress CallFunc_GetCharacterStat_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    AFSDPlayerState* CallFunc_LocalPS_PS;
    HeaderOf_Name = "";
    HeaderOf_Sub = "";
    HeaderOf_Color = FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f);
    HeaderOf_Icon = nullptr;
    HeaderOf_IsYou = false;
    HeaderOf_IsHost = false;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Player);
    if (CallFunc_IsValid_ReturnValue_1) {
        HeaderOf_Name = Player->GetPlayerName();
        HeaderOf_IsHost = Player->bIsServer;
        LocalPS(CallFunc_LocalPS_PS);
        HeaderOf_IsYou = UKismetMathLibrary::EqualEqual_ObjectObject(Player, CallFunc_LocalPS_PS);
        HeaderOf_Class = Player->GetSelectedCharacterID();
        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(HeaderOf_Class);
        if (CallFunc_IsValid_ReturnValue_2) {
            ClassLetter(HeaderOf_Class, CallFunc_ClassLetter_Letter, CallFunc_ClassLetter_Name);
            HeaderOf_Promos = 0;
            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Player->SaveGameStateComponent);
            if (CallFunc_IsValid_ReturnValue) {
                CallFunc_GetCharacterStat_ReturnValue = Player->SaveGameStateComponent->GetCharacterStat(HeaderOf_Class);
                HeaderOf_Promos = CallFunc_GetCharacterStat_ReturnValue.TimesRetired;
            }
            HeaderOf_Sub = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(CallFunc_ClassLetter_Name, L" · LV "), UKismetStringLibrary::Conv_IntToString(Player->GetSelectedCharacterLevel())), L" · "), UKismetMathLibrary::SelectString("NO PROMOTIONS", UKismetMathLibrary::SelectString("1 PROMOTION", UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Conv_IntToString(HeaderOf_Promos), " PROMOTIONS"), UKismetMathLibrary::EqualEqual_IntInt(HeaderOf_Promos, 1)), UKismetMathLibrary::LessEqual_IntInt(HeaderOf_Promos, 0)));
            HeaderOf_Hero = HeaderOf_Class->GetCharacterClass();
            CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(HeaderOf_Hero);
            if (CallFunc_IsValidClass_ReturnValue) {
                HeaderOf_Color = UActorFunctionLibrary::GetHeroColor(HeaderOf_Hero);
                HeaderOf_Icon = UActorFunctionLibrary::GetHeroImageSmall(HeaderOf_Hero);
            }
        } else {
            HeaderOf_Sub = "SELECTING CLASS";
        }
    }
    Name = HeaderOf_Name;
    Sub = HeaderOf_Sub;
    Color = HeaderOf_Color;
    Icon = HeaderOf_Icon;
    IsYou = HeaderOf_IsYou;
    IsHost = HeaderOf_IsHost;
    return;
}

void DB_Data::HeaderOfCode(FString Code, FString& Sub, FLinearColor& Color, UTexture2D*& Icon) {
    FString CallFunc_ClassLetter_Letter;
    FString CallFunc_ClassLetter_Name;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_ParseCode_Valid;
    HeaderOfCode_Sub = "";
    HeaderOfCode_Color = FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f);
    HeaderOfCode_Icon = nullptr;
    ParseCode(Code, CallFunc_ParseCode_Valid);
    if (CallFunc_ParseCode_Valid) {
        ClassLetter(ParseCode_Class, CallFunc_ClassLetter_Letter, CallFunc_ClassLetter_Name);
        HeaderOfCode_Sub = UKismetStringLibrary::Concat_StrStr(CallFunc_ClassLetter_Name, " BUILD");
        HeaderOfCode_Hero = ParseCode_Class->GetCharacterClass();
        CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(HeaderOfCode_Hero);
        if (CallFunc_IsValidClass_ReturnValue) {
            HeaderOfCode_Color = UActorFunctionLibrary::GetHeroColor(HeaderOfCode_Hero);
            HeaderOfCode_Icon = UActorFunctionLibrary::GetHeroImageSmall(HeaderOfCode_Hero);
        }
    }
    Sub = HeaderOfCode_Sub;
    Color = HeaderOfCode_Color;
    Icon = HeaderOfCode_Icon;
    return;
}

void DB_Data::PerksOf(AFSDPlayerState* Player, TArray<UPerkAsset*>& Perks, TArray<int32>& Ranks, int32& State) {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Add_ReturnValue_2;
    int32 CallFunc_Array_Add_ReturnValue_3;
    FSaveGameStatePerkItem CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    bool CallFunc_Find_Found;
    TArray<UPerkAsset*> CallFunc_Find_Perks;
    TArray<int32> CallFunc_Find_Ranks;
    int32 CallFunc_GetCurrentRank_ReturnValue;
    bool CallFunc_IsServer_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_IsValid_ReturnValue_3;
    bool CallFunc_IsValid_ReturnValue_4;
    bool CallFunc_IsValid_ReturnValue_5;
    bool CallFunc_IsValid_ReturnValue_6;
    bool CallFunc_Less_FloatFloat_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    AFSDPlayerState* CallFunc_LocalPS_PS;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    PerksOf_Perks.Clear();
    PerksOf_Ranks.Clear();
    PerksOf_State = 0;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Player);
    if (CallFunc_IsValid_ReturnValue_1) {
        PerksOf_Class = Player->GetSelectedCharacterID();
        CallFunc_IsValid_ReturnValue_6 = UKismetSystemLibrary::IsValid(PerksOf_Class);
        if (CallFunc_IsValid_ReturnValue_6) {
            LocalPS(CallFunc_LocalPS_PS);
            CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(Player, CallFunc_LocalPS_PS);
            if (CallFunc_EqualEqual_ObjectObject_ReturnValue) {
                PerksOf_Own = UPerkFunctionLibrary::GetCharacterEquippedPerks(this, PerksOf_Class);
                Temp_int_Loop_Counter_Variable = 0;
                Temp_int_Array_Index_Variable = 0;
                while (true) {
                    CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerksOf_Own.Num());
                    if (CallFunc_Less_IntInt_ReturnValue_1) {
                        Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                        PerksOf_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                        CallFunc_IsValid_ReturnValue_5 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_1);
                        if (CallFunc_IsValid_ReturnValue_5) {
                            PerksOf_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                            CallFunc_Array_Add_ReturnValue_3 = PerksOf_Perks.Add(CallFunc_Array_Get_Item_1);
                            PerksOf_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                            CallFunc_GetCurrentRank_ReturnValue = CallFunc_Array_Get_Item_1->GetCurrentRank(this);
                            CallFunc_Array_Add_ReturnValue_2 = PerksOf_Ranks.Add(CallFunc_GetCurrentRank_ReturnValue);
                        }
                        Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                    } else {
                        break;
                    }
                }
            } else {
                // Another player: their link when there is one (DB_BuildShare_v2), else what the host sees or the
                // host's 0.1.0 perk table says.
                PerksOf_Link = LinkFor(Player);
                if (PerksOf_Link != nullptr) {
                    if (PerksOf_Link->PerksKnown) {
                        bool LinkFound;
                        PerksOfCode(PerksOf_Link->Code, PerksOf_Perks, PerksOf_Ranks, LinkFound);
                    } else {
                        PerksOf_State = 3;
                    }
                    goto L_03DE;
                }
                CallFunc_IsServer_ReturnValue = UKismetSystemLibrary::IsServer(this);
                if (CallFunc_IsServer_ReturnValue) {
                    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Player->SaveGameStateComponent);
                    if (CallFunc_IsValid_ReturnValue) {
                        PerksOf_Items = Player->SaveGameStateComponent->EquippedPerks;
                        Temp_int_Loop_Counter_Variable_1 = 0;
                        Temp_int_Array_Index_Variable_1 = 0;
                        while (true) {
                            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, PerksOf_Items.Num());
                            if (CallFunc_Less_IntInt_ReturnValue) {
                                Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable_1;
                                PerksOf_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                CallFunc_IsValid_ReturnValue_4 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item.Perk);
                                if (CallFunc_IsValid_ReturnValue_4) {
                                    PerksOf_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                    CallFunc_Array_Add_ReturnValue = PerksOf_Perks.Add(CallFunc_Array_Get_Item.Perk);
                                    PerksOf_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                    CallFunc_Array_Add_ReturnValue_1 = PerksOf_Ranks.Add(UKismetMathLibrary::Conv_ByteToInt(CallFunc_Array_Get_Item.Rank));
                                }
                                Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
                            } else {
                                break;
                            }
                        }
                    } else {
                        goto L_03DE;
                    }
                } else {
                    CallFunc_IsValid_ReturnValue_3 = UKismetSystemLibrary::IsValid(PerksOf_Share);
                    if (!CallFunc_IsValid_ReturnValue_3) {
                        PerksOf_Share = (DB_PerkShare_v1*)UGameplayStatics::GetActorOfClass(this, DB_PerkShare_v1::StaticClass());
                        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(PerksOf_Share);
                        if (!CallFunc_IsValid_ReturnValue_2) {
                            CallFunc_Less_FloatFloat_ReturnValue = UKismetMathLibrary::Less_FloatFloat(GetGameTimeSinceCreation(), 10.0f);
                            if (CallFunc_Less_FloatFloat_ReturnValue) {
                                PerksOf_State = 2;
                                goto L_03DE;
                            } else {
                                PerksOf_State = 1;
                                goto L_03DE;
                            }
                        }
                    }
                    PerksOf_Share->Find(Player->playerID, PerksOf_Class, CallFunc_Find_Perks, CallFunc_Find_Ranks, CallFunc_Find_Found);
                    if (CallFunc_Find_Found) {
                        PerksOf_Perks = CallFunc_Find_Perks;
                        PerksOf_Ranks = CallFunc_Find_Ranks;
                    } else {
                        PerksOf_State = 2;
                        goto L_03DE;
                    }
                }
            }
            // The game sends a player's perks to the host only at times, and to clients never: none known for
            // another player means unknown, not "no perks".
            // But a player whose link is young may still send their build (their mod does it when the link reaches
            // them): "not received yet" for 6 s, so "not available" does not flash.
            if (!CallFunc_EqualEqual_ObjectObject_ReturnValue && PerksOf_Perks.Num() == 0) {
                PerksOf_State = 3;
                for (DB_BuildShare_v2* Young : Links)
                    if (UKismetSystemLibrary::IsValid(Young) && Young->Player == Player &&
                        Young->GetGameTimeSinceCreation() < 6.0f)
                        PerksOf_State = 2;
                goto L_03DE;
            }
            SortPerks();
        }
    }
  L_03DE:
    Perks = PerksOf_Perks;
    Ranks = PerksOf_Ranks;
    State = PerksOf_State;
    return;
}

void DB_Data::ClassOfCode(FString Code, UPlayerCharacterID*& Class, bool& Valid) {
    bool CallFunc_ParseCode_Valid;
    ParseCode(Code, CallFunc_ParseCode_Valid);
    Class = ParseCode_Class;
    Valid = CallFunc_ParseCode_Valid;
    return;
}

// A tier digit above the tier's options, and an overclock number above the item's overclocks, come from a game version
// that has more of them: the choice is -1 (not 0) and UnknownOc is true (not "no overclock"), so ApplyPart keeps what
// the slot has instead of clearing it. Both show as nothing chosen.
void DB_Data::DecodeSlot(FString Code, int32 Slot, UItemID*& Item, TArray<int32>& Choices, UOverclockUpgrade*& Overclock, FString& Digits, bool& Found, bool& UnknownOc) {
    int32 CallFunc_Array_Add_ReturnValue;
    UOverclockUpgrade* CallFunc_Array_Get_Item;
    FString CallFunc_Array_Get_Item_1;
    FUpgradeTier CallFunc_Array_Get_Item_2;
    FString CallFunc_Array_Get_Item_3;
    UItemID* CallFunc_Array_Get_Item_4;
    FString CallFunc_Array_Get_Item_5;
    FString CallFunc_Array_Get_Item_6;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue_3;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue_2;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_2;
    int32 CallFunc_Max_ReturnValue;
    TArray<UOverclockUpgrade*> CallFunc_OverclocksOf_Overclocks;
    bool CallFunc_ParseCode_Valid;
    TArray<FGuid> CallFunc_SlotItems_Ids;
    TArray<UItemID*> CallFunc_SlotItems_Items;
    int32 Temp_int_Variable;
    DecodeSlot_Item = nullptr;
    DecodeSlot_Overclock = nullptr;
    DecodeSlot_Digits = "";
    DecodeSlot_Found = false;
    bool OcBeyond = false;
    DecodeSlot_Choices.Clear();
    ParseCode(Code, CallFunc_ParseCode_Valid);
    if (CallFunc_ParseCode_Valid) {
        CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(Slot, 0) && UKismetMathLibrary::LessEqual_IntInt(Slot, 6);
        if (CallFunc_BooleanAND_ReturnValue) {
            ParseCode_Parts.Get(UKismetMathLibrary::Add_IntInt(Slot, 2), CallFunc_Array_Get_Item_6);
            DecodeSlot_Sub = UKismetStringLibrary::ParseIntoArray(CallFunc_Array_Get_Item_6, "-", false);
            CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(DecodeSlot_Sub.Num(), 3);
            DecodeSlot_Sub.Resize(CallFunc_Max_ReturnValue);
            SlotItems(ParseCode_Class, Slot, CallFunc_SlotItems_Items, CallFunc_SlotItems_Ids);
            DecodeSlot_Items = CallFunc_SlotItems_Items;
            DecodeSlot_Sub.Get(0, CallFunc_Array_Get_Item_5);
            DecodeSlot_Index = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_5);
            CallFunc_BooleanAND_ReturnValue_3 = UKismetMathLibrary::BooleanAND(UKismetMathLibrary::GreaterEqual_IntInt(DecodeSlot_Index, 1), UKismetMathLibrary::LessEqual_IntInt(DecodeSlot_Index, DecodeSlot_Items.Num()));
            if (CallFunc_BooleanAND_ReturnValue_3) {
                DecodeSlot_Items.Get(UKismetMathLibrary::Subtract_IntInt(DecodeSlot_Index, 1), CallFunc_Array_Get_Item_4);
                DecodeSlot_Item = CallFunc_Array_Get_Item_4;
                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(DecodeSlot_Item);
                if (CallFunc_IsValid_ReturnValue) {
                    DecodeSlot_Found = true;
                    DecodeSlot_Class = DecodeSlot_Item->GetItemClass();
                    CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(DecodeSlot_Class);
                    if (CallFunc_IsValidClass_ReturnValue) {
                        DecodeSlot_Tiers = UUpgradableGearComponent::GetItemUpgradeTiers((UClass*)DecodeSlot_Class);
                        DecodeSlot_Sub.Get(1, CallFunc_Array_Get_Item_3);
                        DecodeSlot_TierText = CallFunc_Array_Get_Item_3;
                        Temp_int_Variable = 0;
                        while (true) {
                            CallFunc_LessEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, UKismetMathLibrary::Subtract_IntInt(DecodeSlot_Tiers.Num(), 1));
                            if (CallFunc_LessEqual_IntInt_ReturnValue_2) {
                                DecodeSlot_Digit = UKismetStringLibrary::Conv_StringToInt(UKismetStringLibrary::GetSubstring(DecodeSlot_TierText, Temp_int_Variable, 1));
                                CallFunc_GreaterEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::GreaterEqual_IntInt(DecodeSlot_Digit, 1);
                                DecodeSlot_Tiers.Get(Temp_int_Variable, CallFunc_Array_Get_Item_2);
                                DecodeSlot_Choice = UKismetMathLibrary::SelectInt(DecodeSlot_Digit, DecodeSlot_Digit > CallFunc_Array_Get_Item_2.Upgrades.Num() ? -1 : 0, UKismetMathLibrary::BooleanAND(CallFunc_GreaterEqual_IntInt_ReturnValue_2, UKismetMathLibrary::LessEqual_IntInt(DecodeSlot_Digit, CallFunc_Array_Get_Item_2.Upgrades.Num())));
                                CallFunc_Array_Add_ReturnValue = DecodeSlot_Choices.Add(DecodeSlot_Choice);
                                DecodeSlot_Digits = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(DecodeSlot_Digits, UKismetMathLibrary::SelectString(" ", "", UKismetMathLibrary::Greater_IntInt(Temp_int_Variable, 0))), UKismetMathLibrary::SelectString("?", UKismetStringLibrary::Conv_IntToString(DecodeSlot_Choice), DecodeSlot_Choice < 0));
                                Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                            } else {
                                break;
                            }
                        }
                    }
                    DecodeSlot_Sub.Get(2, CallFunc_Array_Get_Item_1);
                    DecodeSlot_OcIndex = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_1);
                    OverclocksOf(DecodeSlot_Item, CallFunc_OverclocksOf_Overclocks);
                    DecodeSlot_OCs = CallFunc_OverclocksOf_Overclocks;
                    CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::BooleanAND(UKismetMathLibrary::GreaterEqual_IntInt(DecodeSlot_OcIndex, 1), UKismetMathLibrary::LessEqual_IntInt(DecodeSlot_OcIndex, DecodeSlot_OCs.Num()));
                    if (CallFunc_BooleanAND_ReturnValue_1) {
                        DecodeSlot_OCs.Get(UKismetMathLibrary::Subtract_IntInt(DecodeSlot_OcIndex, 1), CallFunc_Array_Get_Item);
                        DecodeSlot_Overclock = CallFunc_Array_Get_Item;
                    } else {
                        OcBeyond = DecodeSlot_OcIndex > DecodeSlot_OCs.Num();
                    }
                }
            }
        }
    }
    Item = DecodeSlot_Item;
    Choices = DecodeSlot_Choices;
    Overclock = DecodeSlot_Overclock;
    Digits = DecodeSlot_Digits;
    Found = DecodeSlot_Found;
    UnknownOc = OcBeyond;
    return;
}

void DB_Data::PerksOfCode(FString Code, TArray<UPerkAsset*>& Perks, TArray<int32>& Ranks, bool& Found) {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    FString CallFunc_Array_Get_Item_2;
    bool CallFunc_Greater_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_ParseCode_Valid;
    TArray<int32> CallFunc_PerkIds_Ids;
    int32 CallFunc_SelectInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    PerksOfCode_Perks.Clear();
    PerksOfCode_Ranks.Clear();
    PerksOfCode_Unknown = 0;
    ParseCode(Code, CallFunc_ParseCode_Valid);
    if (CallFunc_ParseCode_Valid) {
        ParseCode_Parts.Get(9, CallFunc_Array_Get_Item_2);
        PerkIds(CallFunc_Array_Get_Item_2, CallFunc_PerkIds_Ids);
        PerksOfCode_Ids = CallFunc_PerkIds_Ids;
        PerksOfCode_Unknown = PerkIds_Bad;
        EnsurePerkTable();
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerksOfCode_Ids.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                PerksOfCode_Ids.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                PerkTable.Get(UKismetMathLibrary::Subtract_IntInt(CallFunc_Array_Get_Item, 1), CallFunc_Array_Get_Item_1);
                PerksOfCode_Perk = CallFunc_Array_Get_Item_1;
                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(PerksOfCode_Perk);
                if (CallFunc_IsValid_ReturnValue) {
                    CallFunc_Array_Add_ReturnValue_1 = PerksOfCode_Perks.Add(PerksOfCode_Perk);
                    PerksOfCode_Rank = PerksOfCode_Perk->GetCurrentRank(this);
                    CallFunc_SelectInt_ReturnValue = UKismetMathLibrary::SelectInt(PerksOfCode_Rank, PerksOfCode_Perk->GetMaxRank(), UKismetMathLibrary::Greater_IntInt(PerksOfCode_Rank, 0));
                    CallFunc_Array_Add_ReturnValue = PerksOfCode_Ranks.Add(CallFunc_SelectInt_ReturnValue);
                } else {
                    PerksOfCode_Unknown = UKismetMathLibrary::Add_IntInt(PerksOfCode_Unknown, 1);
                }
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                break;
            }
        }
    }
    CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(PerksOfCode_Perks.Num(), 0);
    Perks = PerksOfCode_Perks;
    Ranks = PerksOfCode_Ranks;
    Found = CallFunc_Greater_IntInt_ReturnValue;
    return;
}

void DB_Data::TierOptions(UItemID* Item, int32 Tier, TArray<UItemUpgrade*>& Upgrades) {
    FUpgradeTier CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    TierOptions_Upgrades.Clear();
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Item);
    if (CallFunc_IsValid_ReturnValue) {
        TierOptions_Class = Item->GetItemClass();
        CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(TierOptions_Class);
        if (CallFunc_IsValidClass_ReturnValue) {
            TierOptions_Tiers = UUpgradableGearComponent::GetItemUpgradeTiers((UClass*)TierOptions_Class);
            CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetMathLibrary::GreaterEqual_IntInt(Tier, 0), UKismetMathLibrary::LessEqual_IntInt(Tier, UKismetMathLibrary::Subtract_IntInt(TierOptions_Tiers.Num(), 1)));
            if (CallFunc_BooleanAND_ReturnValue) {
                TierOptions_Tiers.Get(Tier, CallFunc_Array_Get_Item);
                TierOptions_Upgrades = CallFunc_Array_Get_Item.Upgrades;
            }
        }
    }
    Upgrades = TierOptions_Upgrades;
    return;
}

void DB_Data::ItemVisual(UItemID* Item, FText& Name, FText& Category, UTexture2D*& Line, UTexture2D*& Fill) {
    FText CallFunc_ItemTexts_Category;
    FText CallFunc_ItemTexts_Description;
    UTexture2D* CallFunc_ItemTexts_Fill;
    UTexture2D* CallFunc_ItemTexts_Line;
    FText CallFunc_ItemTexts_Name;
    ItemTexts(Item, CallFunc_ItemTexts_Name, CallFunc_ItemTexts_Category, CallFunc_ItemTexts_Description, CallFunc_ItemTexts_Line, CallFunc_ItemTexts_Fill);
    Name = CallFunc_ItemTexts_Name;
    Category = CallFunc_ItemTexts_Category;
    Line = CallFunc_ItemTexts_Line;
    Fill = CallFunc_ItemTexts_Fill;
    return;
}

void DB_Data::ItemTip(UItemID* Item, FText& Title, FText& Category, FText& Body, UTexture2D*& Icon) {
    FText CallFunc_ItemTexts_Category;
    FText CallFunc_ItemTexts_Description;
    UTexture2D* CallFunc_ItemTexts_Fill;
    UTexture2D* CallFunc_ItemTexts_Line;
    FText CallFunc_ItemTexts_Name;
    ItemTexts(Item, CallFunc_ItemTexts_Name, CallFunc_ItemTexts_Category, CallFunc_ItemTexts_Description, CallFunc_ItemTexts_Line, CallFunc_ItemTexts_Fill);
    Title = CallFunc_ItemTexts_Name;
    Category = CallFunc_ItemTexts_Category;
    Body = CallFunc_ItemTexts_Description;
    Icon = CallFunc_ItemTexts_Line;
    return;
}

void DB_Data::UpgradeTip(UItemUpgrade* Upgrade, FText& Title, FText& Category, FText& Body, UTexture2D*& Icon, FText& ExtraTitle, FText& Extra) {
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    FText CallFunc_StatBody_Body;
    bool CallFunc_TextIsEmpty_ReturnValue;
    UpgradeTip_Title = FText();
    UpgradeTip_Category = FText();
    UpgradeTip_Body = FText();
    UpgradeTip_Icon = nullptr;
    UpgradeTip_ExtraTitle = FText();
    UpgradeTip_Extra = FText();
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Upgrade);
    if (CallFunc_IsValid_ReturnValue) {
        UpgradeTip_Title = Upgrade->Name_0;
        StatBody(Upgrade, CallFunc_StatBody_Body);
        UpgradeTip_Body = CallFunc_StatBody_Body;
        UpgradeTip_Cat = Upgrade->Category;
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(UpgradeTip_Cat);
        if (CallFunc_IsValid_ReturnValue_1) {
            UpgradeTip_Category = UKismetTextLibrary::TextToUpper(UpgradeTip_Cat->CategoryName);
            UpgradeTip_Icon = UpgradeTip_Cat->CategoryIcon;
            UpgradeTip_Extra = UpgradeTip_Cat->CategoryDescription;
            CallFunc_TextIsEmpty_ReturnValue = UKismetTextLibrary::TextIsEmpty(UpgradeTip_Extra);
            if (!CallFunc_TextIsEmpty_ReturnValue) {
                UpgradeTip_ExtraTitle = UpgradeTip_Category;
            }
        }
    }
    Title = UpgradeTip_Title;
    Category = UpgradeTip_Category;
    Body = UpgradeTip_Body;
    Icon = UpgradeTip_Icon;
    ExtraTitle = UpgradeTip_ExtraTitle;
    Extra = UpgradeTip_Extra;
    return;
}

void DB_Data::OverclockTip(UOverclockUpgrade* Overclock, FText& Title, FText& Category, FText& Body, UTexture2D*& Icon, FLinearColor& Color, UTexture2D*& Frame) {
    bool CallFunc_Contains_ReturnValue;
    bool CallFunc_Contains_ReturnValue_1;
    bool CallFunc_Contains_ReturnValue_2;
    FText CallFunc_Conv_StringToText_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_IsValid_ReturnValue_3;
    bool CallFunc_IsValid_ReturnValue_4;
    bool CallFunc_IsValid_ReturnValue_5;
    FText CallFunc_StatBody_Body;
    UTexture2D* K2Node_DynamicCast_AsTexture_2D;
    bool K2Node_DynamicCast_bSuccess;
    OverclockTip_Title = FText();
    OverclockTip_Body = FText();
    OverclockTip_CatText = "";
    OverclockTip_CatName = "";
    OverclockTip_TypeFrame = nullptr;
    OverclockTip_Icon = nullptr;
    OverclockTip_Frame = nullptr;
    OverclockTip_Scat = nullptr;
    OverclockTip_Color = FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Overclock);
    if (CallFunc_IsValid_ReturnValue) {
        OverclockTip_Title = Overclock->Name_0;
        StatBody(Overclock, CallFunc_StatBody_Body);
        OverclockTip_Body = CallFunc_StatBody_Body;
        OverclockTip_Cat = Overclock->Category;
        CallFunc_IsValid_ReturnValue_5 = UKismetSystemLibrary::IsValid(OverclockTip_Cat);
        if (CallFunc_IsValid_ReturnValue_5) {
            OverclockTip_Icon = OverclockTip_Cat->CategoryIcon;
        }
        OverclockTip_Scat = USchematicBlueprintLibrary::FindItemUpgradeSchematicCategory(Overclock);
        CallFunc_IsValid_ReturnValue_4 = UKismetSystemLibrary::IsValid(OverclockTip_Scat);
        if (!CallFunc_IsValid_ReturnValue_4) {
            OverclockTip_Scat = Overclock->SchematicCategory;
            CallFunc_IsValid_ReturnValue_3 = UKismetSystemLibrary::IsValid(OverclockTip_Scat);
            if (!CallFunc_IsValid_ReturnValue_3) {
                OverclockTip_Key = "";
                goto L_030B;
            }
        }
        OverclockTip_CatName = UKismetTextLibrary::Conv_TextToString(OverclockTip_Scat->CategoryName);
        OverclockTip_Key = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetSystemLibrary::GetObjectName(OverclockTip_Scat), " "), OverclockTip_CatName);
      L_030B:
        CallFunc_Contains_ReturnValue_2 = UKismetStringLibrary::Contains(OverclockTip_Key, "Clean", false, false);
        if (CallFunc_Contains_ReturnValue_2) {
            OverclockTip_CatText = "CLEAN OVERCLOCK";
            OverclockTip_Color = FLinearColor(0.03819f, 0.33333f, 0.10949f, 1.0f);
            OverclockTip_TypeFrame = OcFrameClean;
        } else {
            CallFunc_Contains_ReturnValue_1 = UKismetStringLibrary::Contains(OverclockTip_Key, "Balanced", false, false);
            if (CallFunc_Contains_ReturnValue_1) {
                OverclockTip_CatText = "BALANCED OVERCLOCK";
                OverclockTip_Color = FLinearColor(0.8125f, 0.58836f, 0.03809f, 1.0f);
                OverclockTip_TypeFrame = OcFrameBalanced;
            } else {
                CallFunc_Contains_ReturnValue = UKismetStringLibrary::Contains(OverclockTip_Key, "Unstable", false, false);
                if (CallFunc_Contains_ReturnValue) {
                    OverclockTip_CatText = "UNSTABLE OVERCLOCK";
                    OverclockTip_Color = FLinearColor(0.65104f, 0.03653f, 0.0f, 1.0f);
                    OverclockTip_TypeFrame = OcFrameUnstable;
                } else {
                    OverclockTip_CatText = UKismetMathLibrary::SelectString(UKismetStringLibrary::ToUpper(OverclockTip_CatName), "OVERCLOCK", UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Len(OverclockTip_CatName), 0));
                }
            }
        }
        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(OverclockTip_Scat);
        if (CallFunc_IsValid_ReturnValue_2) {
            K2Node_DynamicCast_AsTexture_2D = Cast<UTexture2D>(UKismetSystemLibrary::Conv_SoftObjectReferenceToObject(OverclockTip_Scat->FrameTexture));
            K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsTexture_2D != nullptr;
            if (K2Node_DynamicCast_bSuccess) {
                OverclockTip_Frame = K2Node_DynamicCast_AsTexture_2D;
            }
        }
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(OverclockTip_Frame);
        if (!CallFunc_IsValid_ReturnValue_1) {
            OverclockTip_Frame = OverclockTip_TypeFrame;
        }
    }
    CallFunc_Conv_StringToText_ReturnValue = UKismetTextLibrary::Conv_StringToText(OverclockTip_CatText);
    Title = OverclockTip_Title;
    Category = CallFunc_Conv_StringToText_ReturnValue;
    Body = OverclockTip_Body;
    Icon = OverclockTip_Icon;
    Color = OverclockTip_Color;
    Frame = OverclockTip_Frame;
    return;
}

void DB_Data::PerkTip(UPerkAsset* Perk, int32 Rank, FText& Title, FText& Category, FText& Body, UTexture2D*& Icon, FLinearColor& Color) {
    TSoftClassPtr<UClass> CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue;
    FString CallFunc_Conv_TextToString_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    FString CallFunc_GetAdditionalRankDescription_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    UClass* CallFunc_LoadClassAsset_Blocking_ReturnValue;
    FSoftClassPath CallFunc_MakeSoftClassPath_ReturnValue;
    int32 CallFunc_Max_ReturnValue;
    int32 CallFunc_PerkKind_Kind;
    FLinearColor CallFunc_SelectColor_ReturnValue;
    FString CallFunc_SelectString_ReturnValue;
    FString CallFunc_StripTags_Out;
    FString CallFunc_StripTags_Out_1;
    PerkTip_Title = FText();
    PerkTip_Category = FText();
    PerkTip_Body = FText();
    PerkTip_Icon = nullptr;
    PerkTip_Color = FLinearColor(0.484375f, 0.027751f, 0.07654f, 1.0f);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Perk);
    if (CallFunc_IsValid_ReturnValue) {
        PerkTip_Title = Perk->Title;
        PerkTip_Icon = Perk->Icon;
        PerkKind(Perk, CallFunc_PerkKind_Kind);
        CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 1);
        CallFunc_SelectString_ReturnValue = UKismetMathLibrary::SelectString("ACTIVE PERK", "PROFICIENCY", CallFunc_EqualEqual_IntInt_ReturnValue);
        CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 0);
        PerkTip_Category = UKismetTextLibrary::Conv_StringToText(UKismetMathLibrary::SelectString("PASSIVE PERK", CallFunc_SelectString_ReturnValue, CallFunc_EqualEqual_IntInt_ReturnValue_1));
        CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 1);
        CallFunc_SelectColor_ReturnValue = UKismetMathLibrary::SelectColor(FLinearColor(0.07053f, 0.520833f, 0.375543f, 1.0f), FLinearColor(1.0f, 0.332452f, 0.0f, 1.0f), CallFunc_EqualEqual_IntInt_ReturnValue);
        CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 0);
        PerkTip_Color = UKismetMathLibrary::SelectColor(FLinearColor(0.484375f, 0.027751f, 0.07654f, 1.0f), CallFunc_SelectColor_ReturnValue, CallFunc_EqualEqual_IntInt_ReturnValue_1);
        CallFunc_MakeSoftClassPath_ReturnValue = UKismetSystemLibrary::MakeSoftClassPath("/Script/FSD.ProficiencyPerkAsset");
        CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue = UKismetSystemLibrary::Conv_SoftClassPathToSoftClassRef(CallFunc_MakeSoftClassPath_ReturnValue);
        CallFunc_LoadClassAsset_Blocking_ReturnValue = UKismetSystemLibrary::LoadClassAsset_Blocking(CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue);
        CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(CallFunc_LoadClassAsset_Blocking_ReturnValue);
        if (CallFunc_IsValidClass_ReturnValue) {
            CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(Rank, 1);
            CallFunc_Conv_TextToString_ReturnValue = UKismetTextLibrary::Conv_TextToString(Perk->GetRankDescription(CallFunc_Max_ReturnValue, false));
            StripTags(CallFunc_Conv_TextToString_ReturnValue, CallFunc_StripTags_Out_1);
            PerkTip_Body = UKismetTextLibrary::Conv_StringToText(CallFunc_StripTags_Out_1);
        } else {
            CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(Rank, 1);
            CallFunc_GetAdditionalRankDescription_ReturnValue = Perk->GetAdditionalRankDescription(CallFunc_Max_ReturnValue);
            StripTags(CallFunc_GetAdditionalRankDescription_ReturnValue, CallFunc_StripTags_Out);
            PerkTip_Body = UKismetTextLibrary::Conv_StringToText(CallFunc_StripTags_Out);
        }
    }
    Title = PerkTip_Title;
    Category = PerkTip_Category;
    Body = PerkTip_Body;
    Icon = PerkTip_Icon;
    Color = PerkTip_Color;
    return;
}

void DB_Data::MoreInfo(bool& On) {
    bool CallFunc_IsValid_ReturnValue;
    MoreInfo_On = false;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Settings);
    if (CallFunc_IsValid_ReturnValue) {
        MoreInfo_On = Settings->MoreInfo;
    }
    On = MoreInfo_On;
    return;
}

void DB_Data::ToggleMoreInfo() {
    TSoftObjectPtr<UObject> CallFunc_Array_Get_Item;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue;
    DB_Tip* CallFunc_LiveTip_Tip;
    bool CallFunc_SaveGameToSlot_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(Settings);
    if (!CallFunc_IsValid_ReturnValue_2) {
        LoadSettings();
    }
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Settings);
    if (CallFunc_IsValid_ReturnValue_1) {
        ToggleMoreInfo_On = !Settings->MoreInfo;
        Settings->MoreInfo = ToggleMoreInfo_On;
        CallFunc_SaveGameToSlot_ReturnValue = UGameplayStatics::SaveGameToSlot(Settings, "DwarfBuilds", 0);
        ToggleMoreInfo_Refs = TipRefs;
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ToggleMoreInfo_Refs.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                ToggleMoreInfo_Refs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                LiveTip(CallFunc_Array_Get_Item, CallFunc_LiveTip_Tip);
                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_LiveTip_Tip);
                if (CallFunc_IsValid_ReturnValue) {
                    CallFunc_LiveTip_Tip->ShowExtra(ToggleMoreInfo_On);
                }
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                return;
            }
        }
    } else {
        return;
    }
}

void DB_Data::RegisterTip(DB_Tip* Tip) {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    TSoftObjectPtr<UObject> CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    TSoftObjectPtr<UObject> CallFunc_Conv_ObjectToSoftObjectReference_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    DB_Tip* CallFunc_LiveTip_Tip;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Tip);
    if (CallFunc_IsValid_ReturnValue) {
        RegisterTip_Keep.Clear();
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, TipRefs.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                TipRefs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                LiveTip(CallFunc_Array_Get_Item, CallFunc_LiveTip_Tip);
                CallFunc_BooleanAND_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_LiveTip_Tip) && !UKismetMathLibrary::EqualEqual_ObjectObject(CallFunc_LiveTip_Tip, Tip);
                if (CallFunc_BooleanAND_ReturnValue) {
                    TipRefs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    CallFunc_Array_Add_ReturnValue_1 = RegisterTip_Keep.Add(CallFunc_Array_Get_Item);
                }
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                CallFunc_Conv_ObjectToSoftObjectReference_ReturnValue = UKismetSystemLibrary::Conv_ObjectToSoftObjectReference(Tip);
                CallFunc_Array_Add_ReturnValue = RegisterTip_Keep.Add(CallFunc_Conv_ObjectToSoftObjectReference_ReturnValue);
                TipRefs = RegisterTip_Keep;
                return;
            }
        }
    } else {
        return;
    }
}

void DB_Data::HideTips() {
    TSoftObjectPtr<UObject> CallFunc_Array_Get_Item;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    DB_Tip* CallFunc_LiveTip_Tip;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, TipRefs.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            TipRefs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
            LiveTip(CallFunc_Array_Get_Item, CallFunc_LiveTip_Tip);
            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_LiveTip_Tip);
            if (CallFunc_IsValid_ReturnValue) {
                CallFunc_LiveTip_Tip->SetVisibility(ESlateVisibility::Collapsed);
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            return;
        }
    }
}

void DB_Data::CanTake(bool& Ok, FString& Reason) {
    AFSDPlayerController* K2Node_DynamicCast_AsFSDPlayer_Controller;
    bool K2Node_DynamicCast_bSuccess;
    K2Node_DynamicCast_AsFSDPlayer_Controller = Cast<AFSDPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsFSDPlayer_Controller != nullptr;
    if (K2Node_DynamicCast_bSuccess) {
        if (K2Node_DynamicCast_AsFSDPlayer_Controller->IsOnSpaceRig) {
            CanTake_Ok = true;
            CanTake_Reason = "";
            goto L_00AC;
        }
    }
    CanTake_Ok = false;
    CanTake_Reason = "Builds can only be taken on the Space Rig.";
  L_00AC:
    Ok = CanTake_Ok;
    Reason = CanTake_Reason;
    return;
}

void DB_Data::SlotInfo(UPlayerCharacterID* Class, int32& Count, int32& Current) {
    FCharacterSave CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_EqualEqual_GuidGuid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool Temp_bool_True_if_break_was_hit_Variable;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    SlotInfo_Count = 7;
    SlotInfo_Current = 0;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Class);
    if (CallFunc_IsValid_ReturnValue) {
        SlotInfo_Save = UGameFunctionLibrary::GetFSDSaveGame(this);
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(SlotInfo_Save);
        if (CallFunc_IsValid_ReturnValue_1) {
            SlotInfo_Current = SlotInfo_Save->GetSelectedLoadoutIndex(Class);
            SlotInfo_Id = UActorFunctionLibrary::GetSaveGameIDFromCharacterID(Class);
            SlotInfo_Saves = SlotInfo_Save->CharacterSaves;
            Temp_bool_True_if_break_was_hit_Variable = false;
            Temp_int_Loop_Counter_Variable = 0;
            Temp_int_Array_Index_Variable = 0;
            while (true) {
                CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(!Temp_bool_True_if_break_was_hit_Variable, UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, SlotInfo_Saves.Num()));
                if (CallFunc_BooleanAND_ReturnValue) {
                    Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                    SlotInfo_Saves.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    CallFunc_EqualEqual_GuidGuid_ReturnValue = CallFunc_Array_Get_Item.SavegameID == SlotInfo_Id;
                    if (CallFunc_EqualEqual_GuidGuid_ReturnValue) {
                        SlotInfo_Saves.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                        SlotInfo_Count = UKismetMathLibrary::Max(CallFunc_Array_Get_Item.Loadouts.Num(), 7);
                        Temp_bool_True_if_break_was_hit_Variable = true;
                    }
                    Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                } else {
                    SlotInfo_Count = UKismetMathLibrary::Max(SlotInfo_Count, UKismetMathLibrary::Add_IntInt(SlotInfo_Current, 1));
                    break;
                }
            }
        }
    }
    Count = SlotInfo_Count;
    Current = SlotInfo_Current;
    return;
}

void DB_Data::ApplyCode(FString Code, int32 Slot, bool EquipAfter, int32& Applied, int32& Skipped, FString& Message) {
    FGuid CallFunc_Array_Get_Item;
    FGuid CallFunc_Array_Get_Item_1;
    FGuid CallFunc_Array_Get_Item_2;
    FGuid CallFunc_Array_Get_Item_3;
    FGuid CallFunc_Array_Get_Item_4;
    FGuid CallFunc_Array_Get_Item_5;
    FGuid CallFunc_Array_Get_Item_6;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanOR_ReturnValue;
    bool CallFunc_CanTake_Ok;
    FString CallFunc_CanTake_Reason;
    UPlayerCharacterID* CallFunc_ClassOfCode_Class;
    bool CallFunc_ClassOfCode_Valid;
    TArray<int32> CallFunc_DecodeSlot_Choices;
    FString CallFunc_DecodeSlot_Digits;
    bool CallFunc_DecodeSlot_Found;
    UItemID* CallFunc_DecodeSlot_Item;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    FLoadoutCopy CallFunc_GetCurrentLoadout_ReturnValue;
    bool CallFunc_DecodeSlot_UnknownOc;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    AFSDPlayerState* CallFunc_LocalPS_PS;
    bool CallFunc_NotEqual_IntInt_ReturnValue_1;
    bool CallFunc_PerksOfCode_Found;
    TArray<UPerkAsset*> CallFunc_PerksOfCode_Perks;
    TArray<int32> CallFunc_PerksOfCode_Ranks;
    int32 CallFunc_SlotInfo_Count;
    int32 CallFunc_SlotInfo_Current;
    TArray<FGuid> K2Node_MakeArray_Array;
    int32 Temp_int_Variable;
    int32 Temp_int_Variable_1;
    ApplyCode_Applied = 0;
    ApplyCode_Skipped = 0;
    ApplyCode_Unknown = 0;
    ApplyCode_Message = "";
    ApplyPerks_Skips.Clear();
    ApplyPerks_Unknown = 0;
    CanTake(CallFunc_CanTake_Ok, CallFunc_CanTake_Reason);
    if (CallFunc_CanTake_Ok) {
        ClassOfCode(Code, CallFunc_ClassOfCode_Class, CallFunc_ClassOfCode_Valid);
        if (CallFunc_ClassOfCode_Valid) {
            ApplyCode_Class = CallFunc_ClassOfCode_Class;
            ApplyCode_Save = UGameFunctionLibrary::GetFSDSaveGame(this);
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(ApplyCode_Save);
            if (CallFunc_IsValid_ReturnValue_2) {
                SlotInfo(ApplyCode_Class, CallFunc_SlotInfo_Count, CallFunc_SlotInfo_Current);
                CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::GreaterEqual_IntInt(Slot, 0) && UKismetMathLibrary::Less_IntInt(Slot, CallFunc_SlotInfo_Count);
                if (CallFunc_BooleanAND_ReturnValue_1) {
                    Temp_int_Variable = 0;
                    while (true) {
                        CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 6);
                        if (CallFunc_LessEqual_IntInt_ReturnValue) {
                            DecodeSlot(Code, Temp_int_Variable, CallFunc_DecodeSlot_Item, CallFunc_DecodeSlot_Choices, CallFunc_DecodeSlot_Overclock, CallFunc_DecodeSlot_Digits, CallFunc_DecodeSlot_Found, CallFunc_DecodeSlot_UnknownOc);
                            if (!CallFunc_DecodeSlot_Found) {
                                ApplyCode_Unknown = UKismetMathLibrary::Add_IntInt(ApplyCode_Unknown, 1);
                            }
                            Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                        } else {
                            PerksOfCode(Code, CallFunc_PerksOfCode_Perks, CallFunc_PerksOfCode_Ranks, CallFunc_PerksOfCode_Found);
                            CallFunc_BooleanOR_ReturnValue = UKismetMathLibrary::Less_IntInt(ApplyCode_Unknown, 7) || CallFunc_PerksOfCode_Found;
                            if (CallFunc_BooleanOR_ReturnValue) {
                                ApplyCode_Save->SetIgnoreRandomLoadout(true);
                                ApplyCode_Old = ApplyCode_Save->GetSelectedLoadoutIndex(ApplyCode_Class);
                                CallFunc_NotEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::NotEqual_IntInt(Slot, ApplyCode_Old);
                                if (CallFunc_NotEqual_IntInt_ReturnValue_1) {
                                    ApplyCode_Save->SetCharacterLoadout(ApplyCode_Class, Slot);
                                }
                                ApplyCode_Loadout = ULoadoutFunctionLibrary::GetCurrentLoadout(this, ApplyCode_Class);
                                ApplyCode_Map = ApplyCode_Loadout.UpgradeLoadout.Loadout;
                                K2Node_MakeArray_Array = {ApplyCode_Loadout.ItemLoadout.PrimaryWeapon, ApplyCode_Loadout.ItemLoadout.SecondaryWeapon, ApplyCode_Loadout.ItemLoadout.TraversalTool, ApplyCode_Loadout.ItemLoadout.ClassTool, ApplyCode_Loadout.ItemLoadout.Armor, ApplyCode_Loadout.ItemLoadout.Grenade, ApplyCode_Loadout.ItemLoadout.MiningTool};
                                ApplyCode_Guids = K2Node_MakeArray_Array;
                                Temp_int_Variable_1 = 0;
                                while (true) {
                                    CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable_1, 6);
                                    if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
                                        ApplyPart(Code, Temp_int_Variable_1);
                                        Temp_int_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Variable_1, 1);
                                    } else {
                                        // The seven code slots and their selections go back into the structs that
                                        // were read: everything else in them (flare, icon, perks, vanity, fields of a
                                        // later game version) keeps its value.
                                        ApplyCode_Guids.Get(6, CallFunc_Array_Get_Item);
                                        ApplyCode_Guids.Get(5, CallFunc_Array_Get_Item_1);
                                        ApplyCode_Guids.Get(4, CallFunc_Array_Get_Item_2);
                                        ApplyCode_Guids.Get(3, CallFunc_Array_Get_Item_3);
                                        ApplyCode_Guids.Get(2, CallFunc_Array_Get_Item_4);
                                        ApplyCode_Guids.Get(1, CallFunc_Array_Get_Item_5);
                                        ApplyCode_Guids.Get(0, CallFunc_Array_Get_Item_6);
                                        ApplyCode_Loadout.ItemLoadout.PrimaryWeapon = CallFunc_Array_Get_Item_6;
                                        ApplyCode_Loadout.ItemLoadout.SecondaryWeapon = CallFunc_Array_Get_Item_5;
                                        ApplyCode_Loadout.ItemLoadout.TraversalTool = CallFunc_Array_Get_Item_4;
                                        ApplyCode_Loadout.ItemLoadout.ClassTool = CallFunc_Array_Get_Item_3;
                                        ApplyCode_Loadout.ItemLoadout.Armor = CallFunc_Array_Get_Item_2;
                                        ApplyCode_Loadout.ItemLoadout.MiningTool = CallFunc_Array_Get_Item;
                                        ApplyCode_Loadout.ItemLoadout.Grenade = CallFunc_Array_Get_Item_1;
                                        ApplyCode_Loadout.UpgradeLoadout.Loadout = ApplyCode_Map;
                                        ULoadoutFunctionLibrary::SetCurrLoadout(this, ApplyCode_Class, ApplyCode_Loadout);
                                        ApplyPerks(Code);
                                        CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::NotEqual_IntInt(Slot, ApplyCode_Old) && !EquipAfter;
                                        if (CallFunc_BooleanAND_ReturnValue) {
                                            CallFunc_GetCurrentLoadout_ReturnValue = ULoadoutFunctionLibrary::GetCurrentLoadout(this, ApplyCode_Class);
                                            ULoadoutFunctionLibrary::SetCurrLoadout(this, ApplyCode_Class, CallFunc_GetCurrentLoadout_ReturnValue);
                                            ApplyCode_Save->SetCharacterLoadout(ApplyCode_Class, ApplyCode_Old);
                                        }
                                        LocalPS(CallFunc_LocalPS_PS);
                                        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_LocalPS_PS);
                                        if (CallFunc_IsValid_ReturnValue_1) {
                                            CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(CallFunc_LocalPS_PS->GetSelectedCharacterID(), ApplyCode_Class);
                                            if (CallFunc_EqualEqual_ObjectObject_ReturnValue) {
                                                ApplyCode_Save->RefreshLoadoutFromCharacter(ApplyCode_Class);
                                                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_LocalPS_PS->SaveGameStateComponent);
                                                if (CallFunc_IsValid_ReturnValue) {
                                                    CallFunc_LocalPS_PS->SaveGameStateComponent->RefreshLoadoutFromSave(ApplyCode_Class);
                                                }
                                            }
                                        }
                                        ApplyCode_Save->SaveToDisk();
                                        ApplyCode_Save->SetIgnoreRandomLoadout(false);
                                        UKismetSystemLibrary::K2_SetTimer(this, "HostSync", 0.3f, false, 0.0f, 0.0f);
                                        ApplyCode_Message = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr("Saved to loadout ", UKismetMathLibrary::SelectString(UKismetStringLibrary::GetSubstring("ABCDEFGHIJKLMNOPQRSTUVWXYZ", Slot, 1), UKismetStringLibrary::Conv_IntToString(UKismetMathLibrary::Add_IntInt(Slot, 1)), UKismetMathLibrary::Less_IntInt(Slot, 26))), "."), UKismetMathLibrary::SelectString("", UKismetMathLibrary::SelectString(" 1 part you do not own was kept as it was.", UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(" ", UKismetStringLibrary::Conv_IntToString(ApplyCode_Skipped)), " parts you do not own were kept as they were."), UKismetMathLibrary::EqualEqual_IntInt(ApplyCode_Skipped, 1)), UKismetMathLibrary::EqualEqual_IntInt(ApplyCode_Skipped, 0))), UKismetMathLibrary::SelectString("", UKismetMathLibrary::SelectString(" 1 part of the code is unknown and was kept as it was.", UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(" ", UKismetStringLibrary::Conv_IntToString(ApplyCode_Unknown)), " parts of the code are unknown and were kept as they were."), UKismetMathLibrary::EqualEqual_IntInt(ApplyCode_Unknown, 1)), UKismetMathLibrary::EqualEqual_IntInt(ApplyCode_Unknown, 0))), UKismetMathLibrary::SelectString("", UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(" Perks not taken: ", UKismetStringLibrary::JoinStringArray(ApplyPerks_Skips, ", ")), "."), UKismetMathLibrary::EqualEqual_IntInt(ApplyPerks_Skips.Num(), 0))), UKismetMathLibrary::SelectString("", UKismetMathLibrary::SelectString(" 1 perk unknown to this game version was left out.", UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(" ", UKismetStringLibrary::Conv_IntToString(ApplyPerks_Unknown)), " perks unknown to this game version were left out."), UKismetMathLibrary::EqualEqual_IntInt(ApplyPerks_Unknown, 1)), UKismetMathLibrary::EqualEqual_IntInt(ApplyPerks_Unknown, 0)));
                                        ApplyCode_Skipped = UKismetMathLibrary::Add_IntInt(ApplyCode_Skipped, ApplyPerks_Skips.Num());
                                        goto L_02D1;
                                    }
                                }
                            } else {
                                ApplyCode_Message = "Nothing to apply: this build is not known yet.";
                                break;
                            }
                        }
                    }
                } else {
                    ApplyCode_Message = "This loadout slot does not exist.";
                }
            } else {
                ApplyCode_Message = "The save game is not available.";
            }
        } else {
            ApplyCode_Message = "This code is not a valid Dwarf Builds code.";
        }
    } else {
        ApplyCode_Message = CallFunc_CanTake_Reason;
    }
  L_02D1:
    Applied = ApplyCode_Applied;
    Skipped = ApplyCode_Skipped;
    Message = ApplyCode_Message;
    return;
}

void DB_Data::LocalPS(AFSDPlayerState*& PS) {
    APlayerController* CallFunc_GetPlayerController_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    AFSDPlayerState* K2Node_DynamicCast_AsFSDPlayer_State;
    bool K2Node_DynamicCast_bSuccess;
    CallFunc_GetPlayerController_ReturnValue = UGameplayStatics::GetPlayerController(this, 0);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_GetPlayerController_ReturnValue);
    if (CallFunc_IsValid_ReturnValue) {
        CallFunc_GetPlayerController_ReturnValue = UGameplayStatics::GetPlayerController(this, 0);
        K2Node_DynamicCast_AsFSDPlayer_State = Cast<AFSDPlayerState>(CallFunc_GetPlayerController_ReturnValue->PlayerState);
        K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsFSDPlayer_State != nullptr;
        if (K2Node_DynamicCast_bSuccess) {
            LocalPS_PS = K2Node_DynamicCast_AsFSDPlayer_State;
            goto L_00D7;
        }
    }
    LocalPS_PS = nullptr;
  L_00D7:
    PS = LocalPS_PS;
    return;
}

void DB_Data::ClassLetter(UPlayerCharacterID* Class, FString& Letter, FString& Name) {
    bool CallFunc_EndsWith_ReturnValue;
    FString CallFunc_GetObjectName_ReturnValue;
    FString CallFunc_GetSubstring_ReturnValue;
    FString CallFunc_ToUpper_ReturnValue;
    FString CallFunc_ToUpper_ReturnValue_1;
    CallFunc_GetObjectName_ReturnValue = UKismetSystemLibrary::GetObjectName(Class);
    CallFunc_GetSubstring_ReturnValue = UKismetStringLibrary::GetSubstring(CallFunc_GetObjectName_ReturnValue, 0, 1);
    CallFunc_EndsWith_ReturnValue = UKismetStringLibrary::EndsWith(CallFunc_GetObjectName_ReturnValue, "ID", ESearchCase::IgnoreCase);
    CallFunc_ToUpper_ReturnValue = UKismetStringLibrary::ToUpper(CallFunc_GetSubstring_ReturnValue);
    CallFunc_ToUpper_ReturnValue_1 = UKismetStringLibrary::ToUpper(UKismetMathLibrary::SelectString(UKismetStringLibrary::LeftChop(CallFunc_GetObjectName_ReturnValue, 2), CallFunc_GetObjectName_ReturnValue, CallFunc_EndsWith_ReturnValue));
    Letter = CallFunc_ToUpper_ReturnValue;
    Name = CallFunc_ToUpper_ReturnValue_1;
    return;
}

void DB_Data::SlotItems(UPlayerCharacterID* Class, int32 Slot, TArray<UItemID*>& Items, TArray<FGuid>& Ids) {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    UItemID* CallFunc_Array_Get_Item;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool K2Node_SwitchInteger_CmpSuccess;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    SlotItems_Items.Clear();
    SlotItems_Ids.Clear();
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Class);
    if (CallFunc_IsValid_ReturnValue) {
        SlotItems_List = UActorFunctionLibrary::GetHeroInventoryList(Class);
        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(SlotItems_List);
        if (CallFunc_IsValid_ReturnValue_2) {
            K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 0);
            if (K2Node_SwitchInteger_CmpSuccess) {
                K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 1);
                if (K2Node_SwitchInteger_CmpSuccess) {
                    K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 2);
                    if (K2Node_SwitchInteger_CmpSuccess) {
                        K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 3);
                        if (K2Node_SwitchInteger_CmpSuccess) {
                            K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 4);
                            if (K2Node_SwitchInteger_CmpSuccess) {
                                K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 5);
                                if (K2Node_SwitchInteger_CmpSuccess) {
                                    K2Node_SwitchInteger_CmpSuccess = UKismetMathLibrary::NotEqual_IntInt(Slot, 6);
                                    if (K2Node_SwitchInteger_CmpSuccess) {
                                        goto L_022D;
                                    } else {
                                        SlotItems_Items = SlotItems_List->MiningTools;
                                    }
                                } else {
                                    SlotItems_Items = SlotItems_List->Grenades;
                                }
                            } else {
                                SlotItems_Items = SlotItems_List->Armors;
                            }
                        } else {
                            SlotItems_Items = SlotItems_List->ClassTools;
                        }
                    } else {
                        SlotItems_Items = SlotItems_List->TraversalTools;
                    }
                } else {
                    SlotItems_Items = SlotItems_List->SecondaryWeapons;
                }
            } else {
                SlotItems_Items = SlotItems_List->PrimaryWeapons;
            }
            Temp_int_Loop_Counter_Variable = 0;
            Temp_int_Array_Index_Variable = 0;
            while (true) {
                CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, SlotItems_Items.Num());
                if (CallFunc_Less_IntInt_ReturnValue) {
                    Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                    SlotItems_Items.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item);
                    if (CallFunc_IsValid_ReturnValue_1) {
                        SlotItems_Items.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                        CallFunc_Array_Add_ReturnValue_1 = SlotItems_Ids.Add(CallFunc_Array_Get_Item->SavegameID);
                    } else {
                        CallFunc_Array_Add_ReturnValue = SlotItems_Ids.Add(ZeroGuid);
                    }
                    Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                } else {
                    break;
                }
            }
        }
    }
  L_022D:
    Items = SlotItems_Items;
    Ids = SlotItems_Ids;
    return;
}

void DB_Data::GearOf(UItemID* Item, UUpgradableGearComponent*& Gear) {
    AItem* CallFunc_GetItem_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    UUpgradableGearComponent* K2Node_DynamicCast_AsUpgradable_Gear_Component;
    bool K2Node_DynamicCast_bSuccess;
    GearOf_Gear = nullptr;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Item);
    if (CallFunc_IsValid_ReturnValue_1) {
        GearOf_Class = Item->GetItemClass();
        CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(GearOf_Class);
        if (CallFunc_IsValidClass_ReturnValue) {
            K2Node_DynamicCast_AsUpgradable_Gear_Component = Cast<UUpgradableGearComponent>(UActorFunctionLibrary::GetComponentFromClass((UClass*)GearOf_Class, UUpgradableGearComponent::StaticClass()));
            K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsUpgradable_Gear_Component != nullptr;
            if (K2Node_DynamicCast_bSuccess) {
                GearOf_Gear = K2Node_DynamicCast_AsUpgradable_Gear_Component;
                goto L_0126;
            }
        }
        CallFunc_GetItem_ReturnValue = Item->GetItem();
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_GetItem_ReturnValue);
        if (CallFunc_IsValid_ReturnValue) {
            CallFunc_GetItem_ReturnValue = Item->GetItem();
            GearOf_Gear = CallFunc_GetItem_ReturnValue->UpgradableItem;
        }
    }
  L_0126:
    Gear = GearOf_Gear;
    return;
}

void DB_Data::OverclocksOf(UItemID* Item, TArray<UOverclockUpgrade*>& Overclocks) {
    UUpgradableGearComponent* CallFunc_GearOf_Gear;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    TArray<UOverclockUpgrade*> CallFunc_Map_Keys_Keys;
    OverclocksOf_List.Clear();
    GearOf(Item, CallFunc_GearOf_Gear);
    OverclocksOf_Gear = CallFunc_GearOf_Gear;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(OverclocksOf_Gear);
    if (CallFunc_IsValid_ReturnValue_1) {
        OverclocksOf_Bank = OverclocksOf_Gear->OverclockBank;
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(OverclocksOf_Bank);
        if (CallFunc_IsValid_ReturnValue) {
            CallFunc_Map_Keys_Keys = {};
            OverclocksOf_Bank->Overclocks.Keys(CallFunc_Map_Keys_Keys);
            OverclocksOf_List = CallFunc_Map_Keys_Keys;
        }
    }
    Overclocks = OverclocksOf_List;
    return;
}

void DB_Data::EncodePart(UItemID* Item, int32 Index, FItemUpgradeSelection Sel, FString& Part) {
    UOverclockUpgrade* CallFunc_Array_Get_Item;
    UItemUpgrade* CallFunc_Array_Get_Item_1;
    FUpgradeTier CallFunc_Array_Get_Item_2;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_EqualEqual_GuidGuid_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_Guid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    TArray<UOverclockUpgrade*> CallFunc_OverclocksOf_Overclocks;
    bool Temp_bool_True_if_break_was_hit_Variable;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Array_Index_Variable_2;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    int32 Temp_int_Loop_Counter_Variable_2;
    EncodePart_Digits = "";
    EncodePart_OC = 0;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Item);
    if (CallFunc_IsValid_ReturnValue) {
        EncodePart_Class = Item->GetItemClass();
        CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(EncodePart_Class);
        if (CallFunc_IsValidClass_ReturnValue) {
            EncodePart_Tiers = UUpgradableGearComponent::GetItemUpgradeTiers((UClass*)EncodePart_Class);
            EncodePart_Equipped = Sel.EquippedUpgrades;
            Temp_int_Loop_Counter_Variable_1 = 0;
            Temp_int_Array_Index_Variable_2 = 0;
            while (true) {
                CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, EncodePart_Tiers.Num());
                if (CallFunc_Less_IntInt_ReturnValue_2) {
                    Temp_int_Array_Index_Variable_2 = Temp_int_Loop_Counter_Variable_1;
                    EncodePart_Digit = 0;
                    EncodePart_Tiers.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_2);
                    EncodePart_Ups = CallFunc_Array_Get_Item_2.Upgrades;
                    Temp_int_Loop_Counter_Variable = 0;
                    Temp_int_Array_Index_Variable_1 = 0;
                    while (true) {
                        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, EncodePart_Ups.Num());
                        if (CallFunc_Less_IntInt_ReturnValue) {
                            Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable;
                            EncodePart_Ups.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_1);
                            if (CallFunc_IsValid_ReturnValue_2) {
                                CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(EncodePart_Digit, 0);
                                EncodePart_Ups.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                                CallFunc_BooleanAND_ReturnValue = EncodePart_Equipped.Contains(CallFunc_Array_Get_Item_1->SavegameID) && CallFunc_EqualEqual_IntInt_ReturnValue;
                                if (CallFunc_BooleanAND_ReturnValue) {
                                    EncodePart_Digit = UKismetMathLibrary::Add_IntInt(Temp_int_Array_Index_Variable_1, 1);
                                }
                            }
                            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                        } else {
                            EncodePart_Digits = UKismetStringLibrary::Concat_StrStr(EncodePart_Digits, UKismetStringLibrary::Conv_IntToString(EncodePart_Digit));
                            Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
                            break;
                        }
                    }
                } else {
                    CallFunc_IsValid_Guid_ReturnValue = UKismetGuidLibrary::IsValid_Guid(Sel.EquippedOverclock);
                    if (CallFunc_IsValid_Guid_ReturnValue) {
                        OverclocksOf(Item, CallFunc_OverclocksOf_Overclocks);
                        EncodePart_OCs = CallFunc_OverclocksOf_Overclocks;
                        Temp_bool_True_if_break_was_hit_Variable = false;
                        Temp_int_Loop_Counter_Variable_2 = 0;
                        Temp_int_Array_Index_Variable = 0;
                        while (true) {
                            CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::BooleanAND(!Temp_bool_True_if_break_was_hit_Variable, UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_2, EncodePart_OCs.Num()));
                            if (CallFunc_BooleanAND_ReturnValue_1) {
                                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable_2;
                                EncodePart_OCs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                                CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item);
                                if (CallFunc_IsValid_ReturnValue_1) {
                                    EncodePart_OCs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                                    CallFunc_EqualEqual_GuidGuid_ReturnValue = CallFunc_Array_Get_Item->SavegameID == Sel.EquippedOverclock;
                                    if (CallFunc_EqualEqual_GuidGuid_ReturnValue) {
                                        EncodePart_OC = UKismetMathLibrary::Add_IntInt(Temp_int_Array_Index_Variable, 1);
                                        Temp_bool_True_if_break_was_hit_Variable = true;
                                    }
                                }
                                Temp_int_Loop_Counter_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_2, 1);
                            } else {
                                goto L_049E;
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
  L_049E:
    EncodePart_Part = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Conv_IntToString(Index), "-"), EncodePart_Digits), "-"), UKismetStringLibrary::Conv_IntToString(EncodePart_OC));
    Part = EncodePart_Part;
    return;
}

void DB_Data::SaveCode(UPlayerCharacterID* Class, FString& Code) {
    int32 CallFunc_Add_IntInt_ReturnValue;
    UItemID* CallFunc_Array_Get_Item;
    FGuid CallFunc_Array_Get_Item_1;
    FString CallFunc_ClassLetter_Letter;
    FString CallFunc_ClassLetter_Name;
    FString CallFunc_EncodePart_Part;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_Map_Find_ReturnValue;
    FItemUpgradeSelection CallFunc_Map_Find_Value;
    TArray<FGuid> CallFunc_SlotItems_Ids;
    TArray<UItemID*> CallFunc_SlotItems_Items;
    TArray<FGuid> K2Node_MakeArray_Array;
    int32 Temp_int_Variable;
    SaveCode_Loadout = ULoadoutFunctionLibrary::GetCurrentLoadout(this, Class);
    SaveCode_Map = SaveCode_Loadout.UpgradeLoadout.Loadout;
    K2Node_MakeArray_Array = {SaveCode_Loadout.ItemLoadout.PrimaryWeapon, SaveCode_Loadout.ItemLoadout.SecondaryWeapon, SaveCode_Loadout.ItemLoadout.TraversalTool, SaveCode_Loadout.ItemLoadout.ClassTool, SaveCode_Loadout.ItemLoadout.Armor, SaveCode_Loadout.ItemLoadout.Grenade, SaveCode_Loadout.ItemLoadout.MiningTool};
    SaveCode_Guids = K2Node_MakeArray_Array;
    ClassLetter(Class, CallFunc_ClassLetter_Letter, CallFunc_ClassLetter_Name);
    SaveCode_Code = UKismetStringLibrary::Concat_StrStr("DB1:", CallFunc_ClassLetter_Letter);
    Temp_int_Variable = 0;
    while (true) {
        CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 6);
        if (CallFunc_LessEqual_IntInt_ReturnValue) {
            SlotItems(Class, Temp_int_Variable, CallFunc_SlotItems_Items, CallFunc_SlotItems_Ids);
            SaveCode_Items = CallFunc_SlotItems_Items;
            SaveCode_Ids = CallFunc_SlotItems_Ids;
            SaveCode_Guids.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
            SaveCode_Found = SaveCode_Ids.Find(CallFunc_Array_Get_Item_1);
            CallFunc_GreaterEqual_IntInt_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(SaveCode_Found, 0);
            if (CallFunc_GreaterEqual_IntInt_ReturnValue) {
                SaveCode_Items.Get(SaveCode_Found, CallFunc_Array_Get_Item);
                SaveCode_Item = CallFunc_Array_Get_Item;
            } else {
                SaveCode_Item = nullptr;
            }
            CallFunc_Add_IntInt_ReturnValue = UKismetMathLibrary::Add_IntInt(SaveCode_Found, 1);
            SaveCode_Guids.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
            CallFunc_Map_Find_ReturnValue = SaveCode_Map.Find(CallFunc_Array_Get_Item_1, CallFunc_Map_Find_Value);
            EncodePart(SaveCode_Item, CallFunc_Add_IntInt_ReturnValue, CallFunc_Map_Find_Value, CallFunc_EncodePart_Part);
            SaveCode_Code = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(SaveCode_Code, ":"), CallFunc_EncodePart_Part);
            Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
        } else {
            Code = SaveCode_Code;
            return;
        }
    }
}

void DB_Data::RemoteCode(AFSDPlayerState* Player, UPlayerCharacterID* Class, FString& Code) {
    UItemID* CallFunc_Array_Get_Item;
    FItemUpgradeSelection CallFunc_Array_Get_Item_1;
    bool CallFunc_BooleanAND_ReturnValue;
    FString CallFunc_ClassLetter_Letter;
    FString CallFunc_ClassLetter_Name;
    FString CallFunc_EncodePart_Part;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    TArray<FGuid> CallFunc_SlotItems_Ids;
    TArray<UItemID*> CallFunc_SlotItems_Items;
    bool Temp_bool_True_if_break_was_hit_Variable;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Variable;
    RemoteCode_Sels.Clear();
    RemoteCode_Any = false;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Player->SaveGameStateComponent);
    if (CallFunc_IsValid_ReturnValue) {
        RemoteCode_Sels = Player->SaveGameStateComponent->ItemUpgradeSelections;
        ClassLetter(Class, CallFunc_ClassLetter_Letter, CallFunc_ClassLetter_Name);
        RemoteCode_Code = UKismetStringLibrary::Concat_StrStr("DB1:", CallFunc_ClassLetter_Letter);
        Temp_int_Variable = 0;
        while (true) {
            CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 6);
            if (CallFunc_LessEqual_IntInt_ReturnValue) {
                SlotItems(Class, Temp_int_Variable, CallFunc_SlotItems_Items, CallFunc_SlotItems_Ids);
                RemoteCode_Items = CallFunc_SlotItems_Items;
                RemoteCode_Ids = CallFunc_SlotItems_Ids;
                RemoteCode_Found = -1;
                RemoteCode_Item = nullptr;
                RemoteCode_Sel = ZeroSel;
                Temp_bool_True_if_break_was_hit_Variable = false;
                Temp_int_Loop_Counter_Variable = 0;
                Temp_int_Array_Index_Variable = 0;
                while (true) {
                    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(!Temp_bool_True_if_break_was_hit_Variable, UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, RemoteCode_Sels.Num()));
                    if (CallFunc_BooleanAND_ReturnValue) {
                        Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                        RemoteCode_Sels.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                        RemoteCode_Found = RemoteCode_Ids.Find(CallFunc_Array_Get_Item_1.WeaponID);
                        CallFunc_GreaterEqual_IntInt_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(RemoteCode_Found, 0);
                        if (CallFunc_GreaterEqual_IntInt_ReturnValue) {
                            RemoteCode_Items.Get(RemoteCode_Found, CallFunc_Array_Get_Item);
                            RemoteCode_Item = CallFunc_Array_Get_Item;
                            RemoteCode_Sels.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                            RemoteCode_Sel = CallFunc_Array_Get_Item_1;
                            RemoteCode_Any = true;
                            Temp_bool_True_if_break_was_hit_Variable = true;
                        }
                        Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                    } else {
                        EncodePart(RemoteCode_Item, UKismetMathLibrary::Add_IntInt(RemoteCode_Found, 1), RemoteCode_Sel, CallFunc_EncodePart_Part);
                        RemoteCode_Code = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(RemoteCode_Code, ":"), CallFunc_EncodePart_Part);
                        Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                        break;
                    }
                }
            } else if (RemoteCode_Any) {
                goto L_04CA;
            } else {
                break;
            }
        }
    }
    RemoteCode_Code = "";
  L_04CA:
    Code = RemoteCode_Code;
    return;
}

void DB_Data::ParseCode(FString Code, bool& Valid) {
    FString CallFunc_Array_Get_Item;
    UPlayerCharacterID* CallFunc_Array_Get_Item_1;
    FString CallFunc_Array_Get_Item_2;
    FString CallFunc_Array_Get_Item_3;
    FString CallFunc_Array_Get_Item_4;
    FString CallFunc_Array_Get_Item_5;
    FString CallFunc_Array_Get_Item_6;
    FString CallFunc_Array_Get_Item_7;
    FString CallFunc_Array_Get_Item_8;
    FString CallFunc_Array_Get_Item_9;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanOR_ReturnValue_5;
    FString CallFunc_ClassLetter_Letter;
    FString CallFunc_ClassLetter_Name;
    int32 CallFunc_Conv_StringToInt_ReturnValue;
    int32 CallFunc_Conv_StringToInt_ReturnValue_1;
    int32 CallFunc_Conv_StringToInt_ReturnValue_2;
    int32 CallFunc_Conv_StringToInt_ReturnValue_3;
    int32 CallFunc_Conv_StringToInt_ReturnValue_4;
    int32 CallFunc_Conv_StringToInt_ReturnValue_5;
    bool CallFunc_EqualEqual_StrStr_ReturnValue;
    bool CallFunc_EqualEqual_StrStr_ReturnValue_1;
    bool CallFunc_Greater_IntInt_ReturnValue;
    bool CallFunc_Greater_IntInt_ReturnValue_1;
    bool CallFunc_Greater_IntInt_ReturnValue_2;
    bool CallFunc_Greater_IntInt_ReturnValue_3;
    bool CallFunc_Greater_IntInt_ReturnValue_4;
    bool CallFunc_Greater_IntInt_ReturnValue_5;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    int32 CallFunc_Max_ReturnValue;
    bool Temp_bool_True_if_break_was_hit_Variable;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::BooleanAND(ParseCode_Valid, UKismetStringLibrary::EqualEqual_StrStr(Code, ParseCode_Code));
    if (!CallFunc_BooleanAND_ReturnValue_1) {
        ParseCode_Code = Code;
        ParseCode_Parts = UKismetStringLibrary::ParseIntoArray(UKismetStringLibrary::ToUpper(UKismetStringLibrary::TrimTrailing(UKismetStringLibrary::Trim(Code))), ":", false);
        ParseCode_Valid = false;
        ParseCode_Class = nullptr;
        CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(ParseCode_Parts.Num(), 10);
        ParseCode_Parts.Resize(CallFunc_Max_ReturnValue);
        ParseCode_Parts.Get(0, CallFunc_Array_Get_Item_9);
        CallFunc_EqualEqual_StrStr_ReturnValue_1 = UKismetStringLibrary::EqualEqual_StrStr(CallFunc_Array_Get_Item_9, "DB1");
        if (CallFunc_EqualEqual_StrStr_ReturnValue_1) {
            ParseCode_Parts.Get(8, CallFunc_Array_Get_Item_2);
            ParseCode_Parts.Get(7, CallFunc_Array_Get_Item_3);
            CallFunc_Conv_StringToInt_ReturnValue = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_2);
            CallFunc_Conv_StringToInt_ReturnValue_1 = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_3);
            CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue, 0);
            CallFunc_Greater_IntInt_ReturnValue_1 = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue_1, 0);
            ParseCode_Parts.Get(6, CallFunc_Array_Get_Item_4);
            ParseCode_Parts.Get(5, CallFunc_Array_Get_Item_5);
            CallFunc_Conv_StringToInt_ReturnValue_2 = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_4);
            CallFunc_Conv_StringToInt_ReturnValue_3 = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_5);
            CallFunc_Greater_IntInt_ReturnValue_2 = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue_2, 0);
            CallFunc_Greater_IntInt_ReturnValue_3 = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue_3, 0);
            ParseCode_Parts.Get(4, CallFunc_Array_Get_Item_6);
            ParseCode_Parts.Get(3, CallFunc_Array_Get_Item_7);
            CallFunc_Conv_StringToInt_ReturnValue_4 = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_6);
            CallFunc_Conv_StringToInt_ReturnValue_5 = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_7);
            CallFunc_Greater_IntInt_ReturnValue_4 = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue_4, 0);
            CallFunc_Greater_IntInt_ReturnValue_5 = UKismetMathLibrary::Greater_IntInt(CallFunc_Conv_StringToInt_ReturnValue_5, 0);
            ParseCode_Parts.Get(2, CallFunc_Array_Get_Item_8);
            CallFunc_BooleanOR_ReturnValue_5 = UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item_8), 0) || CallFunc_Greater_IntInt_ReturnValue_5 || CallFunc_Greater_IntInt_ReturnValue_4 || CallFunc_Greater_IntInt_ReturnValue_3 || CallFunc_Greater_IntInt_ReturnValue_2 || CallFunc_Greater_IntInt_ReturnValue_1 || CallFunc_Greater_IntInt_ReturnValue;
            if (CallFunc_BooleanOR_ReturnValue_5) {
                ParseCode_Data = UGameFunctionLibrary::GetFSDGameData();
                CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(ParseCode_Data);
                if (CallFunc_IsValid_ReturnValue_2) {
                    ParseCode_Settings = ParseCode_Data->CharacterSettings;
                    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(ParseCode_Settings);
                    if (CallFunc_IsValid_ReturnValue_1) {
                        ParseCode_IDs = ParseCode_Settings->PlayerCharacterIDs;
                        Temp_bool_True_if_break_was_hit_Variable = false;
                        Temp_int_Loop_Counter_Variable = 0;
                        Temp_int_Array_Index_Variable = 0;
                        while (true) {
                            CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(!Temp_bool_True_if_break_was_hit_Variable, UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ParseCode_IDs.Num()));
                            if (CallFunc_BooleanAND_ReturnValue) {
                                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                                ParseCode_IDs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_1);
                                if (CallFunc_IsValid_ReturnValue) {
                                    ParseCode_IDs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                    ClassLetter(CallFunc_Array_Get_Item_1, CallFunc_ClassLetter_Letter, CallFunc_ClassLetter_Name);
                                    ParseCode_Parts.Get(1, CallFunc_Array_Get_Item);
                                    CallFunc_EqualEqual_StrStr_ReturnValue = UKismetStringLibrary::EqualEqual_StrStr(CallFunc_ClassLetter_Letter, CallFunc_Array_Get_Item);
                                    if (CallFunc_EqualEqual_StrStr_ReturnValue) {
                                        ParseCode_IDs.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                        ParseCode_Class = CallFunc_Array_Get_Item_1;
                                        ParseCode_Valid = true;
                                        Temp_bool_True_if_break_was_hit_Variable = true;
                                    }
                                }
                                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                            } else {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    Valid = ParseCode_Valid;
    return;
}

void DB_Data::SortPerks() {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Add_ReturnValue_2;
    int32 CallFunc_Array_Add_ReturnValue_3;
    int32 CallFunc_Array_Add_ReturnValue_4;
    int32 CallFunc_Array_Add_ReturnValue_5;
    int32 CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    int32 CallFunc_Array_Get_Item_2;
    UPerkAsset* CallFunc_Array_Get_Item_3;
    int32 CallFunc_Array_Get_Item_4;
    UPerkAsset* CallFunc_Array_Get_Item_5;
    int32 CallFunc_Array_Length_ReturnValue_2;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_2;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    int32 CallFunc_Max_ReturnValue;
    int32 CallFunc_PerkKind_Kind;
    int32 CallFunc_PerkKind_Kind_1;
    int32 CallFunc_PerkKind_Kind_2;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Array_Index_Variable_2;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    int32 Temp_int_Loop_Counter_Variable_2;
    SortPerks_Perks.Clear();
    SortPerks_Ranks.Clear();
    CallFunc_Array_Length_ReturnValue_2 = PerksOf_Perks.Num();
    CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(PerksOf_Ranks.Num(), CallFunc_Array_Length_ReturnValue_2);
    PerksOf_Ranks.Resize(CallFunc_Max_ReturnValue);
    Temp_int_Loop_Counter_Variable_1 = 0;
    Temp_int_Array_Index_Variable_2 = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, PerksOf_Perks.Num());
        if (CallFunc_Less_IntInt_ReturnValue_2) {
            Temp_int_Array_Index_Variable_2 = Temp_int_Loop_Counter_Variable_1;
            PerksOf_Perks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_5);
            CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_5);
            if (CallFunc_IsValid_ReturnValue_2) {
                PerksOf_Perks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_5);
                PerkKind(CallFunc_Array_Get_Item_5, CallFunc_PerkKind_Kind_2);
                CallFunc_EqualEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind_2, 0);
                if (CallFunc_EqualEqual_IntInt_ReturnValue_2) {
                    PerksOf_Perks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_5);
                    CallFunc_Array_Add_ReturnValue_5 = SortPerks_Perks.Add(CallFunc_Array_Get_Item_5);
                    PerksOf_Ranks.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_4);
                    CallFunc_Array_Add_ReturnValue_4 = SortPerks_Ranks.Add(CallFunc_Array_Get_Item_4);
                }
            }
            Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
        } else {
            Temp_int_Loop_Counter_Variable = 0;
            Temp_int_Array_Index_Variable = 0;
            while (true) {
                CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerksOf_Perks.Num());
                if (CallFunc_Less_IntInt_ReturnValue) {
                    Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                    PerksOf_Perks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_3);
                    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_3);
                    if (CallFunc_IsValid_ReturnValue_1) {
                        PerksOf_Perks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_3);
                        PerkKind(CallFunc_Array_Get_Item_3, CallFunc_PerkKind_Kind_1);
                        CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind_1, 1);
                        if (CallFunc_EqualEqual_IntInt_ReturnValue_1) {
                            PerksOf_Perks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_3);
                            CallFunc_Array_Add_ReturnValue_3 = SortPerks_Perks.Add(CallFunc_Array_Get_Item_3);
                            PerksOf_Ranks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_2);
                            CallFunc_Array_Add_ReturnValue_2 = SortPerks_Ranks.Add(CallFunc_Array_Get_Item_2);
                        }
                    }
                    Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                } else {
                    Temp_int_Loop_Counter_Variable_2 = 0;
                    Temp_int_Array_Index_Variable_1 = 0;
                    while (true) {
                        CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_2, PerksOf_Perks.Num());
                        if (CallFunc_Less_IntInt_ReturnValue_1) {
                            Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable_2;
                            PerksOf_Perks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_1);
                            if (CallFunc_IsValid_ReturnValue) {
                                PerksOf_Perks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                                PerkKind(CallFunc_Array_Get_Item_1, CallFunc_PerkKind_Kind);
                                CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 2);
                                if (CallFunc_EqualEqual_IntInt_ReturnValue) {
                                    PerksOf_Perks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_1);
                                    CallFunc_Array_Add_ReturnValue_1 = SortPerks_Perks.Add(CallFunc_Array_Get_Item_1);
                                    PerksOf_Ranks.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                    CallFunc_Array_Add_ReturnValue = SortPerks_Ranks.Add(CallFunc_Array_Get_Item);
                                }
                            }
                            Temp_int_Loop_Counter_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_2, 1);
                        } else {
                            PerksOf_Perks = SortPerks_Perks;
                            PerksOf_Ranks = SortPerks_Ranks;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void DB_Data::ItemTexts(UItemID* Item, FText& Name, FText& Category, FText& Description, UTexture2D*& Line, UTexture2D*& Fill) {
    UUpgradableGearComponent* CallFunc_GearOf_Gear;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    ItemTexts_Name = FText();
    ItemTexts_Category = FText();
    ItemTexts_Description = FText();
    ItemTexts_Line = nullptr;
    ItemTexts_Fill = nullptr;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Item);
    if (CallFunc_IsValid_ReturnValue) {
        ItemTexts_Data = Item->GetItemData();
        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(ItemTexts_Data);
        if (CallFunc_IsValid_ReturnValue_2) {
            ItemTexts_Name = ItemTexts_Data->Name_0;
            ItemTexts_Category = UKismetTextLibrary::TextToUpper(ItemTexts_Data->Category);
            ItemTexts_Description = ItemTexts_Data->Description;
            ItemTexts_Line = ItemTexts_Data->IconLine;
            ItemTexts_Fill = ItemTexts_Data->IconBG;
        } else {
            GearOf(Item, CallFunc_GearOf_Gear);
            ItemTexts_Gear = CallFunc_GearOf_Gear;
            CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(ItemTexts_Gear);
            if (CallFunc_IsValid_ReturnValue_1) {
                ItemTexts_Name = ItemTexts_Gear->GetGearName();
                ItemTexts_Category = UKismetTextLibrary::TextToUpper(ItemTexts_Gear->GetGearCategory());
                ItemTexts_Description = ItemTexts_Gear->GetGearDescription();
                ItemTexts_Line = ItemTexts_Gear->GetIconLine();
                ItemTexts_Fill = ItemTexts_Gear->GetIconBG();
            }
        }
    }
    Name = ItemTexts_Name;
    Category = ItemTexts_Category;
    Description = ItemTexts_Description;
    Line = ItemTexts_Line;
    Fill = ItemTexts_Fill;
    return;
}

void DB_Data::StatBody(UItemUpgrade* Upgrade, FText& Body) {
    FItemUpgradeStatText CallFunc_Array_Get_Item;
    FString CallFunc_Concat_StrStr_ReturnValue;
    bool CallFunc_Greater_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    StatBody_Text = UKismetTextLibrary::Conv_TextToString(Upgrade->Description);
    StatBody_Stats = Upgrade->GetUpgradeStatTexts();
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, StatBody_Stats.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            StatBody_Stats.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
            StatBody_Line = UKismetTextLibrary::Conv_TextToString(CallFunc_Array_Get_Item.StatText);
            CallFunc_Greater_IntInt_ReturnValue_1 = UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Len(StatBody_Line), 0);
            if (CallFunc_Greater_IntInt_ReturnValue_1) {
                CallFunc_Concat_StrStr_ReturnValue = UKismetStringLibrary::Concat_StrStr(StatBody_Text, UKismetMathLibrary::SelectString("\n", "", UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Len(StatBody_Text), 0)));
                StatBody_Stats.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                StatBody_Text = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(CallFunc_Concat_StrStr_ReturnValue, UKismetMathLibrary::SelectString("+ ", "- ", CallFunc_Array_Get_Item.IsAdventageous)), StatBody_Line);
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Body = UKismetTextLibrary::Conv_StringToText(StatBody_Text);
            return;
        }
    }
}

void DB_Data::StripTags(FString Source, FString& Out) {
    int32 CallFunc_Add_IntInt_ReturnValue_1;
    FString CallFunc_Array_Get_Item;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    StripTags_Out = "";
    StripTags_Pieces = UKismetStringLibrary::ParseIntoArray(Source, "<", false);
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, StripTags_Pieces.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(Temp_int_Array_Index_Variable, 0);
            if (CallFunc_EqualEqual_IntInt_ReturnValue) {
                StripTags_Pieces.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                StripTags_Out = CallFunc_Array_Get_Item;
            } else {
                StripTags_Pieces.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                StripTags_Close = UKismetStringLibrary::FindSubstring(CallFunc_Array_Get_Item, ">", false, false, -1);
                CallFunc_GreaterEqual_IntInt_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(StripTags_Close, 0);
                if (CallFunc_GreaterEqual_IntInt_ReturnValue) {
                    CallFunc_Add_IntInt_ReturnValue_1 = UKismetMathLibrary::Add_IntInt(StripTags_Close, 1);
                    StripTags_Pieces.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    StripTags_Out = UKismetStringLibrary::Concat_StrStr(StripTags_Out, UKismetStringLibrary::GetSubstring(CallFunc_Array_Get_Item, CallFunc_Add_IntInt_ReturnValue_1, UKismetStringLibrary::Len(CallFunc_Array_Get_Item)));
                }
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Out = StripTags_Out;
            return;
        }
    }
}

void DB_Data::ApplyPart(FString Code, int32 CodeSlot) {
    int32 CallFunc_Array_AddUnique_ReturnValue;
    UItemUpgrade* CallFunc_Array_Get_Item;
    int32 CallFunc_Array_Get_Item_1;
    FUpgradeTier CallFunc_Array_Get_Item_2;
    int32 CallFunc_Array_Length_ReturnValue_2;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue_2;
    TArray<int32> CallFunc_DecodeSlot_Choices;
    FString CallFunc_DecodeSlot_Digits;
    bool CallFunc_DecodeSlot_Found;
    UItemID* CallFunc_DecodeSlot_Item;
    UOverclockUpgrade* CallFunc_DecodeSlot_Overclock;
    TArray<UItemUpgrade*> CallFunc_GetEquippableOverclocks_ReturnValue;
    bool CallFunc_IsItemOwned_ReturnValue;
    bool CallFunc_IsTierUnLocked_ReturnValue;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue_1;
    bool CallFunc_LessEqual_IntInt_ReturnValue_2;
    bool CallFunc_Map_Find_ReturnValue;
    FItemUpgradeSelection CallFunc_Map_Find_Value;
    int32 CallFunc_Max_ReturnValue;
    FItemUpgradeSelection K2Node_MakeStruct_ItemUpgradeSelection;
    bool K2Node_Select_Default;
    bool K2Node_Select_Default_1;
    bool Temp_bool_Variable;
    bool Temp_bool_Variable_1;
    bool Temp_bool_Variable_2;
    bool Temp_bool_Variable_3;
    bool Temp_bool_Variable_4;
    bool Temp_bool_Variable_5;
    bool Temp_bool_Variable_6;
    bool Temp_bool_Variable_7;
    bool Temp_bool_Variable_8;
    bool Temp_bool_Variable_9;
    EItemUpgradeStatus Temp_byte_Variable;
    int32 Temp_int_Variable;
    bool UnknownOc;
    DecodeSlot(Code, CodeSlot, CallFunc_DecodeSlot_Item, CallFunc_DecodeSlot_Choices, CallFunc_DecodeSlot_Overclock, CallFunc_DecodeSlot_Digits, CallFunc_DecodeSlot_Found, UnknownOc);
    ApplyPart_Item = CallFunc_DecodeSlot_Item;
    ApplyPart_Choices = CallFunc_DecodeSlot_Choices;
    ApplyPart_Overclock = CallFunc_DecodeSlot_Overclock;
    if (CallFunc_DecodeSlot_Found) {
        CallFunc_IsItemOwned_ReturnValue = UUpgradableGearComponent::IsItemOwned(this, ApplyPart_Item);
        if (CallFunc_IsItemOwned_ReturnValue) {
            ApplyPart_Class = ApplyPart_Item->GetItemClass();
            CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(ApplyPart_Class);
            if (CallFunc_IsValidClass_ReturnValue) {
                ApplyPart_Guid = ApplyPart_Item->SavegameID;
                ApplyCode_Guids.Set(CodeSlot, ApplyPart_Guid, false);
                ApplyCode_Applied = UKismetMathLibrary::Add_IntInt(ApplyCode_Applied, 1);
                CallFunc_Map_Find_ReturnValue = ApplyCode_Map.Find(ApplyPart_Guid, CallFunc_Map_Find_Value);
                if (CallFunc_Map_Find_ReturnValue) {
                    CallFunc_Map_Find_ReturnValue = ApplyCode_Map.Find(ApplyPart_Guid, CallFunc_Map_Find_Value);
                    ApplyPart_Sel = CallFunc_Map_Find_Value;
                } else {
                    K2Node_MakeStruct_ItemUpgradeSelection.WeaponID = ApplyPart_Guid;
                    K2Node_MakeStruct_ItemUpgradeSelection.EquippedOverclock = FGuid(0, 0, 0, 0);
                    K2Node_MakeStruct_ItemUpgradeSelection.OverclockingUnlocked = false;
                    K2Node_MakeStruct_ItemUpgradeSelection.EquippedSkinColor = FGuid(0, 0, 0, 0);
                    K2Node_MakeStruct_ItemUpgradeSelection.EquippedSkinMesh = FGuid(0, 0, 0, 0);
                    ApplyPart_Sel = K2Node_MakeStruct_ItemUpgradeSelection;
                }
                ApplyPart_Ups = ApplyPart_Sel.EquippedUpgrades;
                ApplyPart_OcGuid = ApplyPart_Sel.EquippedOverclock;
                ApplyPart_Tiers = UUpgradableGearComponent::GetItemUpgradeTiers((UClass*)ApplyPart_Class);
                CallFunc_Array_Length_ReturnValue_2 = ApplyPart_Tiers.Num();
                CallFunc_Max_ReturnValue = UKismetMathLibrary::Max(ApplyPart_Choices.Num(), CallFunc_Array_Length_ReturnValue_2);
                ApplyPart_Choices.Resize(CallFunc_Max_ReturnValue);
                Temp_int_Variable = 0;
                while (true) {
                    CallFunc_LessEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, UKismetMathLibrary::Subtract_IntInt(ApplyPart_Tiers.Num(), 1));
                    if (CallFunc_LessEqual_IntInt_ReturnValue_2) {
                        ApplyPart_Tiers.Get(Temp_int_Variable, CallFunc_Array_Get_Item_2);
                        ApplyPart_TierUps = CallFunc_Array_Get_Item_2.Upgrades;
                        ApplyPart_Choices.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                        ApplyPart_Choice = CallFunc_Array_Get_Item_1;
                        CallFunc_LessEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::LessEqual_IntInt(ApplyPart_Choice, 0);
                        if (ApplyPart_Choice < 0) {
                            // An option this game does not have: the slot's own choice stays.
                            ApplyCode_Unknown += 1;
                        } else if (CallFunc_LessEqual_IntInt_ReturnValue_1) {
                            ClearTier();
                        } else {
                            CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(ApplyPart_Choice, ApplyPart_TierUps.Num());
                            if (CallFunc_LessEqual_IntInt_ReturnValue) {
                                ApplyPart_TierUps.Get(UKismetMathLibrary::Subtract_IntInt(ApplyPart_Choice, 1), CallFunc_Array_Get_Item);
                                ApplyPart_Upgrade = CallFunc_Array_Get_Item;
                                CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(ApplyPart_Upgrade);
                                if (CallFunc_IsValid_ReturnValue_1) {
                                    Temp_bool_Variable_5 = true;
                                    Temp_bool_Variable_6 = true;
                                    Temp_bool_Variable_7 = false;
                                    Temp_bool_Variable_8 = false;
                                    Temp_bool_Variable_9 = false;
                                    CallFunc_IsTierUnLocked_ReturnValue = UUpgradableGearComponent::IsTierUnLocked(this, (UClass*)ApplyPart_Class, Temp_int_Variable, ApplyCode_Class);
                                    CallFunc_BooleanAND_ReturnValue_2 = (UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Upgrade, ApplyCode_Class) == EItemUpgradeStatus::Locked ? Temp_bool_Variable_9 : UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Upgrade, ApplyCode_Class) == EItemUpgradeStatus::AvailableCannotAfford ? Temp_bool_Variable_8 : UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Upgrade, ApplyCode_Class) == EItemUpgradeStatus::AvailableCanAfford ? Temp_bool_Variable_7 : UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Upgrade, ApplyCode_Class) == EItemUpgradeStatus::Owned ? Temp_bool_Variable_6 : UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Upgrade, ApplyCode_Class) == EItemUpgradeStatus::Equipped ? Temp_bool_Variable_5 : K2Node_Select_Default_1) && CallFunc_IsTierUnLocked_ReturnValue;
                                    if (CallFunc_BooleanAND_ReturnValue_2) {
                                        ClearTier();
                                        CallFunc_Array_AddUnique_ReturnValue = ApplyPart_Ups.AddUnique(ApplyPart_Upgrade->SavegameID);
                                        ApplyCode_Applied = UKismetMathLibrary::Add_IntInt(ApplyCode_Applied, 1);
                                    } else {
                                        ApplyCode_Skipped = UKismetMathLibrary::Add_IntInt(ApplyCode_Skipped, 1);
                                    }
                                }
                            }
                        }
                        Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                    } else {
                        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(ApplyPart_Overclock);
                        if (CallFunc_IsValid_ReturnValue) {
                            CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::BooleanAND(UUpgradableGearComponent::CanItemOverclock(this, (UClass*)ApplyPart_Class), UUpgradableGearComponent::IsOverclockingEnabled(this, ApplyCode_Class, (UClass*)ApplyPart_Class));
                            if (CallFunc_BooleanAND_ReturnValue_1) {
                                Temp_bool_Variable = true;
                                Temp_bool_Variable_1 = true;
                                Temp_bool_Variable_2 = false;
                                Temp_bool_Variable_3 = false;
                                Temp_bool_Variable_4 = false;
                                Temp_byte_Variable = UUpgradableGearComponent::GetItemUpgradeStatus(this, (UClass*)ApplyPart_Class, ApplyPart_Overclock, ApplyCode_Class);
                                CallFunc_GetEquippableOverclocks_ReturnValue = UUpgradableGearComponent::GetEquippableOverclocks(this, (UClass*)ApplyPart_Class, ApplyCode_Class);
                                CallFunc_BooleanAND_ReturnValue = CallFunc_GetEquippableOverclocks_ReturnValue.Contains(ApplyPart_Overclock) && (Temp_byte_Variable == EItemUpgradeStatus::Locked ? Temp_bool_Variable_4 : Temp_byte_Variable == EItemUpgradeStatus::AvailableCannotAfford ? Temp_bool_Variable_3 : Temp_byte_Variable == EItemUpgradeStatus::AvailableCanAfford ? Temp_bool_Variable_2 : Temp_byte_Variable == EItemUpgradeStatus::Owned ? Temp_bool_Variable_1 : Temp_byte_Variable == EItemUpgradeStatus::Equipped ? Temp_bool_Variable : K2Node_Select_Default);
                                if (CallFunc_BooleanAND_ReturnValue) {
                                    ApplyPart_OcGuid = ApplyPart_Overclock->SavegameID;
                                    ApplyCode_Applied = UKismetMathLibrary::Add_IntInt(ApplyCode_Applied, 1);
                                    goto L_0990;
                                }
                            }
                            ApplyCode_Skipped = UKismetMathLibrary::Add_IntInt(ApplyCode_Skipped, 1);
                        } else if (UnknownOc) {
                            ApplyCode_Unknown += 1;   // an overclock this game does not have: the slot's own stays
                        } else {
                            ApplyPart_OcGuid = ZeroGuid;
                        }
                      L_0990:
                        ApplyPart_Sel.EquippedUpgrades = ApplyPart_Ups;
                        ApplyPart_Sel.EquippedOverclock = ApplyPart_OcGuid;
                        ApplyCode_Map.Add(ApplyPart_Guid, ApplyPart_Sel);
                        return;
                    }
                }
            }
        }
        ApplyCode_Skipped = UKismetMathLibrary::Add_IntInt(ApplyCode_Skipped, 1);
        return;
    } else {
        return;
    }
}

void DB_Data::ApplyPerks(FString Code) {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Add_ReturnValue_2;
    int32 CallFunc_Array_Add_ReturnValue_3;
    int32 CallFunc_Array_Add_ReturnValue_4;
    int32 CallFunc_Array_Add_ReturnValue_5;
    int32 CallFunc_Array_Add_ReturnValue_6;
    int32 CallFunc_Array_Add_ReturnValue_7;
    bool CallFunc_Array_Contains_ReturnValue_1;
    UPerkAsset* CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    UPerkAsset* CallFunc_Array_Get_Item_10;
    UPerkAsset* CallFunc_Array_Get_Item_2;
    int32 CallFunc_Array_Get_Item_3;
    int32 CallFunc_Array_Get_Item_4;
    int32 CallFunc_Array_Get_Item_5;
    UPerkAsset* CallFunc_Array_Get_Item_6;
    int32 CallFunc_Array_Get_Item_7;
    int32 CallFunc_Array_Get_Item_8;
    int32 CallFunc_Array_Get_Item_9;
    bool CallFunc_BooleanAND_ReturnValue;
    FString CallFunc_Conv_TextToString_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_1;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_2;
    bool CallFunc_EqualEqual_IntInt_ReturnValue_3;
    TSubclassOf<APlayerCharacter> CallFunc_GetCharacterClass_ReturnValue;
    TArray<UPerkAsset*> CallFunc_GetOwnedPerksByType_ReturnValue;
    int32 CallFunc_GetPerkEquipSlots_OutAvailableSlots;
    int32 CallFunc_GetPerkEquipSlots_OutAvailableSlots_1;
    int32 CallFunc_GetPerkEquipSlots_OutAvailableSlots_2;
    int32 CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots;
    int32 CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots_1;
    int32 CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots_2;
    int32 CallFunc_GetPerkEquipSlots_OutUnavailableSlots;
    int32 CallFunc_GetPerkEquipSlots_OutUnavailableSlots_1;
    int32 CallFunc_GetPerkEquipSlots_OutUnavailableSlots_2;
    bool CallFunc_HasUnlockProficiency_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue_3;
    bool CallFunc_Less_IntInt_ReturnValue_4;
    bool CallFunc_Less_IntInt_ReturnValue_5;
    bool CallFunc_Less_IntInt_ReturnValue_6;
    bool CallFunc_Not_PreBool_ReturnValue_1;
    int32 CallFunc_PerkKind_Kind;
    int32 CallFunc_PerkKind_Kind_1;
    int32 CallFunc_PerkKind_Kind_2;
    int32 CallFunc_PerkKind_Kind_3;
    int32 CallFunc_PerkKind_Kind_4;
    bool CallFunc_PerksOfCode_Found;
    TArray<UPerkAsset*> CallFunc_PerksOfCode_Perks;
    TArray<int32> CallFunc_PerksOfCode_Ranks;
    UProficiencyPerkAsset* K2Node_DynamicCast_AsProficiency_Perk_Asset;
    bool K2Node_DynamicCast_bSuccess;
    TArray<int32> K2Node_MakeArray_Array;
    TArray<int32> K2Node_MakeArray_Array_1;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Array_Index_Variable_2;
    int32 Temp_int_Array_Index_Variable_3;
    int32 Temp_int_Array_Index_Variable_4;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    int32 Temp_int_Loop_Counter_Variable_2;
    int32 Temp_int_Loop_Counter_Variable_3;
    int32 Temp_int_Loop_Counter_Variable_4;
    ApplyPerks_Chosen.Clear();
    PerksOfCode(Code, CallFunc_PerksOfCode_Perks, CallFunc_PerksOfCode_Ranks, CallFunc_PerksOfCode_Found);
    ApplyPerks_Code = CallFunc_PerksOfCode_Perks;
    ApplyPerks_Unknown = PerksOfCode_Unknown;
    if (CallFunc_PerksOfCode_Found) {
        CallFunc_GetCharacterClass_ReturnValue = ApplyCode_Class->GetCharacterClass();
        UPerkFunctionLibrary::GetPerkEquipSlots(this, EPerkUsageType::Proficiency, CallFunc_GetCharacterClass_ReturnValue, CallFunc_GetPerkEquipSlots_OutAvailableSlots, CallFunc_GetPerkEquipSlots_OutUnavailableSlots, CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots);
        UPerkFunctionLibrary::GetPerkEquipSlots(this, EPerkUsageType::Active, CallFunc_GetCharacterClass_ReturnValue, CallFunc_GetPerkEquipSlots_OutAvailableSlots_1, CallFunc_GetPerkEquipSlots_OutUnavailableSlots_1, CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots_1);
        UPerkFunctionLibrary::GetPerkEquipSlots(this, EPerkUsageType::Passive, CallFunc_GetCharacterClass_ReturnValue, CallFunc_GetPerkEquipSlots_OutAvailableSlots_2, CallFunc_GetPerkEquipSlots_OutUnavailableSlots_2, CallFunc_GetPerkEquipSlots_OutPromotionLockedSlots_2);
        K2Node_MakeArray_Array_1 = {CallFunc_GetPerkEquipSlots_OutAvailableSlots_2, CallFunc_GetPerkEquipSlots_OutAvailableSlots_1, CallFunc_GetPerkEquipSlots_OutAvailableSlots};
        ApplyPerks_Slots = K2Node_MakeArray_Array_1;
        K2Node_MakeArray_Array = {0, 0, 0};
        ApplyPerks_Used = K2Node_MakeArray_Array;
        ApplyPerks_Owned = UPerkFunctionLibrary::GetOwnedPerks(this);
        CallFunc_GetOwnedPerksByType_ReturnValue = UPerkFunctionLibrary::GetOwnedPerksByType(this, EPerkUsageType::Proficiency);
        ApplyPerks_Owned.Append(CallFunc_GetOwnedPerksByType_ReturnValue);
        ApplyPerks_Old = UPerkFunctionLibrary::GetCharacterEquippedPerks(this, ApplyCode_Class);
        Temp_int_Loop_Counter_Variable_4 = 0;
        Temp_int_Array_Index_Variable_4 = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue_6 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_4, ApplyPerks_Code.Num());
            if (CallFunc_Less_IntInt_ReturnValue_6) {
                Temp_int_Array_Index_Variable_4 = Temp_int_Loop_Counter_Variable_4;
                ApplyPerks_Code.Get(Temp_int_Array_Index_Variable_4, CallFunc_Array_Get_Item_10);
                ApplyPerks_Perk = CallFunc_Array_Get_Item_10;
                PerkKind(ApplyPerks_Perk, CallFunc_PerkKind_Kind_4);
                ApplyPerks_Kind = CallFunc_PerkKind_Kind_4;
                CallFunc_Not_PreBool_ReturnValue_1 = !ApplyPerks_Chosen.Contains(ApplyPerks_Perk);
                if (CallFunc_Not_PreBool_ReturnValue_1) {
                    CallFunc_Array_Contains_ReturnValue_1 = ApplyPerks_Owned.Contains(ApplyPerks_Perk);
                    if (CallFunc_Array_Contains_ReturnValue_1) {
                        CallFunc_EqualEqual_IntInt_ReturnValue_3 = UKismetMathLibrary::EqualEqual_IntInt(ApplyPerks_Kind, 2);
                        if (CallFunc_EqualEqual_IntInt_ReturnValue_3) {
                            K2Node_DynamicCast_AsProficiency_Perk_Asset = Cast<UProficiencyPerkAsset>(ApplyPerks_Perk);
                            K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsProficiency_Perk_Asset != nullptr;
                            if (K2Node_DynamicCast_bSuccess) {
                                CallFunc_HasUnlockProficiency_ReturnValue = ApplyCode_Save->HasUnlockProficiency(K2Node_DynamicCast_AsProficiency_Perk_Asset->Proficiency);
                                if (!CallFunc_HasUnlockProficiency_ReturnValue) {
                                    CallFunc_Conv_TextToString_ReturnValue = UKismetTextLibrary::Conv_TextToString(ApplyPerks_Perk->Title);
                                    CallFunc_Array_Add_ReturnValue_4 = ApplyPerks_Skips.Add(UKismetStringLibrary::Concat_StrStr(CallFunc_Conv_TextToString_ReturnValue, " (locked)"));
                                    goto L_10E4;
                                }
                            }
                        }
                        ApplyPerks_Slots.Get(ApplyPerks_Kind, CallFunc_Array_Get_Item_8);
                        ApplyPerks_Used.Get(ApplyPerks_Kind, CallFunc_Array_Get_Item_9);
                        CallFunc_Less_IntInt_ReturnValue_5 = UKismetMathLibrary::Less_IntInt(CallFunc_Array_Get_Item_9, CallFunc_Array_Get_Item_8);
                        if (CallFunc_Less_IntInt_ReturnValue_5) {
                            CallFunc_Array_Add_ReturnValue_6 = ApplyPerks_Chosen.Add(ApplyPerks_Perk);
                            ApplyPerks_Used.Get(ApplyPerks_Kind, CallFunc_Array_Get_Item_7);
                            ApplyPerks_Used.Set(ApplyPerks_Kind, UKismetMathLibrary::Add_IntInt(CallFunc_Array_Get_Item_7, 1), false);
                            ApplyCode_Applied = UKismetMathLibrary::Add_IntInt(ApplyCode_Applied, 1);
                        } else {
                            CallFunc_Conv_TextToString_ReturnValue = UKismetTextLibrary::Conv_TextToString(ApplyPerks_Perk->Title);
                            CallFunc_Array_Add_ReturnValue_5 = ApplyPerks_Skips.Add(UKismetStringLibrary::Concat_StrStr(CallFunc_Conv_TextToString_ReturnValue, " (no free slot)"));
                        }
                    } else {
                        CallFunc_Conv_TextToString_ReturnValue = UKismetTextLibrary::Conv_TextToString(ApplyPerks_Perk->Title);
                        CallFunc_Array_Add_ReturnValue_7 = ApplyPerks_Skips.Add(UKismetStringLibrary::Concat_StrStr(CallFunc_Conv_TextToString_ReturnValue, " (not owned)"));
                    }
                }
              L_10E4:
                Temp_int_Loop_Counter_Variable_4 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_4, 1);
            } else {
                Temp_int_Loop_Counter_Variable_2 = 0;
                Temp_int_Array_Index_Variable_2 = 0;
                while (true) {
                    CallFunc_Less_IntInt_ReturnValue_4 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_2, ApplyPerks_Old.Num());
                    if (CallFunc_Less_IntInt_ReturnValue_4) {
                        Temp_int_Array_Index_Variable_2 = Temp_int_Loop_Counter_Variable_2;
                        ApplyPerks_Old.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_6);
                        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_6);
                        if (CallFunc_IsValid_ReturnValue) {
                            ApplyPerks_Old.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_6);
                            PerkKind(CallFunc_Array_Get_Item_6, CallFunc_PerkKind_Kind_3);
                            ApplyPerks_OldKind = CallFunc_PerkKind_Kind_3;
                            ApplyPerks_Slots.Get(ApplyPerks_OldKind, CallFunc_Array_Get_Item_4);
                            ApplyPerks_Used.Get(ApplyPerks_OldKind, CallFunc_Array_Get_Item_5);
                            CallFunc_Less_IntInt_ReturnValue_3 = UKismetMathLibrary::Less_IntInt(CallFunc_Array_Get_Item_5, CallFunc_Array_Get_Item_4);
                            ApplyPerks_Old.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_6);
                            CallFunc_BooleanAND_ReturnValue = !ApplyPerks_Chosen.Contains(CallFunc_Array_Get_Item_6) && CallFunc_Less_IntInt_ReturnValue_3;
                            if (CallFunc_BooleanAND_ReturnValue) {
                                ApplyPerks_Old.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_6);
                                CallFunc_Array_Add_ReturnValue_3 = ApplyPerks_Chosen.Add(CallFunc_Array_Get_Item_6);
                                ApplyPerks_Used.Get(ApplyPerks_OldKind, CallFunc_Array_Get_Item_3);
                                ApplyPerks_Used.Set(ApplyPerks_OldKind, UKismetMathLibrary::Add_IntInt(CallFunc_Array_Get_Item_3, 1), false);
                            }
                        }
                        Temp_int_Loop_Counter_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_2, 1);
                    } else {
                        ApplyPerks_Final.Clear();
                        Temp_int_Loop_Counter_Variable = 0;
                        Temp_int_Array_Index_Variable_1 = 0;
                        while (true) {
                            CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ApplyPerks_Chosen.Num());
                            if (CallFunc_Less_IntInt_ReturnValue_2) {
                                Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable;
                                ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_2);
                                PerkKind(CallFunc_Array_Get_Item_2, CallFunc_PerkKind_Kind_2);
                                CallFunc_EqualEqual_IntInt_ReturnValue_2 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind_2, 0);
                                if (CallFunc_EqualEqual_IntInt_ReturnValue_2) {
                                    ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item_2);
                                    CallFunc_Array_Add_ReturnValue_2 = ApplyPerks_Final.Add(CallFunc_Array_Get_Item_2);
                                }
                                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                            } else {
                                Temp_int_Loop_Counter_Variable_3 = 0;
                                Temp_int_Array_Index_Variable = 0;
                                while (true) {
                                    CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_3, ApplyPerks_Chosen.Num());
                                    if (CallFunc_Less_IntInt_ReturnValue_1) {
                                        Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable_3;
                                        ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                        PerkKind(CallFunc_Array_Get_Item_1, CallFunc_PerkKind_Kind_1);
                                        CallFunc_EqualEqual_IntInt_ReturnValue_1 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind_1, 1);
                                        if (CallFunc_EqualEqual_IntInt_ReturnValue_1) {
                                            ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                            CallFunc_Array_Add_ReturnValue_1 = ApplyPerks_Final.Add(CallFunc_Array_Get_Item_1);
                                        }
                                        Temp_int_Loop_Counter_Variable_3 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_3, 1);
                                    } else {
                                        Temp_int_Loop_Counter_Variable_1 = 0;
                                        Temp_int_Array_Index_Variable_3 = 0;
                                        while (true) {
                                            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, ApplyPerks_Chosen.Num());
                                            if (CallFunc_Less_IntInt_ReturnValue) {
                                                Temp_int_Array_Index_Variable_3 = Temp_int_Loop_Counter_Variable_1;
                                                ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable_3, CallFunc_Array_Get_Item);
                                                PerkKind(CallFunc_Array_Get_Item, CallFunc_PerkKind_Kind);
                                                CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_PerkKind_Kind, 2);
                                                if (CallFunc_EqualEqual_IntInt_ReturnValue) {
                                                    ApplyPerks_Chosen.Get(Temp_int_Array_Index_Variable_3, CallFunc_Array_Get_Item);
                                                    CallFunc_Array_Add_ReturnValue = ApplyPerks_Final.Add(CallFunc_Array_Get_Item);
                                                }
                                                Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
                                            } else {
                                                UPerkAsset::SetCharacterPerks(this, ApplyCode_Class, ApplyPerks_Final);
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
    } else {
        return;
    }
}

void DB_Data::EnsurePerkNames() {
    bool CallFunc_EqualEqual_IntInt_ReturnValue;
    TArray<FString> K2Node_MakeArray_Array;
    CallFunc_EqualEqual_IntInt_ReturnValue = UKismetMathLibrary::EqualEqual_IntInt(PerkNames.Num(), 0);
    if (CallFunc_EqualEqual_IntInt_ReturnValue) {
        K2Node_MakeArray_Array = {"PERK_Berzerker", "PERK_DeepPockets", "PERK_ExpertDepositor", "PERK_FieldMedic", "PERK_RedSugarRocks", "PERK_Resupplier", "PERK_StrongBack", "PERK_IronWill", "PERK_Vampire", "PERK_FlareThrower", "PERK_FriendlyFire", "PERK_HeightenedSenses", "PERK_TemperatureInsulation", "PERK_MarathonGuy", "PERK_BornReady", "PERK_ShieldLink", "PERK_Dash", "PERK_BugThing", "PERK_BeastMaster", "PERK_DownedBomb", "PERK_Thorns", "PERK_HoverBoots", "PERK_ManagementsFavorite", "PERK_SecondChance", "PERK_Executioner", "PERK_LiquidCourage", "PERK_FinishThem", "PERK_LastCall", "PERK_FearTheBeard", "PERK_PickKills", "PERK_NoDwarfLeftBehind", "PERK_HoldTheLine", "PERK_Taunt", "Proficiency_Combat", "Proficiency_Survival", "Proficiency_Mobility", "Proficiency_Resourcefulness"};
        PerkNames = K2Node_MakeArray_Array;
        return;
    } else {
        return;
    }
}

void DB_Data::EnsurePerkTable() {
    UPerkAsset* CallFunc_Array_Get_Item;
    int32 CallFunc_Array_Length_ReturnValue_2;
    FString CallFunc_GetObjectName_ReturnValue;
    TArray<UPerkAsset*> CallFunc_GetOwnedPerks_ReturnValue;
    bool CallFunc_GreaterEqual_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    if (PerkTableDone) {
        return;
    } else {
        EnsurePerkNames();
        PerkTable.Clear();
        CallFunc_Array_Length_ReturnValue_2 = PerkNames.Num();
        PerkTable.Resize(CallFunc_Array_Length_ReturnValue_2);
        PerkTable_All = UPerkFunctionLibrary::GetAvailablePerks();
        CallFunc_GetOwnedPerks_ReturnValue = UPerkFunctionLibrary::GetOwnedPerks(this);
        PerkTable_All.Append(CallFunc_GetOwnedPerks_ReturnValue);
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerkTable_All.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                PerkTable_All.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item);
                if (CallFunc_IsValid_ReturnValue) {
                    PerkTable_All.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    CallFunc_GetObjectName_ReturnValue = UKismetSystemLibrary::GetObjectName(CallFunc_Array_Get_Item);
                    PerkTable_Index = PerkNames.Find(CallFunc_GetObjectName_ReturnValue);
                    CallFunc_GreaterEqual_IntInt_ReturnValue = UKismetMathLibrary::GreaterEqual_IntInt(PerkTable_Index, 0);
                    if (CallFunc_GreaterEqual_IntInt_ReturnValue) {
                        PerkTable_All.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                        PerkTable.Set(PerkTable_Index, CallFunc_Array_Get_Item, false);
                    }
                }
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                PerkTableDone = UKismetMathLibrary::Greater_IntInt(PerkTable_All.Num(), 0);
                return;
            }
        }
    }
}

void DB_Data::PerkId(UPerkAsset* Perk, int32& Id) {
    FString CallFunc_GetObjectName_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    PerkId_Id = 0;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Perk);
    if (CallFunc_IsValid_ReturnValue) {
        EnsurePerkNames();
        CallFunc_GetObjectName_ReturnValue = UKismetSystemLibrary::GetObjectName(Perk);
        PerkId_Id = UKismetMathLibrary::Add_IntInt(PerkNames.Find(CallFunc_GetObjectName_ReturnValue), 1);
    }
    Id = PerkId_Id;
    return;
}

void DB_Data::PerksPart(TArray<UPerkAsset*>& Perks, FString& Part) {
    UPerkAsset* CallFunc_Array_Get_Item;
    bool CallFunc_Greater_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 CallFunc_PerkId_Id;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    PerksPart_Part = "";
    PerksPart_Perks = Perks;
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerksPart_Perks.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            PerksPart_Perks.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
            PerkId(CallFunc_Array_Get_Item, CallFunc_PerkId_Id);
            CallFunc_Greater_IntInt_ReturnValue_1 = UKismetMathLibrary::Greater_IntInt(CallFunc_PerkId_Id, 0);
            if (CallFunc_Greater_IntInt_ReturnValue_1) {
                PerksPart_Part = UKismetStringLibrary::Concat_StrStr(UKismetStringLibrary::Concat_StrStr(PerksPart_Part, UKismetMathLibrary::SelectString(".", "", UKismetMathLibrary::Greater_IntInt(UKismetStringLibrary::Len(PerksPart_Part), 0))), UKismetStringLibrary::Conv_IntToString(CallFunc_PerkId_Id));
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Part = PerksPart_Part;
            return;
        }
    }
}

void DB_Data::PerkIds(FString Part, TArray<int32>& Ids) {
    int32 CallFunc_Array_AddUnique_ReturnValue;
    FString CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    PerkIds_Ids.Clear();
    PerkIds_Bad = 0;
    EnsurePerkNames();
    PerkIds_Pieces = UKismetStringLibrary::ParseIntoArray(Part, ".", true);
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, PerkIds_Pieces.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            PerkIds_Pieces.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
            PerkIds_Id = UKismetStringLibrary::Conv_StringToInt(CallFunc_Array_Get_Item);
            CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(UKismetMathLibrary::GreaterEqual_IntInt(PerkIds_Id, 1), UKismetMathLibrary::LessEqual_IntInt(PerkIds_Id, PerkNames.Num()));
            if (CallFunc_BooleanAND_ReturnValue) {
                CallFunc_Array_AddUnique_ReturnValue = PerkIds_Ids.AddUnique(PerkIds_Id);
            } else {
                PerkIds_Bad = UKismetMathLibrary::Add_IntInt(PerkIds_Bad, 1);
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Ids = PerkIds_Ids;
            return;
        }
    }
}

void DB_Data::PerkKind(UPerkAsset* Perk, int32& Kind) {
    bool CallFunc_IsUsageType_ReturnValue;
    CallFunc_IsUsageType_ReturnValue = Perk->IsUsageType(EPerkUsageType::Passive);
    Kind = UKismetMathLibrary::SelectInt(0, UKismetMathLibrary::SelectInt(1, 2, Perk->IsUsageType(EPerkUsageType::Active)), CallFunc_IsUsageType_ReturnValue);
    return;
}

void DB_Data::ClearTier() {
    UItemUpgrade* CallFunc_Array_Get_Item;
    bool CallFunc_Array_RemoveItem_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, ApplyPart_TierUps.Num());
        if (CallFunc_Less_IntInt_ReturnValue) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            ApplyPart_TierUps.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item);
            if (CallFunc_IsValid_ReturnValue) {
                ApplyPart_TierUps.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                CallFunc_Array_RemoveItem_ReturnValue = ApplyPart_Ups.RemoveItem(CallFunc_Array_Get_Item->SavegameID);
            }
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            return;
        }
    }
}

void DB_Data::LiveTip(TSoftObjectPtr<UObject> Ref, DB_Tip*& Tip) {
    bool CallFunc_IsValid_ReturnValue;
    DB_Tip* K2Node_DynamicCast_AsDB_Tip;
    bool K2Node_DynamicCast_bSuccess;
    LiveTip_Tip = nullptr;
    LiveTip_Obj = UKismetSystemLibrary::Conv_SoftObjectReferenceToObject(Ref);
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(LiveTip_Obj);
    if (CallFunc_IsValid_ReturnValue) {
        K2Node_DynamicCast_AsDB_Tip = Cast<DB_Tip>(LiveTip_Obj);
        K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Tip != nullptr;
        if (K2Node_DynamicCast_bSuccess) {
            LiveTip_Tip = K2Node_DynamicCast_AsDB_Tip;
        }
    }
    Tip = LiveTip_Tip;
    return;
}

void DB_Data::LoadSettings() {
    bool CallFunc_DoesSaveGameExist_ReturnValue;
    DB_Settings* K2Node_DynamicCast_AsDB_Settings;
    DB_Settings* K2Node_DynamicCast_AsDB_Settings_1;
    bool K2Node_DynamicCast_bSuccess;
    bool K2Node_DynamicCast_bSuccess_1;
    CallFunc_DoesSaveGameExist_ReturnValue = UGameplayStatics::DoesSaveGameExist("DwarfBuilds", 0);
    if (CallFunc_DoesSaveGameExist_ReturnValue) {
        K2Node_DynamicCast_AsDB_Settings_1 = Cast<DB_Settings>(UGameplayStatics::LoadGameFromSlot("DwarfBuilds", 0));
        K2Node_DynamicCast_bSuccess_1 = K2Node_DynamicCast_AsDB_Settings_1 != nullptr;
        if (K2Node_DynamicCast_bSuccess_1) {
            Settings = K2Node_DynamicCast_AsDB_Settings_1;
            return;
        }
    }
    K2Node_DynamicCast_AsDB_Settings = Cast<DB_Settings>(UGameplayStatics::CreateSaveGameObject(DB_Settings::StaticClass()));
    K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Settings != nullptr;
    if (K2Node_DynamicCast_bSuccess) {
        Settings = K2Node_DynamicCast_AsDB_Settings;
        return;
    } else {
        return;
    }
}

// Another player's link, when it is the best source of their build: on a client any link the host has filled, on the
// host only one the player's own mod sent (otherwise the host reads the player itself, as 0.1.0 did).
DB_BuildShare_v2* DB_Data::LinkFor(AFSDPlayerState* Player) {
    AFSDPlayerState* Local;
    LocalPS(Local);
    if (!UKismetSystemLibrary::IsValid(Player) || Player == Local) return nullptr;
    bool Server = UKismetSystemLibrary::IsServer(this);
    for (DB_BuildShare_v2* Link : Links) {
        if (UKismetSystemLibrary::IsValid(Link) && Link->Player == Player && UKismetStringLibrary::Len(Link->Code) > 0 &&
            (!Server || Link->FromPlayer))
            return Link;
    }
    return nullptr;
}

// The sharing runs on no schedule (as upstream 0.2.0): HostSync runs at BeginPlay, when a link arrives or gets its
// player's code (LinkChanged), a moment after one of the game's events (ShareCheck binds them: a player joined or
// left, the local player's class changed, the class selection ended, the last game window closed, which is where
// loadouts and perks change), when DB_Select sees your class or loadout change, and after ApplyCode. Each event
// starts a one-shot timer (EventDelay), which folds a burst of events into one call.

// Every link of this world, once at BeginPlay; later ones tell DB_Data when they arrive (DB_BuildShare_v2::Tell).
void DB_Data::ShareScan() {
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(this, DB_BuildShare_v2::StaticClass(), Found);
    Links.Clear();
    for (AActor* Actor : Found) Links.AddUnique((DB_BuildShare_v2*)Actor);
}

// Binds the game's events, again while a source is missing (it may not exist yet at BeginPlay), every HookRetry at
// most HookTries times, and once more when the local player's link arrives. Binding twice is harmless: the engine
// keeps one binding per object and function.
void DB_Data::ShareCheck() {
    bool Missing = false;
    UFSDGameInstance* GI = UGameFunctionLibrary::GetFSDGameInstance(this);
    if (UKismetSystemLibrary::IsValid(GI)) GI->OnCharacterSelectionStop.Add(this, &DB_Data::OnShareEvent);
    UWindowManager* WM = UGameFunctionLibrary::GetWindowManager(this);
    if (UKismetSystemLibrary::IsValid(WM)) {
        WM->OnLastWindowClosed.Add(this, &DB_Data::OnShareEvent);
    } else {
        Missing = true;
    }
    AFSDGameState* State = UGameFunctionLibrary::GetFSDGameState(this);
    if (UKismetSystemLibrary::IsValid(State)) {
        State->OnPlayerJoined.Add(this, &DB_Data::OnSharePlayer);
        State->OnPlayerLeave.Add(this, &DB_Data::OnSharePlayer);
    } else {
        Missing = true;
    }
    AFSDPlayerState* Local;
    LocalPS(Local);
    if (UKismetSystemLibrary::IsValid(Local)) {
        Local->OnSelectedCharacterChanged.Add(this, &DB_Data::OnShareClass);
    } else {
        Missing = true;
    }
    HostSync();
    if (Missing && HookTries < 40) {
        HookTries += 1;
        UKismetSystemLibrary::K2_SetTimer(this, "ShareCheck", 0.5f, false, 0.0f, 0.0f);
    }
}

void DB_Data::OnShareEvent() {
    UKismetSystemLibrary::K2_SetTimer(this, "HostSync", 0.3f, false, 0.0f, 0.0f);
}

void DB_Data::OnSharePlayer(AFSDPlayerState* PlayerState) {
    OnShareEvent();
}

void DB_Data::OnShareClass(TSubclassOf<APlayerCharacter> NewCharacter) {
    OnShareEvent();
}

// The game instance outlives the level.
void DB_Data::ReceiveEndPlay(TEnum<EEndPlayReason> EndPlayReason) {
    UFSDGameInstance* GI = UGameFunctionLibrary::GetFSDGameInstance(this);
    if (UKismetSystemLibrary::IsValid(GI)) GI->OnCharacterSelectionStop.Remove(this, &DB_Data::OnShareEvent);
}

// A link tells this machine that it is here, that its player is known now, or (on the host) that the player's code
// came. The local player's own link: the build goes through it at once, and the events are bound again (the local
// player state is certainly there now). On the host, HostSync follows: who has sent a build decides whether the
// 0.1.0 table is kept up.
void DB_Data::LinkChanged(DB_BuildShare_v2* Link) {
    if (!UKismetSystemLibrary::IsValid(Link)) return;
    Links.AddUnique(Link);
    AFSDPlayerState* Local;
    LocalPS(Local);
    if (UKismetSystemLibrary::IsValid(Local) && Link->Player == Local) {
        SendOwn();
        UKismetSystemLibrary::K2_SetTimer(this, "ShareCheck", 0.3f, false, 0.0f, 0.0f);
    } else if (UKismetSystemLibrary::IsServer(this)) {
        OnShareEvent();
    }
}

// The local player's build into their own link, when it is not there already. The host writes it; a client sends it
// (Server_SetCode) once per code and link, and only through a link it owns: a server RPC needs the owner's
// connection, so while the link does not name its owner yet this is tried again every second, at most 20 times.
void DB_Data::SendOwn() {
    AFSDPlayerState* Local;
    LocalPS(Local);
    if (!UKismetSystemLibrary::IsValid(Local)) return;
    DB_BuildShare_v2* Link = nullptr;
    for (DB_BuildShare_v2* Each : Links)
        if (UKismetSystemLibrary::IsValid(Each) && Each->Player == Local) Link = Each;
    if (Link == nullptr) return;
    FString Own;
    LocalCode(Own);
    if (UKismetStringLibrary::Len(Own) == 0) return;
    if (UKismetSystemLibrary::IsServer(this)) {
        if (Link->Code == Own) return;
        Link->PerksKnown = true;
        Link->FromPlayer = true;
        Link->Code = Own;
        Link->ForceNetUpdate();
        return;
    }
    if (Link->GetOwner() != UGameplayStatics::GetPlayerController(this, 0)) {
        if (OwnerTries < 20) {
            OwnerTries += 1;
            UKismetSystemLibrary::K2_SetTimer(this, "SendOwn", 1.0f, false, 0.0f, 0.0f);
        }
        return;
    }
    if (Own == SentCode && Link == SentLink) return;
    SentCode = Own;
    SentLink = Link;
    OwnerTries = 0;
    Link->Server_SetCode(Own);
}

// A client: SendOwn. The host, with company: one link per player, owned by the player's controller so the player can
// send through it; a link starts empty and stays so for a player without the mod (readers then use what the game
// replicates and the 0.1.0 table, as before). Links of players who left go. The 0.1.0 perk table collects only while
// some other player has sent no build (a player without the mod, or on 0.1.0: the table is what clients read for
// them). Alone: no links and the table rests. Then the host's own build.
void DB_Data::HostSync() {
    if (!UKismetSystemLibrary::IsServer(this)) {
        SendOwn();
        return;
    }
    AFSDPlayerState* Local;
    LocalPS(Local);
    bool Need = false;
    AGameStateBase* State = UGameplayStatics::GetGameState(this);
    if (UKismetSystemLibrary::IsValid(State) && State->PlayerArray.Num() > 1) {
        FTransform Origin = UKismetMathLibrary::MakeTransform(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f),
                                                              FVector(1.0f, 1.0f, 1.0f));
        TArray<APlayerState*> Players = State->PlayerArray;
        for (APlayerState* Each : Players) {
            AFSDPlayerState* PS = Cast<AFSDPlayerState>(Each);
            if (PS == nullptr) continue;
            DB_BuildShare_v2* Link = nullptr;
            for (DB_BuildShare_v2* Other : Links)
                if (UKismetSystemLibrary::IsValid(Other) && Other->Player == PS) Link = Other;
            AActor* Controller = PS->GetOwner();
            if (Link == nullptr) {
                Link = (DB_BuildShare_v2*)UGameplayStatics::BeginDeferredActorSpawnFromClass(
                    this, DB_BuildShare_v2::StaticClass(), Origin, ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
                    Controller);
                Link->Player = PS;
                UGameplayStatics::FinishSpawningActor(Link, Origin);
                Links.AddUnique(Link);
            } else if (UKismetSystemLibrary::IsValid(Controller) && Link->GetOwner() != Controller) {
                Link->SetOwner(Controller);
                Link->ForceNetUpdate();
            }
            if (PS != Local && UKismetStringLibrary::Len(Link->Code) == 0) Need = true;
        }
    }
    TArray<DB_BuildShare_v2*> Keep;
    for (DB_BuildShare_v2* Link : Links) {
        if (!UKismetSystemLibrary::IsValid(Link)) continue;
        if (UKismetSystemLibrary::IsValid(Link->Player)) {
            Keep.Add(Link);
        } else {
            Link->K2_DestroyActor();
        }
    }
    Links = Keep;
    DB_PerkShare_v1* Table = (DB_PerkShare_v1*)UGameplayStatics::GetActorOfClass(this, DB_PerkShare_v1::StaticClass());
    if (UKismetSystemLibrary::IsValid(Table)) {
        if (!Need) {
            UKismetSystemLibrary::K2_ClearTimer(Table, "Collect");
        } else if (!UKismetSystemLibrary::K2_IsTimerActive(Table, "Collect")) {
            UKismetSystemLibrary::K2_SetTimer(Table, "Collect", 3.0f, true, 0.0f, 0.0f);
            Table->Collect();
        }
    }
    SendOwn();
}
