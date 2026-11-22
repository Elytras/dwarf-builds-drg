// The Mod Hub interfaces Dwarf Builds implements and calls, as Mod Hub ships them at /Game/_ModHub (each a Blueprint
// interface, IHubMod_C and so on). They are declared here, not built: Mod Hub, or a hub that embeds the same
// interfaces, provides them in game. Without one, DB_Mod never loads DB_Hub, so nothing here is needed.
#pragma once
#include "UeApi/UeMeta.h"
#include "UeApi/Slate.h"

namespace Game { namespace _ModHub {

class IHub;
class IHubContext;
class IHubDataSource;
class IHubMod;
class IHubPageWidget;

// What a hub hands a mod's pages: the hub, the mod and its data source.
class IHubContext {
public:
    UE_CLASS("/Game/_ModHub/IHubContext", "IHubContext_C");
    virtual void WriteLog(FString Title, FString LogContent);
    virtual void SaveDataSource();
    virtual void GetMod(TScriptInterface<IHubMod>& Mod);
    virtual void GetHub(TScriptInterface<IHub>& Hub);
    virtual TScriptInterface<IHubDataSource> GetDataSource();
private:
    struct UeMarkers { static constexpr bool UeInterfaceMeta = true; };
};

// A mod's saved settings, as the hub's own settings pages edit them.
class IHubDataSource {
public:
    UE_CLASS("/Game/_ModHub/IHubDataSource", "IHubDataSource_C");
    virtual void SetFloatValue(FGuid SaveID, float Value);
    virtual void save();
private:
    struct UeMarkers { static constexpr bool UeInterfaceMeta = true; };
};

// The hub: what a registered mod may ask of it.
class IHub {
public:
    UE_CLASS("/Game/_ModHub/IHub", "IHub_C");
    virtual void WriteLog(FString Title, FString LogContent);
    virtual void ShowNotification(FString Title, FString Message, int Duration);
    virtual void SetOpacity(float Opacity);
    virtual void SelectPage(TScriptInterface<IHubPageWidget> HubPage);
    virtual void RegisterKeybinding(TScriptInterface<IHubMod> Mod, FName ActionName, FText DisplayName,
                                    FInputChord DefaultKey);
    virtual void RefreshPages();
    virtual void OpenHub();
    virtual int GetMaxPlayerCount();
    virtual void GetKeybinding(FName ActionName, FInputChord& Key);
    virtual void EnableGameplayFeatures();
    virtual void CloseHub();
private:
    struct UeMarkers { static constexpr bool UeInterfaceMeta = true; };
};

// A mod the hub lists: an actor, which the hub finds by this interface.
class IHubMod {
public:
    UE_CLASS("/Game/_ModHub/IHubMod", "IHubMod_C");
    virtual void OnHubContext(const TScriptInterface<IHubContext>& HubContext);
    virtual void ModRegistered(TScriptInterface<IHub> Hub);
    virtual void ModHubOpened();
    virtual void ModHubClosed();
    virtual void MaxPlayersChanged(int LobbySize);
    virtual void KeybindingChanged(FName ActionName, FInputChord NewKey);
    virtual void GetModPages(TArray<TScriptInterface<IHubPageWidget>>& HubPages);
    virtual void GetModInfo(FText& ModName, FText& ModAuthor, FText& ModVersion);
    virtual void GetDataSource(TScriptInterface<IHubDataSource>& DataSource);
private:
    struct UeMarkers { static constexpr bool UeInterfaceMeta = true; };
};

// One page of a mod in the hub: a widget.
class IHubPageWidget {
public:
    UE_CLASS("/Game/_ModHub/IHubPageWidget", "IHubPageWidget_C");
    virtual void HubPageOpened();
    virtual void HubPageClosed();
    virtual void GetPageInfo(FText& PageName);
    virtual void GetContainerMode(bool& bStretchToContainer);
    virtual void BeginHubPage(const TScriptInterface<IHubContext>& HubContext);
private:
    struct UeMarkers { static constexpr bool UeInterfaceMeta = true; };
};

}}   // namespace Game::_ModHub

using IHub = ::Game::_ModHub::IHub;
using IHubContext = ::Game::_ModHub::IHubContext;
using IHubDataSource = ::Game::_ModHub::IHubDataSource;
using IHubMod = ::Game::_ModHub::IHubMod;
using IHubPageWidget = ::Game::_ModHub::IHubPageWidget;
