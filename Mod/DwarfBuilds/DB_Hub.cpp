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
    bool CallFunc_IsValid_ReturnValue;
    DB_HubPage* K2Node_DynamicCast_AsDB_Hub_Page;
    bool K2Node_DynamicCast_bSuccess;
    TArray<TScriptInterface<::IHubPageWidget>> K2Node_MakeArray_Array;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Page);
    if (!CallFunc_IsValid_ReturnValue) {
        K2Node_DynamicCast_AsDB_Hub_Page = Cast<DB_HubPage>(UWidgetBlueprintLibrary::Create(this, DB_HubPage::StaticClass(), nullptr));
        K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsDB_Hub_Page != nullptr;
        if (K2Node_DynamicCast_bSuccess) {
            Page = K2Node_DynamicCast_AsDB_Hub_Page;
        }
    }
    K2Node_MakeArray_Array = {Page};
    HubPages = K2Node_MakeArray_Array;
    return;
}

