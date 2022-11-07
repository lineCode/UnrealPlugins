#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WrapBox.h"
#include "Kismet/GameplayStatics.h"
#include "FluidInventoryRuntime.h"
#include "FluidInventoryUI.generated.h"

//Fluid Inventory Grid Item Widget
UCLASS(Blueprintable, BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryItemSetItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnBuild();
	UFUNCTION(BlueprintImplementableEvent)
		void OnBuildEmpty();
	UFUNCTION(BlueprintImplementableEvent)
		void OnUpdate();
	UFUNCTION(BlueprintImplementableEvent)
	    void GetWidthHeight(float& Width_Out,float& Height_Out);

	UPROPERTY(BlueprintReadOnly)
		class UFluidInventoryItemSetWidget* ItemSetWidget;
	UPROPERTY(BlueprintReadOnly)
		int Item;
	UPROPERTY(BlueprintReadOnly)
		int IndexInItemSet;
};

//Fluid Inventory Action Menu Item Widget
UCLASS(Blueprintable, BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryActionMenuItemWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnBuild(FName Text);

	UPROPERTY(BlueprintReadOnly)
		class UFluidInventoryActionMenuWidget* ActionMenuWidget;
	UPROPERTY(BlueprintReadOnly)
		int IndexActionMenu;
};

//Fluid Inventory Grid Widget
UCLASS(Blueprintable, BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryItemSetWidget : public UUserWidget
{
   GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void Setup(UFluidInventorySubsystem* FluidInventorySubsystem, UWrapBox* WrapBoxToUse, TSubclassOf<UFluidInventoryItemSetItemWidget> InventoryItemSetItemWidgetClass);
	UFUNCTION(BlueprintCallable)
		void Update(int LocalPlayerIndex,bool bCaptureKeyboardWadsKeys,bool bCaptureKeyboardDirectionKeys, bool bCaptureGamePadLeftStick, bool bCaptureGamePadDPad, bool bIsInputEnabled);
	UFUNCTION(BlueprintCallable)
		void Build(const int Inventory);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetHoveredItem(int& Item_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		void GetHoveredItemPrevious(int& Item_Out);

	UPROPERTY(BlueprintReadWrite)
		TArray<FName> ItemTags;
	UPROPERTY(BlueprintReadOnly)
		FVector2D CursorPosition;
	UPROPERTY(BlueprintReadOnly)
		FVector2D CursorPositionPrevious;
	UPROPERTY(BlueprintReadOnly)
		FVector2D CursorPositionMax;
	UPROPERTY(BlueprintReadOnly)
		int CursorPosition1D;
	UPROPERTY(BlueprintReadOnly)
		int CursorPosition1DPrevious;

	UFluidInventorySubsystem* FluidInventorySubsystemInternal;
	TSubclassOf<UFluidInventoryItemSetItemWidget> InventoryItemSetItemWidgetClassInternal;
	TArray<UFluidInventoryItemSetItemWidget*> ItemSetItemWidgets;
	UWrapBox* WrapBox;
	TArray<int> Items;
};

//Fluid Inventory Action Menu Widget
UCLASS(Blueprintable, BlueprintType)
class FLUIDINVENTORYRUNTIME_API UFluidInventoryActionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void Setup(UFluidInventorySubsystem* FluidInventorySubsystem, UWrapBox* WrapBoxToUse, TSubclassOf<UFluidInventoryActionMenuItemWidget> ActionMenuItemWidgetClass);
	UFUNCTION(BlueprintCallable)
		void Update(int LocalPlayerIndex,bool bCaptureKeyboardWadsKeys,bool bCaptureKeyboardDirectionKeys, bool bCaptureGamePadLeftStick, bool bCaptureGamePadDPad, bool bIsInputEnabled);
	UFUNCTION(BlueprintCallable)
		void Build(const int Item);

	UPROPERTY(BlueprintReadOnly)
		int CursorPosition;
	UPROPERTY(BlueprintReadOnly)
		int CursorPositionPrevious;
	UPROPERTY(BlueprintReadOnly)
		int CursorPositionMax;
		
	UFluidInventorySubsystem* FluidInventorySubsystemInternal;
	TSubclassOf<UFluidInventoryActionMenuItemWidget> ActionMenuItemWidgetClassInternal;
	TArray<UFluidInventoryActionMenuItemWidget*> ActionMenuItemWidgets;
	UWrapBox* WrapBox;
	TArray<int> Items;
};