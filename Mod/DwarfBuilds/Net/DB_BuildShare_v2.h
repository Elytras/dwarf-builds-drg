// One player's build, as every client sees it. A host running this build spawns one per player (DB_Data::HostSync),
// owned by that player's controller, so the player's own copy of the mod can send its build (Server_SetCode). A
// player without the mod has an empty link. A new class next to DB_PerkShare_v1, which stays as 0.1.0 shipped it: a
// client without this class (0.1.0, or no mod) drops the actor's channel and plays on. The replicated layout is
// frozen (Mod/netcontract.json) and matches upstream 0.2.0's BPForge class.
#pragma once
#include "../DwarfBuilds.h"
namespace Net {
class DB_BuildShare_v2 : public AActor {
public:
    DB_BuildShare_v2() {
        bAlwaysRelevant = true;
        bReplicates = true;
        // A link changes a few times a session and every change is pushed (ForceNetUpdate).
        NetUpdateFrequency = 0.5f;
    }
    // RepNotify: on a client the player state may come after the link.
    UE_REPLICATED_USING(AFSDPlayerState*, Player, OnRep_Player);
    // A DB1 build code; its perk part counts only when PerksKnown.
    UE_REPLICATED(FString, Code);
    UE_REPLICATED(bool, PerksKnown);
    // The code came from the player's own mod, not from what the host can see of them.
    UE_REPLICATED(bool, FromPlayer);

    UE_SERVER UE_RELIABLE void Server_SetCode(const FString& NewCode);
    void OnRep_Player();
    void Tell();
protected:
    void ReceiveBeginPlay();
};
}   // namespace Net
