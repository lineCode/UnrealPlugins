// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Containers/UnrealString.h"
#include "Internationalization/Text.h"
#include "FluidMenuRuntime.h"
#include "FluidMenuItemBaseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLUIDMENURUNTIME_API UFluidMenuItemBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnSetData(UFluidMenuBaseUserWidget* ParentMenuWidget_in, FName Data_in,FName ActionID_in);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnSetIsSelected(bool bIsSelected_in, bool bIsBeingInitalized_in);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DoGetActionIDS(FName& ID_out, FName& SubID_out);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnHorizontalInput(int Axis);

public:
	UPROPERTY(BlueprintReadWrite)
		UFluidMenuBaseUserWidget* ParentMenuWidget;
	UPROPERTY(BlueprintReadWrite)
		FName ActionID;
	UPROPERTY(BlueprintReadWrite)
		bool bIsSelected;
	UPROPERTY(BlueprintReadWrite)
		bool bIsBeingInitalized;
};
