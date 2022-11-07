// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "FluidMenuRuntime.h"
#include "FluidMenuBaseUserWidget.generated.h"


struct UFluidMenuBaseUserWidgetPanelData
{
	TArray <UFluidMenuItemBaseUserWidget*> MenuItems;
	int CurrentSelectedMenuItemIndex;
};


UCLASS()
class FLUIDMENURUNTIME_API UFluidMenuBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void DoSetCurrentPanelWidget(UPanelWidget* PanelWidget);
	UFUNCTION(BlueprintCallable)
		void DoClearMenuItems(UPanelWidget* PanelWidget);
	UFUNCTION(BlueprintCallable)
		void DoAddMenuItem(TSubclassOf<UFluidMenuItemBaseUserWidget> MenuItemClass, UPanelWidget* PanelWidget, FName Data, FName ActionID);
	UFUNCTION(BlueprintCallable)
		void DoAddMenuItemMulti(TSubclassOf<UFluidMenuItemBaseUserWidget> MenuItemClass, UPanelWidget* PanelWidget, TArray<FName> Datas, TArray<FName> ActionIDs, bool bClearExistingMenuItems);
	UFUNCTION(BlueprintCallable)
		void DoUpdateSelected(bool bIsBeingInitalized);
	UFUNCTION(BlueprintCallable)
		void DoUpdateSelectedInput(int InputDirection);
	UFUNCTION(BlueprintCallable)
		void DoActionSetInput();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnDoActionSet(FName ID, FName SubID);
	UFUNCTION(BlueprintCallable)
		void OnDoHorizontalInput(int Axis);

private:
	UPanelWidget* CurrentPanelWidget;
	TMap< UPanelWidget*, UFluidMenuBaseUserWidgetPanelData> PanelDatas;
};
