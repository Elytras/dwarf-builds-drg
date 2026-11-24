// Decompiled from /Game/_DwarfBuilds/DB_Hub by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "DwarfBuilds.h"


void DB_Hub::ModHubOpened() {
    return;
}

void DB_Hub::ModHubClosed() {
    return;
}

void DB_Hub::ModRegistered(TScriptInterface<::IHub> Hub) {
    return;
}

void DB_Hub::GetModInfo(FText& ModName, FText& ModAuthor, FText& ModVersion) {
    ModName = "Dwarf Builds";
    ModAuthor = "rodionmasalov";
    ModVersion = "0.2.0+assetgen";
    return;
}

void DB_Hub::GetModPages(TArray<TScriptInterface<::IHubPageWidget>>& HubPages) {
    if (!UKismetSystemLibrary::IsValid(Page))
        Page = Cast<DB_Window>(UWidgetBlueprintLibrary::Create(this, DB_Window::StaticClass(), nullptr));
    HubPages = {Page};
}
