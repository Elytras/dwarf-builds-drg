// Decompiled from /Game/_DwarfBuilds/Net/DB_PerkShare_v1 by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#pragma once
#include "../DwarfBuilds.h"
namespace Net {
class DB_PerkShare_v1 : public AActor {
public:
    DB_PerkShare_v1() {
        bAlwaysRelevant = true;
        bReplicates = true;
        NetUpdateFrequency = 2.0f;
    }
    UE_REPLICATED(TArray<int32>, Ids);
    UE_REPLICATED(TArray<UPlayerCharacterID*>, Classes);
    UE_REPLICATED(TArray<UPerkAsset*>, Perks);
    UE_REPLICATED(TArray<int32>, Ranks);
    UE_REPLICATED(TArray<int32>, Counts);
    TArray<UPerkAsset*> Find_Perks;
    TArray<int32> Find_Ranks;
    bool Find_Found;
    int32 Find_Offset;
    int32 Find_Count;
    TArray<int32> Collect_Ids;
    TArray<UPlayerCharacterID*> Collect_Classes;
    TArray<UPerkAsset*> Collect_Perks;
    TArray<int32> Collect_Ranks;
    TArray<int32> Collect_Counts;
    AGameStateBase* Collect_State;
    TArray<APlayerState*> Collect_Players;
    APlayerState* Collect_Local;
    AFSDPlayerState* Collect_PS;
    UPlayerCharacterID* Collect_Class;
    int32 Collect_Start;
    TArray<UPerkAsset*> Collect_Own;
    TArray<FSaveGameStatePerkItem> Collect_Items;
    void Find(int32 PlayerId, UPlayerCharacterID* Class, TArray<UPerkAsset*>& Perks, TArray<int32>& Ranks, bool& Found);
    void Collect();
protected:
    void ReceiveBeginPlay();
};
}   // namespace Net
