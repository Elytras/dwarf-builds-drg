// Decompiled from /Game/_DwarfBuilds/UI/DB_Select by AssetGen/tools/bpdecomp.py; edit freely, it is source now.
#include "../DwarfBuilds.h"

namespace UI {

// Runs only while the overlay is drawn, that is while the selection screen shows. It refreshes the overlay once a
// second (Update), refits it when the screen's size changes, and redraws your column at once when your class or
// loadout changes. A gap since the last frame means the screen was hidden and is back: that counts as just shown.
void DB_Select::Tick(FGeometry MyGeometry, float InDeltaTime) {
    float Now = UGameplayStatics::GetRealTimeSeconds(this);
    if (Now - LastTick > 0.3f) {
        WasUp = false;
        NextUpdate = 0.0f;
    }
    LastTick = Now;
    if (Now >= NextUpdate) {
        NextUpdate = Now + 1.0f;
        bool Up;
        bool Became;
        Update(Up, Became);
    }
    FVector2D Size = USlateBlueprintLibrary::GetLocalSize(MyGeometry);
    if (!UKismetMathLibrary::NearlyEqual_FloatFloat(Size.X, ViewW, 0.5f) ||
        !UKismetMathLibrary::NearlyEqual_FloatFloat(Size.Y, ViewH, 0.5f)) {
        ViewW = Size.X;
        ViewH = Size.Y;
        FitScale(TeamCount);
    }
    AFSDPlayerState* PS = Cast<AFSDPlayerState>(GetOwningPlayer()->PlayerState);
    if (PS == nullptr) return;
    UFSDSaveGame* Save = UGameFunctionLibrary::GetFSDSaveGame(this);
    if (!UKismetSystemLibrary::IsValid(Save)) return;
    UPlayerCharacterID* Class = PS->GetSelectedCharacterID();
    int32 Loadout = Save->GetSelectedLoadoutIndex(Class);
    if (Class == OwnClass && Loadout == OwnLoadout) return;
    OwnClass = Class;
    OwnLoadout = Loadout;
    Own->ShowPlayer(PS);
}

void DB_Select::AttachTo(UUserWidget* NewScreen, UCanvasPanel* NewCanvas, DB_Data* NewData, int32 Layer) {
    UCanvasPanelSlot* CallFunc_AddChildToCanvas_ReturnValue;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    bool CallFunc_IsInViewport_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    TArray<DB_Column*> K2Node_MakeArray_Array;
    FAnchors K2Node_MakeStruct_Anchors;
    FMargin K2Node_MakeStruct_Margin;
    Screen = NewScreen;
    Canvas = NewCanvas;
    Data = NewData;
    Team0->Data = Data;
    Team1->Data = Data;
    Team2->Data = Data;
    Own->Data = Data;
    if (!Inited) {
        K2Node_MakeArray_Array = {Team0, Team1, Team2};
        TeamCols = K2Node_MakeArray_Array;
        Team0->SetCompact(true);
        Team1->SetCompact(true);
        Team2->SetCompact(true);
        Own->SetCompact(true);
        Inited = true;
    }
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(NewCanvas);
    if (CallFunc_IsValid_ReturnValue) {
        CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(GetParent(), NewCanvas);
        if (!CallFunc_EqualEqual_ObjectObject_ReturnValue) {
            CallFunc_AddChildToCanvas_ReturnValue = NewCanvas->AddChildToCanvas(this);
            CallFunc_AddChildToCanvas_ReturnValue->SetZOrder(9999);
            K2Node_MakeStruct_Anchors.Minimum = FVector2D(0.0f, 0.0f);
            K2Node_MakeStruct_Anchors.Maximum = FVector2D(1.0f, 1.0f);
            CallFunc_AddChildToCanvas_ReturnValue->SetAnchors(K2Node_MakeStruct_Anchors);
            K2Node_MakeStruct_Margin.Left = 0.0f;
            K2Node_MakeStruct_Margin.Top = 0.0f;
            K2Node_MakeStruct_Margin.Right = 0.0f;
            K2Node_MakeStruct_Margin.Bottom = 0.0f;
            CallFunc_AddChildToCanvas_ReturnValue->SetOffsets(K2Node_MakeStruct_Margin);
        }
    } else {
        CallFunc_IsInViewport_ReturnValue = IsInViewport();
        if (!CallFunc_IsInViewport_ReturnValue) {
            RemoveFromParent();
            AddToViewport(Layer);
        }
    }
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    WasUp = false;
    Refresh();
    return;
}

void DB_Select::IsAttached(bool& Up) {
    bool CallFunc_BooleanOR_ReturnValue;
    bool CallFunc_EqualEqual_ObjectObject_ReturnValue;
    UPanelWidget* CallFunc_GetParent_ReturnValue_1;
    bool CallFunc_IsInViewport_ReturnValue;
    bool CallFunc_IsInViewport_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(Screen);
    if (CallFunc_IsValid_ReturnValue_2) {
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Canvas);
        if (CallFunc_IsValid_ReturnValue) {
            CallFunc_EqualEqual_ObjectObject_ReturnValue = UKismetMathLibrary::EqualEqual_ObjectObject(GetParent(), Canvas);
            CallFunc_GetParent_ReturnValue_1 = Screen->GetParent();
            CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_GetParent_ReturnValue_1);
            CallFunc_IsInViewport_ReturnValue_1 = Screen->IsInViewport();
            CallFunc_BooleanOR_ReturnValue = CallFunc_IsInViewport_ReturnValue_1 || CallFunc_IsValid_ReturnValue_1;
            UpTmp = CallFunc_BooleanOR_ReturnValue && CallFunc_EqualEqual_ObjectObject_ReturnValue;
        } else {
            CallFunc_IsInViewport_ReturnValue = IsInViewport();
            CallFunc_GetParent_ReturnValue_1 = Screen->GetParent();
            CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(CallFunc_GetParent_ReturnValue_1);
            CallFunc_IsInViewport_ReturnValue_1 = Screen->IsInViewport();
            CallFunc_BooleanOR_ReturnValue = CallFunc_IsInViewport_ReturnValue_1 || CallFunc_IsValid_ReturnValue_1;
            UpTmp = CallFunc_BooleanOR_ReturnValue && CallFunc_IsInViewport_ReturnValue;
        }
    } else {
        UpTmp = false;
    }
    Up = UpTmp;
    return;
}

