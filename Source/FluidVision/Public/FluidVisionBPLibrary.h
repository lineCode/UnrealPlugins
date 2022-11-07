// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/PlayerController.h"
#include "FluidVisionBPLibrary.generated.h"

class APostProcessVolume;

UCLASS()
class UFluidVisionBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable)
		static void InitVision();
	UFUNCTION(BlueprintCallable)
		static void CleanupVision();
	UFUNCTION(BlueprintCallable)
		static void ToggleVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass);
	UFUNCTION(BlueprintCallable)
		static void SetVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass);
	UFUNCTION(BlueprintCallable)
		static void RemoveVisionEffect(TSubclassOf<AFluidVisionEffectActor> VisionEffectClass);

private:

	static void CreatePostProcessVolume();

	static TMap<TSubclassOf<AFluidVisionEffectActor>, AFluidVisionEffectActor*> EffectActors;
	static AFluidVisionEffectActor* CurrentEffectActor;
	static APostProcessVolume* PostProcessVolume;
};

UCLASS(Blueprintable, BlueprintType, abstract)
class AFluidVisionEffectActor : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnEffectStart(APostProcessVolume* PostProcessVolume);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnEffectEnd(APostProcessVolume* PostProcessVolume);

	UPROPERTY(BlueprintReadOnly)
		bool bIsActive;

};