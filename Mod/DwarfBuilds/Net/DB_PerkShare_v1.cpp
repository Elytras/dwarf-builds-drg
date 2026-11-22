// Decompiled from /Game/_DwarfBuilds/Net/DB_PerkShare_v1 by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace Net {

void DB_PerkShare_v1::ReceiveBeginPlay() {
    bool CallFunc_BooleanAND_ReturnValue;
    FTimerHandle CallFunc_K2_SetTimer_ReturnValue;
    CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::BooleanAND(HasAuthority(), UKismetSystemLibrary::IsServer(this));
    if (CallFunc_BooleanAND_ReturnValue) {
        CallFunc_K2_SetTimer_ReturnValue = UKismetSystemLibrary::K2_SetTimer(this, "Collect", 3.0f, true, 0.0f, 0.0f);
        Collect();
        return;
    } else {
        return;
    }
}

void DB_PerkShare_v1::Find(int32 PlayerId, UPlayerCharacterID* Class, TArray<UPerkAsset*>& Perks, TArray<int32>& Ranks, bool& Found) {
    int32 CallFunc_Add_IntInt_ReturnValue_2;
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    UPlayerCharacterID* CallFunc_Array_Get_Item_2;
    int32 CallFunc_Array_Get_Item_3;
    int32 CallFunc_Array_Get_Item_4;
    int32 CallFunc_Array_Length_ReturnValue;
    int32 CallFunc_Array_Length_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    bool CallFunc_BooleanAND_ReturnValue_2;
    bool CallFunc_BooleanAND_ReturnValue_3;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    bool CallFunc_Less_IntInt_ReturnValue_4;
    bool Temp_bool_True_if_break_was_hit_Variable;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Variable;
    Find_Perks.Clear();
    Find_Ranks.Clear();
    Find_Found = false;
    Find_Offset = 0;
    Temp_bool_True_if_break_was_hit_Variable = false;
    Temp_int_Loop_Counter_Variable = 0;
    Temp_int_Array_Index_Variable = 0;
    while (true) {
        CallFunc_BooleanAND_ReturnValue_3 = UKismetMathLibrary::BooleanAND(!Temp_bool_True_if_break_was_hit_Variable, UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, Ids.Num()));
        if (CallFunc_BooleanAND_ReturnValue_3) {
            Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
            CallFunc_Less_IntInt_ReturnValue_4 = UKismetMathLibrary::Less_IntInt(Temp_int_Array_Index_Variable, Counts.Num());
            if (CallFunc_Less_IntInt_ReturnValue_4) {
                Counts.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_3);
                Find_Count = CallFunc_Array_Get_Item_3;
            } else {
                Find_Count = 0;
            }
            CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(Temp_int_Array_Index_Variable, Counts.Num());
            CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Array_Index_Variable, Classes.Num()) && CallFunc_Less_IntInt_ReturnValue_2;
            Ids.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_4);
            CallFunc_BooleanAND_ReturnValue_2 = UKismetMathLibrary::EqualEqual_IntInt(CallFunc_Array_Get_Item_4, PlayerId) && CallFunc_BooleanAND_ReturnValue_1;
            if (CallFunc_BooleanAND_ReturnValue_2) {
                Classes.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_2);
                CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(CallFunc_Array_Get_Item_2, Class);
                if (CallFunc_EqualEqual_ObjectObject_ReturnValue) {
                    Temp_int_Variable = 0;
                    while (true) {
                        CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, UKismetMathLibrary::Subtract_IntInt(Find_Count, 1));
                        if (CallFunc_LessEqual_IntInt_ReturnValue) {
                            CallFunc_Array_Length_ReturnValue = this->Ranks.Num();
                            CallFunc_Array_Length_ReturnValue_1 = this->Perks.Num();
                            CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(Find_Offset, Temp_int_Variable);
                            CallFunc_BooleanAND_ReturnValue = UKismetMathLibrary::Less_IntInt(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Length_ReturnValue_1) && UKismetMathLibrary::Less_IntInt(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Length_ReturnValue);
                            if (CallFunc_BooleanAND_ReturnValue) {
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(Find_Offset, Temp_int_Variable);
                                this->Perks.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item_1);
                                CallFunc_Array_Add_ReturnValue_1 = Find_Perks.Add(CallFunc_Array_Get_Item_1);
                                CallFunc_Add_IntInt_ReturnValue_2 = UKismetMathLibrary::Add_IntInt(Find_Offset, Temp_int_Variable);
                                this->Ranks.Get(CallFunc_Add_IntInt_ReturnValue_2, CallFunc_Array_Get_Item);
                                CallFunc_Array_Add_ReturnValue = Find_Ranks.Add(CallFunc_Array_Get_Item);
                            }
                            Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                        } else {
                            Find_Found = true;
                            Temp_bool_True_if_break_was_hit_Variable = true;
                            goto L_0778;
                        }
                    }
                }
            }
            Find_Offset = UKismetMathLibrary::Add_IntInt(Find_Offset, Find_Count);
          L_0778:
            Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
        } else {
            Perks = Find_Perks;
            Ranks = Find_Ranks;
            Found = Find_Found;
            return;
        }
    }
}