void DB_Select::Update(bool& Up, bool& Became) {
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    bool CallFunc_IsValid_ReturnValue_2;
    ESlateVisibility K2Node_Select_Default;
    BecTmp = false;
    UpTmp = false;
    CallFunc_IsValid_ReturnValue_2 = UKismetSystemLibrary::IsValid(Screen);
    if (CallFunc_IsValid_ReturnValue_2) {
        UpTmp = Screen->IsVisible();
        CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Canvas);
        if (!CallFunc_IsValid_ReturnValue_1) {
            SetVisibility(UpTmp == false ? ESlateVisibility::Collapsed : UpTmp == true ? ESlateVisibility::SelfHitTestInvisible : K2Node_Select_Default);
        }
        if (UpTmp) {
            if (WasUp) {
                Refresh();
                HideDesc();
            } else {
                BecTmp = true;
                WasUp = true;
                Refresh();
                FindDesc();
                HideDesc();
            }
        } else if (WasUp) {
            CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
            if (CallFunc_IsValid_ReturnValue) {
                Data->HideTips();
            }
            WasUp = false;
        }
    }
    Up = UpTmp;
    Became = BecTmp;
    return;
}

void DB_Select::Detach() {
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsValid_ReturnValue_1;
    CallFunc_IsValid_ReturnValue_1 = UKismetSystemLibrary::IsValid(Screen);
    if (CallFunc_IsValid_ReturnValue_1) {
        CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
        if (CallFunc_IsValid_ReturnValue) {
            Data->HideTips();
        }
    }
    RemoveFromParent();
    Screen = nullptr;
    Canvas = nullptr;
    Desc = nullptr;
    WasUp = false;
    return;
}

void DB_Select::OnStop() {
    bool CallFunc_IsValid_ReturnValue;
    WasUp = false;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
    if (CallFunc_IsValid_ReturnValue) {
        Data->HideTips();
        return;
    } else {
        return;
    }
}

