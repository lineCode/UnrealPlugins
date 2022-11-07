// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidMenuBaseUserWidget.h"

void UFluidMenuBaseUserWidget::DoSetCurrentPanelWidget(UPanelWidget* PanelWidget)
{
	if (!PanelDatas.Contains(PanelWidget))
		PanelDatas.Add(PanelWidget);
	CurrentPanelWidget = PanelWidget;
	CurrentPanelWidget->SetVisibility(ESlateVisibility::Visible);
}

void UFluidMenuBaseUserWidget::DoClearMenuItems(UPanelWidget* PanelWidget)
{
	if (PanelWidget == nullptr)
		return;

	PanelWidget->ClearChildren();
	PanelDatas.Empty();
}


void UFluidMenuBaseUserWidget::DoAddMenuItem(TSubclassOf<UFluidMenuItemBaseUserWidget> MenuItemClass, UPanelWidget* PanelWidget, FName Data, FName ActionID)
{
	if (PanelWidget == nullptr)
		return;

	auto N = CreateWidget<UFluidMenuItemBaseUserWidget>(this, MenuItemClass);

	DoSetCurrentPanelWidget(PanelWidget);
	PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex = 0;
	PanelDatas[CurrentPanelWidget].MenuItems.Add(N);
	N->OnSetData(this, Data,ActionID);
	PanelWidget->AddChild(N);
	DoUpdateSelected(true);
}

void UFluidMenuBaseUserWidget::DoAddMenuItemMulti(TSubclassOf<UFluidMenuItemBaseUserWidget> MenuItemClass, UPanelWidget* PanelWidget, TArray<FName> Datas, TArray<FName> ActionIDs, bool bClearExistingMenuItems)
{
	if (Datas.Num() != ActionIDs.Num())
		return;

	if (bClearExistingMenuItems)
		DoClearMenuItems(PanelWidget);

	for (int i = 0; i < Datas.Num(); i++)
		DoAddMenuItem(MenuItemClass, PanelWidget, Datas[i],ActionIDs[i]);
}


void UFluidMenuBaseUserWidget::DoUpdateSelected(bool bIsBeingInitalized)
{
	if (CurrentPanelWidget == nullptr)
		return;

	for (int i = 0; i < PanelDatas[CurrentPanelWidget].MenuItems.Num(); i++)
		PanelDatas[CurrentPanelWidget].MenuItems[i]->OnSetIsSelected(i == PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex, bIsBeingInitalized);
}

void UFluidMenuBaseUserWidget::DoUpdateSelectedInput(int InputDirection)
{
	if (CurrentPanelWidget == nullptr)
		return;

	int PreviousMenuItemSelectedIndex = PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex;
	PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex += InputDirection;
	PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex = FMath::Clamp(PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex, 0, PanelDatas[CurrentPanelWidget].MenuItems.Num() - 1);

	if (PreviousMenuItemSelectedIndex != PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex)
		DoUpdateSelected(false);
}

void UFluidMenuBaseUserWidget::DoActionSetInput()
{
	if (CurrentPanelWidget == nullptr)
		return;

	FName ID = "";
	FName SubID = "";

	if (PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex <= PanelDatas[CurrentPanelWidget].MenuItems.Num())
		PanelDatas[CurrentPanelWidget].MenuItems[PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex]->DoGetActionIDS(ID, SubID);

	OnDoActionSet(ID, SubID);
}

void UFluidMenuBaseUserWidget::OnDoActionSet_Implementation(FName ID, FName SubID)
{
}

void UFluidMenuBaseUserWidget::OnDoHorizontalInput(int Axis)
{
	if (PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex <= PanelDatas[CurrentPanelWidget].MenuItems.Num())
		PanelDatas[CurrentPanelWidget].MenuItems[PanelDatas[CurrentPanelWidget].CurrentSelectedMenuItemIndex]->OnHorizontalInput(Axis);
}