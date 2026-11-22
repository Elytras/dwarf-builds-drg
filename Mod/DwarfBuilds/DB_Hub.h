// Decompiled from /Game/_DwarfBuilds/DB_Hub by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#pragma once
#include "DwarfBuilds.h"
class DB_Hub : public AActor, public ::IHubMod {
public:
    UE_CLASS("/Game/_DwarfBuilds/DB_Hub", "DB_Hub_C");
    DB_HubPage* Page;
    void ModHubOpened();
    void ModHubClosed();
    void ModRegistered(TScriptInterface<::IHub> Hub);
    void GetModInfo(FText& ModName, FText& ModAuthor, FText& ModVersion);
    void GetModPages(TArray<TScriptInterface<::IHubPageWidget>>& HubPages);
};