void DB_Select::Refresh() {
    int32 CallFunc_Array_Add_ReturnValue;
    AFSDPlayerState* CallFunc_Array_Get_Item;
    DB_Column* CallFunc_Array_Get_Item_1;
    AFSDPlayerState* CallFunc_Array_Get_Item_2;
    int32 CallFunc_Array_Length_ReturnValue;
    FText CallFunc_Format_ReturnValue;
    APlayerController* CallFunc_GetOwningPlayer_ReturnValue;
    TArray<AFSDPlayerState*> CallFunc_GetPlayers_Players;
    bool CallFunc_Greater_IntInt_ReturnValue;
    bool CallFunc_IsValid_ReturnValue;
    bool CallFunc_IsVisible_ReturnValue;
    bool CallFunc_LessEqual_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue_1;
    int32 CallFunc_Min_ReturnValue;
    bool CallFunc_NotEqual_ObjectObject_ReturnValue;
    int32 CallFunc_Subtract_IntInt_ReturnValue;
    AFSDPlayerState* K2Node_DynamicCast_AsFSDPlayer_State;
    bool K2Node_DynamicCast_bSuccess;
    TArray<FFormatArgumentData> K2Node_MakeArray_Array;
    FFormatArgumentData K2Node_MakeStruct_FormatArgumentData;
    ESlateVisibility K2Node_Select_Default;
    ESlateVisibility K2Node_Select_Default_1;
    ESlateVisibility Temp_byte_Variable;
    ESlateVisibility Temp_byte_Variable_1;
    ESlateVisibility Temp_byte_Variable_2;
    ESlateVisibility Temp_byte_Variable_3;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    int32 Temp_int_Variable;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Data);
    if (CallFunc_IsValid_ReturnValue) {
        Data->GetPlayers(CallFunc_GetPlayers_Players);
        Mates.Clear();
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue_1 = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, CallFunc_GetPlayers_Players.Num());
            if (CallFunc_Less_IntInt_ReturnValue_1) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                CallFunc_GetOwningPlayer_ReturnValue = GetOwningPlayer();
                CallFunc_GetPlayers_Players.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_2);
                CallFunc_NotEqual_ObjectObject_ReturnValue = UKismetMathLibrary::NotEqual_ObjectObject(CallFunc_Array_Get_Item_2, CallFunc_GetOwningPlayer_ReturnValue->PlayerState);
                if (CallFunc_NotEqual_ObjectObject_ReturnValue) {
                    CallFunc_GetPlayers_Players.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item_2);
                    CallFunc_Array_Add_ReturnValue = Mates.Add(CallFunc_Array_Get_Item_2);
                }
                Temp_int_Loop_Counter_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Loop_Counter_Variable, 1);
            } else {
                CallFunc_Array_Length_ReturnValue = Mates.Num();
                CallFunc_Min_ReturnValue = UKismetMathLibrary::Min(CallFunc_Array_Length_ReturnValue, 3);
                TeamCount = CallFunc_Min_ReturnValue;
                CallFunc_Array_Length_ReturnValue = Mates.Num();
                CallFunc_Min_ReturnValue = UKismetMathLibrary::Min(CallFunc_Array_Length_ReturnValue, 3);
                FitScale(CallFunc_Min_ReturnValue);
                Temp_int_Variable = 0;
                while (true) {
                    CallFunc_LessEqual_IntInt_ReturnValue = UKismetMathLibrary::LessEqual_IntInt(Temp_int_Variable, 2);
                    if (CallFunc_LessEqual_IntInt_ReturnValue) {
                        CallFunc_Array_Length_ReturnValue = Mates.Num();
                        CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Variable, CallFunc_Array_Length_ReturnValue);
                        if (CallFunc_Less_IntInt_ReturnValue) {
                            Mates.Get(Temp_int_Variable, CallFunc_Array_Get_Item);
                            TeamCols.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                            CallFunc_Array_Get_Item_1->ShowPlayer(CallFunc_Array_Get_Item);
                            TeamCols.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                            CallFunc_Array_Get_Item_1->SetVisibility(ESlateVisibility::Visible);
                        } else {
                            TeamCols.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                            CallFunc_IsVisible_ReturnValue = CallFunc_Array_Get_Item_1->IsVisible();
                            if (CallFunc_IsVisible_ReturnValue) {
                                TeamCols.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                                CallFunc_Array_Get_Item_1->Clear();
                                TeamCols.Get(Temp_int_Variable, CallFunc_Array_Get_Item_1);
                                CallFunc_Array_Get_Item_1->SetVisibility(ESlateVisibility::Collapsed);
                            }
                        }
                        Temp_int_Variable = UKismetMathLibrary::Add_IntInt(Temp_int_Variable, 1);
                    } else {
                        Temp_byte_Variable_2 = ESlateVisibility::HitTestInvisible;
                        Temp_byte_Variable_3 = ESlateVisibility::Collapsed;
                        CallFunc_Array_Length_ReturnValue = Mates.Num();
                        TeamHead->SetVisibility(UKismetMathLibrary::Greater_IntInt(CallFunc_Array_Length_ReturnValue, 0) == false ? Temp_byte_Variable_3 : UKismetMathLibrary::Greater_IntInt(CallFunc_Array_Length_ReturnValue, 0) == true ? Temp_byte_Variable_2 : K2Node_Select_Default_1);
                        Temp_byte_Variable = ESlateVisibility::HitTestInvisible;
                        Temp_byte_Variable_1 = ESlateVisibility::Collapsed;
                        CallFunc_Array_Length_ReturnValue = Mates.Num();
                        CallFunc_Subtract_IntInt_ReturnValue = UKismetMathLibrary::Subtract_IntInt(CallFunc_Array_Length_ReturnValue, 3);
                        CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(CallFunc_Subtract_IntInt_ReturnValue, 0);
                        MoreBox->SetVisibility(CallFunc_Greater_IntInt_ReturnValue == false ? Temp_byte_Variable_1 : CallFunc_Greater_IntInt_ReturnValue == true ? Temp_byte_Variable : K2Node_Select_Default);
                        CallFunc_Array_Length_ReturnValue = Mates.Num();
                        CallFunc_Subtract_IntInt_ReturnValue = UKismetMathLibrary::Subtract_IntInt(CallFunc_Array_Length_ReturnValue, 3);
                        CallFunc_Greater_IntInt_ReturnValue = UKismetMathLibrary::Greater_IntInt(CallFunc_Subtract_IntInt_ReturnValue, 0);
                        if (CallFunc_Greater_IntInt_ReturnValue) {
                            CallFunc_Array_Length_ReturnValue = Mates.Num();
                            CallFunc_Subtract_IntInt_ReturnValue = UKismetMathLibrary::Subtract_IntInt(CallFunc_Array_Length_ReturnValue, 3);
                            K2Node_MakeStruct_FormatArgumentData.ArgumentName = "N";
                            K2Node_MakeStruct_FormatArgumentData.ArgumentValueType = EFormatArgumentType::Int;
                            K2Node_MakeStruct_FormatArgumentData.ArgumentValue = FText();
                            K2Node_MakeStruct_FormatArgumentData.ArgumentValueInt = CallFunc_Subtract_IntInt_ReturnValue;
                            K2Node_MakeStruct_FormatArgumentData.ArgumentValueFloat = 0.0f;
                            K2Node_MakeStruct_FormatArgumentData.ArgumentValueGender = ETextGender::Masculine;
                            K2Node_MakeArray_Array = {K2Node_MakeStruct_FormatArgumentData};
                            CallFunc_Format_ReturnValue = UKismetTextLibrary::Format("+{N} MORE", K2Node_MakeArray_Array);
                            MoreText->SetText(CallFunc_Format_ReturnValue);
                        }
                        CallFunc_GetOwningPlayer_ReturnValue = GetOwningPlayer();
                        K2Node_DynamicCast_AsFSDPlayer_State = Cast<AFSDPlayerState>(CallFunc_GetOwningPlayer_ReturnValue->PlayerState);
                        K2Node_DynamicCast_bSuccess = K2Node_DynamicCast_AsFSDPlayer_State != nullptr;
                        if (K2Node_DynamicCast_bSuccess) {
                            Own->ShowPlayer(K2Node_DynamicCast_AsFSDPlayer_State);
                            Own->SetVisibility(ESlateVisibility::Visible);
                            return;
                        } else {
                            Own->Clear();
                            Own->SetVisibility(ESlateVisibility::Collapsed);
                            return;
                        }
                    }
                }
            }
        }
    } else {
        return;
    }
}