void DB_PerkShare_v1::Collect() {
    int32 CallFunc_Array_Add_ReturnValue;
    int32 CallFunc_Array_Add_ReturnValue_1;
    int32 CallFunc_Array_Add_ReturnValue_2;
    int32 CallFunc_Array_Add_ReturnValue_3;
    int32 CallFunc_Array_Add_ReturnValue_4;
    int32 CallFunc_Array_Add_ReturnValue_5;
    int32 CallFunc_Array_Add_ReturnValue_6;
    FSaveGameStatePerkItem CallFunc_Array_Get_Item;
    UPerkAsset* CallFunc_Array_Get_Item_1;
    APlayerState* CallFunc_Array_Get_Item_2;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    int32 CallFunc_GetCurrentRank_ReturnValue;
    APlayerController* CallFunc_GetPlayerController_ReturnValue;
    bool CallFunc_Greater_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    bool CallFunc_IsValid_ReturnValue_3;
    bool CallFunc_IsValid_ReturnValue_4;
    bool CallFunc_IsValid_ReturnValue_5;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    bool CallFunc_Less_IntInt_ReturnValue_2;
    int32 CallFunc_Subtract_IntInt_ReturnValue;
    AFSDPlayerState* K2Node_DynamicCast_AsFSDPlayer_State;
    bool K2Node_DynamicCast_bSuccess;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Array_Index_Variable_1;
    int32 Temp_int_Array_Index_Variable_2;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Loop_Counter_Variable_1;
    int32 Temp_int_Loop_Counter_Variable_2;
    Collect_State = UGameplayStatics::GetGameState(this);
    CallFunc_IsValid_ReturnValue_5 = UKismetSystemLibrary::IsValid(Collect_State);
    if (CallFunc_IsValid_ReturnValue_5) {
        Collect_Players = Collect_State->PlayerArray;
        CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(Collect_Players.Num(), 1);
        if (CallFunc_Greater_IntInt_ReturnValue) {
            Collect_Ids.Clear();
            Collect_Classes.Clear();
            Collect_Perks.Clear();
            Collect_Ranks.Clear();
            Collect_Counts.Clear();
            Collect_Local = nullptr;
            CallFunc_GetPlayerController_ReturnValue = UGameplayStatics::GetPlayerController(this, 0);
            CallFunc_IsValid_ReturnValue_4 = UKismetSystemLibrary::IsValid(CallFunc_GetPlayerController_ReturnValue);
            if (CallFunc_IsValid_ReturnValue_4) {
                CallFunc_GetPlayerController_ReturnValue = UGameplayStatics::GetPlayerController(this, 0);
                Collect_Local = CallFunc_GetPlayerController_ReturnValue->PlayerState;
            }
            Temp_int_Loop_Counter_Variable_1 = 0;
            Temp_int_Array_Index_Variable_2 = 0;
            while (true) {
                CallFunc_Less_IntInt_ReturnValue_2 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_1, Collect_Players.Num());
                if (CallFunc_Less_IntInt_ReturnValue_2) {
                    Temp_int_Array_Index_Variable_2 = Temp_int_Loop_Counter_Variable_1;
                    Collect_Players.Get(Temp_int_Array_Index_Variable_2, CallFunc_Array_Get_Item_2);
                    K2Node_DynamicCast_AsFSDPlayer_State = Cast<AFSDPlayerState>(CallFunc_Array_Get_Item_2);
                    K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsFSDPlayer_State != nullptr;
                    if (K2Node_DynamicCast_bSuccess) {
                        Collect_PS = K2Node_DynamicCast_AsFSDPlayer_State;
                        Collect_Class = Collect_PS->GetSelectedCharacterID();
                        CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(Collect_Class);
                        if (CallFunc_IsValid_ReturnValue_2) {
                            Collect_Start = Collect_Perks.Num();
                            CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(Collect_PS, Collect_Local);
                            if (CallFunc_EqualEqual_ObjectObject_ReturnValue) {
                                Collect_Own = UPerkFunctionLibrary::GetCharacterEquippedPerks(this, Collect_Class);
                                Temp_int_Loop_Counter_Variable = 0;
                                Temp_int_Array_Index_Variable = 0;
                                while (true) {
                                    CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, Collect_Own.Num());
                                    if (CallFunc_Less_IntInt_ReturnValue) {
                                        Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                                        Collect_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item_1);
                                        if (CallFunc_IsValid_ReturnValue_1) {
                                            Collect_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                            CallFunc_Array_Add_ReturnValue_5 = Collect_Perks.Add(CallFunc_Array_Get_Item_1);
                                            Collect_Own.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_1);
                                            CallFunc_GetCurrentRank_ReturnValue = CallFunc_Array_Get_Item_1->GetCurrentRank(this);
                                            CallFunc_Array_Add_ReturnValue_4 = Collect_Ranks.Add(CallFunc_GetCurrentRank_ReturnValue);
                                        }
                                        Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
                                    } else {
                                        break;
                                    }
                                }
                            } else {
                                CallFunc_IsValid_ReturnValue_3 = UKismetSystemLibrary::IsValid(Collect_PS->SaveGameStateComponent);
                                if (CallFunc_IsValid_ReturnValue_3) {
                                    Collect_Items = Collect_PS->SaveGameStateComponent->EquippedPerks;
                                    Temp_int_Loop_Counter_Variable_2 = 0;
                                    Temp_int_Array_Index_Variable_1 = 0;
                                    while (true) {
                                        CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable_2, Collect_Items.Num());
                                        if (CallFunc_Less_IntInt_ReturnValue_1) {
                                            Temp_int_Array_Index_Variable_1 = Temp_int_Loop_Counter_Variable_2;
                                            Collect_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(CallFunc_Array_Get_Item.Perk);
                                            if (CallFunc_IsValid_ReturnValue) {
                                                Collect_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                                CallFunc_Array_Add_ReturnValue_2 = Collect_Perks.Add(CallFunc_Array_Get_Item.Perk);
                                                Collect_Items.Get(Temp_int_Array_Index_Variable_1, CallFunc_Array_Get_Item);
                                                CallFunc_Array_Add_ReturnValue_3 = Collect_Ranks.Add(UKismetMathLibrary::Conv_ByteToInt(CallFunc_Array_Get_Item.Rank));
                                            }
                                            Temp_int_Loop_Counter_Variable_2 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_2, 1);
                                        } else {
                                            break;
                                        }
                                    }
                                }
                            }
                            CallFunc_Array_Add_ReturnValue_6 = Collect_Ids.Add(Collect_PS->playerID);
                            CallFunc_Array_Add_ReturnValue_1 = Collect_Classes.Add(Collect_Class);
                            CallFunc_Subtract_IntInt_ReturnValue = UKismetMathLibrary::Subtract_IntInt(Collect_Perks.Num(), Collect_Start);
                            CallFunc_Array_Add_ReturnValue = Collect_Counts.Add(CallFunc_Subtract_IntInt_ReturnValue);
                        }
                    }
                    Temp_int_Loop_Counter_Variable_1 = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable_1, 1);
                } else {
                    FlushNetDormancy();
                    Ids = Collect_Ids;
                    UNetPushModelHelpers::MarkPropertyDirtyFromRepIndex(this, 10, "Ids");
                    FlushNetDormancy();
                    Classes = Collect_Classes;
                    UNetPushModelHelpers::MarkPropertyDirtyFromRepIndex(this, 11, "Classes");
                    FlushNetDormancy();
                    Perks = Collect_Perks;
                    UNetPushModelHelpers::MarkPropertyDirtyFromRepIndex(this, 12, "Perks");
                    FlushNetDormancy();
                    Ranks = Collect_Ranks;
                    UNetPushModelHelpers::MarkPropertyDirtyFromRepIndex(this, 13, "Ranks");
                    FlushNetDormancy();
                    Counts = Collect_Counts;
                    UNetPushModelHelpers::MarkPropertyDirtyFromRepIndex(this, 14, "Counts");
                    return;
                }
            }
        } else {
            return;
        }
    } else {
        return;
    }
}

}   // namespace Net
