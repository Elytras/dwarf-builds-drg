// Dwarf Builds' page in Elytras Mod Manager's hub: every player's build, and the build code tools, laid out in the
// hub's pane in EMM's own widgets and palette (BasicButtons, the hub's font and fields through IHubLook), with no
// window, frame or title bar of its own. DB_Hub hands it to the hub as the mod's one page.
//
// TEAM shows a card per player (DB_Column, filtered by class past four players), as many to a row as the pane's width
// holds, from the top, scrolled down past that. BUILD CODE takes a pasted code, previews it in a column and takes it into a
// loadout (DB_TakeDialog over the page), or copies or publishes your own. Browse builds opens the website in the
// Steam overlay. The page refreshes once a second while it is shown.
#pragma once
#include "../DwarfBuilds.h"
namespace UI {
class DB_Window : public UUserWidget, public ::IHubPageWidget, public ::IHubLook {
public:
    UE_CLASS("/Game/_DwarfBuilds/UI/DB_Window", "DB_Window_C");
    UE_WIDGET(UOverlay, Root);
    UE_WIDGET(UVerticalBox, Body);
    UE_WIDGET(UHorizontalBox, TopRow);
    UE_WIDGET(UBasicButton, TabTeam);
    UE_WIDGET(UBasicButton, TabCode);
    UE_WIDGET(UBasicButton, BrowseBtn);
    UE_WIDGET(USpacer, TopGap);
    UE_WIDGET(UTextBlock, CountText);
    UE_WIDGET(UHorizontalBox, Filters);
    UE_WIDGET(UBasicButton, Filter0);
    UE_WIDGET(UBasicButton, Filter1);
    UE_WIDGET(UBasicButton, Filter2);
    UE_WIDGET(UBasicButton, Filter3);
    UE_WIDGET(UBasicButton, Filter4);
    UE_WIDGET(UWidgetSwitcher, Pages);
    UE_WIDGET(UScrollBox, TeamScroll);
    UE_WIDGET(UWrapBox, TeamWrap);
    UE_WIDGET(UHorizontalBox, CodePage);
    UE_WIDGET(UVerticalBox, CodeCol);
    UE_WIDGET(UTextBlock, CodeHint);
    UE_WIDGET(UEditableTextBox, CodeBox);
    UE_WIDGET(UTextBlock, CodeStatus);
    UE_WIDGET(UHorizontalBox, CodeButtons);
    UE_WIDGET(UBasicButton, PasteBtn);
    UE_WIDGET(UBasicButton, TakeCodeBtn);
    UE_WIDGET(UHorizontalBox, MineButtons);
    UE_WIDGET(UBasicButton, CopyMineBtn);
    UE_WIDGET(UBasicButton, PublishBtn);
    UE_WIDGET(UBasicButton, SiteBtn);
    UE_WIDGET(UTextBlock, CopyNote);
    UE_WIDGET(UScaleBox, PreviewFit);
    UE_WIDGET(DB_Column, Preview);
    UE_WIDGET(DB_TakeDialog, Take);
    DB_Data* Data;
    Widget_C* Look;                     // the hub's look, from ApplyLook; columns made later take it too
    TArray<AFSDPlayerState*> Players;
    TArray<int32> PlayerClass;          // each player's class: 0 none, 1 driller, 2 engineer, 3 gunner, 4 scout
    TArray<int32> Shown;                // the players the filter lets through, by index into Players
    TArray<int32> ClassCounts;          // players per class, 0 holding everyone
    TArray<DB_Column*> Columns;         // made as more players show, kept for the level
    int32 Filter;
    int32 Tab;
    bool HubShown;                      // the page is on show in a hub: it refreshes only then
    bool Inited;
    bool TakeOpen;
    FString CodeText;
    bool CodeValid;
    DB_Window() {
        bIsFocusable = true;
        UOverlaySlot* Body_Slot = Cast<UOverlaySlot>(Root->AddChild(Body));
        Body_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        Body_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;

        // The top row: the two tabs, the website, and how many players there are.
        UVerticalBoxSlot* TopRow_Slot = Cast<UVerticalBoxSlot>(Body->AddChild(TopRow));
        TopRow_Slot->Padding.Bottom = 8.0f;
        TopRow->AddChild(TabTeam);
        TabTeam->Category = EStyleCategory::PageTab;
        TabTeam->TextString = "Team";
        TabTeam->HintString = "Every player's weapons, upgrades, overclocks, equipment and perks";
        UHorizontalBoxSlot* TabCode_Slot = Cast<UHorizontalBoxSlot>(TopRow->AddChild(TabCode));
        TabCode_Slot->Padding.Left = 6.0f;
        TabCode->Category = EStyleCategory::PageTab;
        TabCode->TextString = "Build code";
        TabCode->HintString = "Paste a code, preview it and take it; copy or publish your own";
        UHorizontalBoxSlot* BrowseBtn_Slot = Cast<UHorizontalBoxSlot>(TopRow->AddChild(BrowseBtn));
        BrowseBtn_Slot->Padding.Left = 6.0f;
        BrowseBtn->Category = EStyleCategory::Action;
        BrowseBtn->TextString = "Browse builds";
        BrowseBtn->HintString = "dwarfbuilds.pages.dev in the Steam overlay";
        UHorizontalBoxSlot* TopGap_Slot = Cast<UHorizontalBoxSlot>(TopRow->AddChild(TopGap));
        TopGap_Slot->Size.SizeRule = ESlateSizeRule::Fill;
        TopGap_Slot->Size.Value = 1.0f;
        UHorizontalBoxSlot* CountText_Slot = Cast<UHorizontalBoxSlot>(TopRow->AddChild(CountText));
        CountText_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
        CountText->Font.Size = 16;

        // The class filters, past four players.
        UVerticalBoxSlot* Filters_Slot = Cast<UVerticalBoxSlot>(Body->AddChild(Filters));
        Filters_Slot->Padding.Bottom = 8.0f;
        Filters->Visibility = ESlateVisibility::Collapsed;
        Filters->AddChild(Filter0);
        UHorizontalBoxSlot* Filter1_Slot = Cast<UHorizontalBoxSlot>(Filters->AddChild(Filter1));
        Filter1_Slot->Padding.Left = 6.0f;
        UHorizontalBoxSlot* Filter2_Slot = Cast<UHorizontalBoxSlot>(Filters->AddChild(Filter2));
        Filter2_Slot->Padding.Left = 6.0f;
        UHorizontalBoxSlot* Filter3_Slot = Cast<UHorizontalBoxSlot>(Filters->AddChild(Filter3));
        Filter3_Slot->Padding.Left = 6.0f;
        UHorizontalBoxSlot* Filter4_Slot = Cast<UHorizontalBoxSlot>(Filters->AddChild(Filter4));
        Filter4_Slot->Padding.Left = 6.0f;
        Filter0->Category = EStyleCategory::Action;
        Filter1->Category = EStyleCategory::Action;
        Filter2->Category = EStyleCategory::Action;
        Filter3->Category = EStyleCategory::Action;
        Filter4->Category = EStyleCategory::Action;

        UVerticalBoxSlot* Pages_Slot = Cast<UVerticalBoxSlot>(Body->AddChild(Pages));
        Pages_Slot->Size.SizeRule = ESlateSizeRule::Fill;
        Pages_Slot->Size.Value = 1.0f;

        // TEAM: the cards in rows as wide as the pane, scrolled down when they do not fit.
        UWidgetSwitcherSlot* TeamScroll_Slot = Cast<UWidgetSwitcherSlot>(Pages->AddChild(TeamScroll));
        TeamScroll_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        TeamScroll_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;
        TeamScroll->AddChild(TeamWrap);
        TeamWrap->InnerSlotPadding = FVector2D(12.0f, 12.0f);

        // BUILD CODE: the code and its buttons on the left, its preview on the right.
        UWidgetSwitcherSlot* CodePage_Slot = Cast<UWidgetSwitcherSlot>(Pages->AddChild(CodePage));
        CodePage_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        CodePage_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;
        UHorizontalBoxSlot* CodeCol_Slot = Cast<UHorizontalBoxSlot>(CodePage->AddChild(CodeCol));
        CodeCol_Slot->Size.SizeRule = ESlateSizeRule::Fill;
        CodeCol_Slot->Size.Value = 1.0f;
        CodeCol_Slot->Padding.Right = 16.0f;
        UVerticalBoxSlot* CodeHint_Slot = Cast<UVerticalBoxSlot>(CodeCol->AddChild(CodeHint));
        CodeHint_Slot->Padding.Bottom = 6.0f;
        CodeHint->Text = "Paste a build code (it starts with DB1:) to preview it and take it into a loadout.";
        CodeHint->Font.Size = 16;
        CodeHint->AutoWrapText = true;
        UVerticalBoxSlot* CodeBox_Slot = Cast<UVerticalBoxSlot>(CodeCol->AddChild(CodeBox));
        CodeBox_Slot->Padding.Bottom = 6.0f;
        CodeBox->HintText = "DB1:...";
        UVerticalBoxSlot* CodeStatus_Slot = Cast<UVerticalBoxSlot>(CodeCol->AddChild(CodeStatus));
        CodeStatus_Slot->Padding.Bottom = 8.0f;
        CodeStatus->Font.Size = 16;
        UVerticalBoxSlot* CodeButtons_Slot = Cast<UVerticalBoxSlot>(CodeCol->AddChild(CodeButtons));
        CodeButtons_Slot->Padding.Bottom = 20.0f;
        CodeButtons->AddChild(PasteBtn);
        PasteBtn->Category = EStyleCategory::Action;
        PasteBtn->TextString = "Paste from clipboard";
        UHorizontalBoxSlot* TakeCodeBtn_Slot = Cast<UHorizontalBoxSlot>(CodeButtons->AddChild(TakeCodeBtn));
        TakeCodeBtn_Slot->Padding.Left = 6.0f;
        TakeCodeBtn->Category = EStyleCategory::Action;
        TakeCodeBtn->TextString = "Take this build";
        TakeCodeBtn->HintString = "Save the pasted build into one of your loadouts";
        UVerticalBoxSlot* MineButtons_Slot = Cast<UVerticalBoxSlot>(CodeCol->AddChild(MineButtons));
        MineButtons_Slot->Padding.Bottom = 6.0f;
        MineButtons->AddChild(CopyMineBtn);
        CopyMineBtn->Category = EStyleCategory::Action;
        CopyMineBtn->TextString = "Copy my code";
        CopyMineBtn->HintString = "Your current build's code, to the clipboard";
        UHorizontalBoxSlot* PublishBtn_Slot = Cast<UHorizontalBoxSlot>(MineButtons->AddChild(PublishBtn));
        PublishBtn_Slot->Padding.Left = 6.0f;
        PublishBtn->Category = EStyleCategory::Action;
        PublishBtn->TextString = "Publish my build";
        PublishBtn->HintString = "Your current build on the website, in the Steam overlay";
        UHorizontalBoxSlot* SiteBtn_Slot = Cast<UHorizontalBoxSlot>(MineButtons->AddChild(SiteBtn));
        SiteBtn_Slot->Padding.Left = 6.0f;
        SiteBtn->Category = EStyleCategory::Action;
        SiteBtn->TextString = "Open in browser";
        SiteBtn->HintString = "dwarfbuilds.pages.dev in your own browser";
        CodeCol->AddChild(CopyNote);
        CopyNote->Font.Size = 16;
        UHorizontalBoxSlot* PreviewFit_Slot = Cast<UHorizontalBoxSlot>(CodePage->AddChild(PreviewFit));
        PreviewFit_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Right;
        PreviewFit->Stretch = EStretch::ScaleToFit;
        PreviewFit->StretchDirection = EStretchDirection::DownOnly;
        PreviewFit->AddChild(Preview);

        // The take dialog covers the page while it is open.
        UOverlaySlot* Take_Slot = Cast<UOverlaySlot>(Root->AddChild(Take));
        Take_Slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Fill;
        Take_Slot->VerticalAlignment = EVerticalAlignment::VAlign_Fill;
        Take->Visibility = ESlateVisibility::Collapsed;

        TabTeam->OnClicked.Add(this, &DB_Window::OnButton);
        TabCode->OnClicked.Add(this, &DB_Window::OnButton);
        BrowseBtn->OnClicked.Add(this, &DB_Window::OnButton);
        Filter0->OnClicked.Add(this, &DB_Window::OnButton);
        Filter1->OnClicked.Add(this, &DB_Window::OnButton);
        Filter2->OnClicked.Add(this, &DB_Window::OnButton);
        Filter3->OnClicked.Add(this, &DB_Window::OnButton);
        Filter4->OnClicked.Add(this, &DB_Window::OnButton);
        PasteBtn->OnClicked.Add(this, &DB_Window::OnButton);
        TakeCodeBtn->OnClicked.Add(this, &DB_Window::OnButton);
        CopyMineBtn->OnClicked.Add(this, &DB_Window::OnButton);
        PublishBtn->OnClicked.Add(this, &DB_Window::OnButton);
        SiteBtn->OnClicked.Add(this, &DB_Window::OnButton);
        CodeBox->OnTextChanged.Add(this, &DB_Window::OnCodeChanged);
    }
    // IHubPageWidget, IHubLook
    void HubPageOpened();
    void HubPageClosed();
    void GetPageInfo(FText& PageName);
    void GetContainerMode(bool& bStretchToContainer);
    void ApplyLook(Widget_C* Service);

    void Open();
    void OpenTake(FString NewCode, FString Title);
    void CloseTake();
    void Refresh();
    void TimerRefresh();
    void Collect();
    void ClassIndex(AFSDPlayerState* Player, int32& Index);
    void ShowPage();
    void UpdateHeader();
    DB_Column* ColumnAt(int32 Index);
    void StyleColumn(DB_Column* Column);
    void SetFilter(int32 NewFilter);
    void SetTab(int32 NewTab);
    void Stop();
    void HideAllTips();
    void IsOpenNow(bool& bOpen);
    void InitOnce();
    void UpdateCode(FString NewText);
    void PasteCode();
    void CopyMine();
    void OpenSite(FString URL);
    void PublishMine();
    void OnButton(UBasicButton* Button);
    void OnCodeChanged(FText Text);
    void OnOverlayClosed(FString LastURL);
    void OnRemovedFromFocusPath(FFocusEvent InFocusEvent);
    void OnFocusLost(FFocusEvent InFocusEvent);
    void Destruct();
    FEventReply OnKeyUp(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    FEventReply OnKeyDown(FGeometry MyGeometry, FKeyEvent InKeyEvent);
    FEventReply OnMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent);
};
}   // namespace UI