void DB_Select::FitScale(int32 Count) {
    bool CallFunc_Greater_FloatFloat_ReturnValue;
    CallFunc_Greater_FloatFloat_ReturnValue = UKismetMathLibrary::Greater_FloatFloat(ViewH, 400.0f);
    if (CallFunc_Greater_FloatFloat_ReturnValue) {
        TeamFit->SetUserSpecifiedScale(UKismetMathLibrary::FClamp(UKismetMathLibrary::FMin(UKismetMathLibrary::Divide_FloatFloat(UKismetMathLibrary::Subtract_FloatFloat(UKismetMathLibrary::Multiply_FloatFloat(ViewW, 0.5f), 294.0f), UKismetMathLibrary::Conv_IntToFloat(UKismetMathLibrary::Subtract_IntInt(UKismetMathLibrary::Multiply_IntInt(UKismetMathLibrary::Max(Count, 1), 426), 8))), UKismetMathLibrary::Divide_FloatFloat(UKismetMathLibrary::Subtract_FloatFloat(ViewH, 440.0f), 940.0f)), 0.45f, 0.8f));
        OwnFit->SetUserSpecifiedScale(UKismetMathLibrary::FClamp(UKismetMathLibrary::Divide_FloatFloat(UKismetMathLibrary::Subtract_FloatFloat(ViewH, 472.0f), 940.0f), 0.45f, 0.8f));
        return;
    } else {
        return;
    }
}

