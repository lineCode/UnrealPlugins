// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LatentActionManager.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FluidToolboxInput.generated.h"

UENUM(Blueprintable)
enum EFluidToolboxInputInputOutput
{
	Success			UMETA(DisplayName = "Success"),
	Failure			UMETA(DisplayName = "Failure"),
};

UENUM(Blueprintable)
enum EFluidToolboxInputInputPressedHeldOutput
{
	Pressed			UMETA(DisplayName = "Pressed"),
	Held			UMETA(DisplayName = "Held"),
	HeldRepeat		UMETA(DisplayName = "HeldRepeat"),
	FailureBoth		UMETA(DisplayName = "Failure"),
};

USTRUCT(BlueprintType)
struct FFluidToolboxInputSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		float SensitivityX;
	UPROPERTY(EditDefaultsOnly)
		float SensitivityY;
	UPROPERTY(EditDefaultsOnly)
		bool bIsInvertedX;
	UPROPERTY(EditDefaultsOnly)
		bool bIsInvertedY;
};

USTRUCT(BlueprintType)
struct FLUIDTOOLBOXRUNTIME_API FFluidToolboxInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		FKey KeyboardKey;
	UPROPERTY(EditDefaultsOnly)
		FKey GamePadKey;
};

USTRUCT(BlueprintType)
struct FLUIDTOOLBOXRUNTIME_API FFluidToolboxInputAxis
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureKeyboardWadsKeys;
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureKeyboardDirectionKeys;
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureMouse;
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureGamePadLeftStick;
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureGamePadRightStick;
	UPROPERTY(EditDefaultsOnly)
		bool bCaptureGamePadDPad;
};

// Fluid Toolbox Input Profile
USTRUCT(BlueprintType)
struct FFluidToolboxInputActionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidToolboxInputAction> Actions;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidToolboxInputAxis> Axis;
	UPROPERTY(EditDefaultsOnly)
		FFluidToolboxInputSettings MouseSettings;
	UPROPERTY(EditDefaultsOnly)
		FFluidToolboxInputSettings GamePadJoystickLeftSettings;
	UPROPERTY(EditDefaultsOnly)
		FFluidToolboxInputSettings GamePadJoystickRightSettings;
};

// Fluid Toolbox Input Profiles
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInputProfiles : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
		TMap<FName, FFluidToolboxInputActionProfile> Profiles;
};

// Fluid Toolbox Input Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInputBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	//Actions
	UFUNCTION(BlueprintCallable)
		static void SetCurrentInputProfile(int LocalPlayerIndex, UFluidToolboxInputProfiles* InputProfileAsset, FName ProfileToUseByName);
	UFUNCTION(BlueprintCallable)
		static void GetInputAction(int LocalPlayerIndex, FName ActionName, FFluidToolboxInputAction& InputAction_Out);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputActionDown(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome,bool& bIsDown_Out);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputActionPressed(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputActionReleased(int LocalPlayerIndex, FName ActionName, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputActionPressedHeld(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome);

	//Axis
	UFUNCTION(BlueprintCallable)
		static void GetAxisInput(int LocalPlayerIndex, FName AxisName, float& AxisX_Out, float& AxisY_Out);

	// Keys | Buttons
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputKeyDown(int LocalPlayerIndex,FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputKeyPressed(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputKeyReleased(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		static void GetIsInputKeyPressedHeld(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome);
};

// Fluid Toolbox Input Subsystem
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInputSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	//Begin | End
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//Actions
	UFUNCTION(BlueprintCallable)
		void SetCurrentInputProfile(int LocalPlayerIndex, UFluidToolboxInputProfiles* InputProfileAsset, FName ProfileToUseByName);
	UFUNCTION(BlueprintCallable)
		void GetInputAction(int LocalPlayerIndex, FName ActionName, FFluidToolboxInputAction& InputAction_Out);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputActionDown(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputActionPressed(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputActionReleased(int LocalPlayerIndex, FName ActionName, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputActionPressedHeld(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome);
	
	//Axis
	UFUNCTION(BlueprintCallable)
		void GetAxisInput(int LocalPlayerIndex, FName AxisName, float& AxisX_Out, float& AxisY_Out);
	
	// Keys | Buttons
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputKeyDown(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputKeyPressed(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputKeyReleased(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome);
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = "Outcome"))
		void GetIsInputKeyPressedHeld(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome);
	void AddInputKeyPairToProcess(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey);

public:
	TMap<FKey, float> InputHeldTimes;
	TMap<FKey, bool> InputHeldStates;
	TMap<FKey, bool> InputDidRepeats;
	UObject* WorldContextObject;
	TArray<UFluidToolboxInputProfiles*> CurrentInputProfileAssets;
	TArray<FName> CurrentInputProfileNames;
};