void DB_Select::FindDesc() {
    UUserWidget* CallFunc_Array_Get_Item;
    bool CallFunc_BooleanAND_ReturnValue;
    bool CallFunc_BooleanAND_ReturnValue_1;
    TSoftClassPtr<UClass> CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue;
    bool CallFunc_EqualEqual_StrStr_ReturnValue;
    TArray<UUserWidget*> CallFunc_GetAllWidgetsOfClass_FoundWidgets;
    UObject* CallFunc_GetOuterObject_ReturnValue;
    UObject* CallFunc_GetOuterObject_ReturnValue_1;
    UObject* CallFunc_GetOuterObject_ReturnValue_2;
    bool CallFunc_IsValidClass_ReturnValue;
    bool CallFunc_Less_IntInt_ReturnValue;
    FSoftClassPath CallFunc_MakeSoftClassPath_ReturnValue;
    TSubclassOf<UUserWidget> K2Node_ClassDynamicCast_AsUser_Widget;
    bool K2Node_ClassDynamicCast_bSuccess;
    int32 Temp_int_Array_Index_Variable;
    int32 Temp_int_Loop_Counter_Variable;
    Desc = nullptr;
    CallFunc_MakeSoftClassPath_ReturnValue = UKismetSystemLibrary::MakeSoftClassPath("/Game/UI/Art/WidgetParts/Basic_Menu_ColorBar.Basic_Menu_ColorBar_C");
    CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue = UKismetSystemLibrary::Conv_SoftClassPathToSoftClassRef(CallFunc_MakeSoftClassPath_ReturnValue);
    K2Node_ClassDynamicCast_AsUser_Widget = (UKismetMathLibrary::ClassIsChildOf((UClass*)UKismetSystemLibrary::LoadClassAsset_Blocking(CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue), UUserWidget::StaticClass()) ? TSubclassOf<UUserWidget>(UKismetSystemLibrary::LoadClassAsset_Blocking(CallFunc_Conv_SoftClassPathToSoftClassRef_ReturnValue)) : TSubclassOf<UUserWidget>(nullptr));
    K2Node_ClassDynamicCast_bSuccess = K2Node_ClassDynamicCast_AsUser_Widget != nullptr;
    if (K2Node_ClassDynamicCast_bSuccess) {
        DescClass = K2Node_ClassDynamicCast_AsUser_Widget;
    }
    CallFunc_IsValidClass_ReturnValue = UKismetSystemLibrary::IsValidClass(DescClass);
    if (CallFunc_IsValidClass_ReturnValue) {
        CallFunc_GetAllWidgetsOfClass_FoundWidgets = {};
        UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, CallFunc_GetAllWidgetsOfClass_FoundWidgets, DescClass, false);
        Temp_int_Loop_Counter_Variable = 0;
        Temp_int_Array_Index_Variable = 0;
        while (true) {
            CallFunc_Less_IntInt_ReturnValue = UKismetMathLibrary::Less_IntInt(Temp_int_Loop_Counter_Variable, CallFunc_GetAllWidgetsOfClass_FoundWidgets.Num());
            if (CallFunc_Less_IntInt_ReturnValue) {
                Temp_int_Array_Index_Variable = Temp_int_Loop_Counter_Variable;
                CallFunc_GetAllWidgetsOfClass_FoundWidgets.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                CallFunc_EqualEqual_StrStr_ReturnValue = UKismetStringLibrary::EqualEqual_StrStr(UKismetSystemLibrary::GetObjectName(CallFunc_Array_Get_Item), "Basic_Menu_ColorBar_1");
                CallFunc_GetOuterObject_ReturnValue = UKismetSystemLibrary::GetOuterObject(CallFunc_Array_Get_Item);
                CallFunc_GetOuterObject_ReturnValue_1 = UKismetSystemLibrary::GetOuterObject(CallFunc_GetOuterObject_ReturnValue);
                CallFunc_BooleanAND_ReturnValue = UKismetStringLibrary::EqualEqual_StrStr(UKismetSystemLibrary::GetObjectName(CallFunc_GetOuterObject_ReturnValue_1), "DescriptionWindow") && CallFunc_EqualEqual_StrStr_ReturnValue;
                CallFunc_GetOuterObject_ReturnValue_2 = UKismetSystemLibrary::GetOuterObject(CallFunc_GetOuterObject_ReturnValue_1);
                CallFunc_BooleanAND_ReturnValue_1 = UKismetMathLibrary::EqualEqual_ObjectObject(UKismetSystemLibrary::GetOuterObject(CallFunc_GetOuterObject_ReturnValue_2), Screen) && CallFunc_BooleanAND_ReturnValue;
                if (CallFunc_BooleanAND_ReturnValue_1) {
                    CallFunc_GetAllWidgetsOfClass_FoundWidgets.Get(Temp_int_Array_Index_Variable, CallFunc_Array_Get_Item);
                    Desc = CallFunc_Array_Get_Item->GetParent()->GetParent()->GetParent();
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

void DB_Select::HideDesc() {
    bool CallFunc_IsValid_ReturnValue;
    CallFunc_IsValid_ReturnValue = UKismetSystemLibrary::IsValid(Desc);
    if (CallFunc_IsValid_ReturnValue) {
        Desc->SetVisibility(ESlateVisibility::Hidden);
        return;
    } else {
        return;
    }
}

FEventReply DB_Select::OnPreviewMouseButtonDown(FGeometry MyGeometry, const FPointerEvent& MouseEvent) {
    FEventReply CallFunc_Handled_ReturnValue;
    FEventReply CallFunc_SetUserFocus_ReturnValue;
    FEventReply CallFunc_Unhandled_ReturnValue;
    FEventReply K2Node_Select_Default;
    CallFunc_Unhandled_ReturnValue = UWidgetBlueprintLibrary::Unhandled();
    CallFunc_Handled_ReturnValue = UWidgetBlueprintLibrary::Handled();
    CallFunc_SetUserFocus_ReturnValue = UWidgetBlueprintLibrary::SetUserFocus(CallFunc_Handled_ReturnValue, Screen, false);
    return UKismetSystemLibrary::IsValid(Screen) == false ? CallFunc_Unhandled_ReturnValue : UKismetSystemLibrary::IsValid(Screen) == true ? CallFunc_SetUserFocus_ReturnValue : K2Node_Select_Default;
}

}   // namespace UI
