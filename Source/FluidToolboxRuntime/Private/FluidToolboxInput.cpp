// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidToolboxInput.h"

float HeldTimeMinA = 0.0001f;
float HeldTimeMaxA = 0.2f;
float HeldTimeMinB = 0.201f;
float HeldTimeMaxB = 999.0f;

//////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Blueprint Library : Actions

void UFluidToolboxInputBPLibrary::SetCurrentInputProfile(int LocalPlayerIndex, UFluidToolboxInputProfiles* InputProfileAsset, FName ProfileToUseByName)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->SetCurrentInputProfile(LocalPlayerIndex, InputProfileAsset, ProfileToUseByName);
}

void UFluidToolboxInputBPLibrary::GetInputAction(int LocalPlayerIndex, FName ActionName, FFluidToolboxInputAction& InputAction_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetInputAction(LocalPlayerIndex, ActionName, InputAction_Out);
}

void UFluidToolboxInputBPLibrary::GetIsInputActionDown(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome, bool& bIsDown_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputActionDown(LocalPlayerIndex, ActionName, Outcome);
	bIsDown_Out = Outcome.GetValue() == 0;
}

void UFluidToolboxInputBPLibrary::GetIsInputActionPressed(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputActionPressed(LocalPlayerIndex, ActionName, Outcome);
}

void UFluidToolboxInputBPLibrary::GetIsInputActionReleased(int LocalPlayerIndex, FName ActionName, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputActionReleased(LocalPlayerIndex, ActionName,HeldTimeMin,HeldTimeMax, bAllowRepeats, Outcome);
}

void UFluidToolboxInputBPLibrary::GetIsInputActionPressedHeld(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputActionPressedHeld(LocalPlayerIndex, ActionName, Outcome);
}

//////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Blueprint Library : Axis

void UFluidToolboxInputBPLibrary::GetAxisInput(int LocalPlayerIndex, FName AxisName, float& AxisX_Out, float& AxisY_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetAxisInput(LocalPlayerIndex,AxisName,AxisX_Out,AxisY_Out);
}


//////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Blueprint Library : Keys | Buttons

void UFluidToolboxInputBPLibrary::GetIsInputKeyDown(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputKeyDown(LocalPlayerIndex, KeyboardKey, GamePadKey, Outcome);
}

void UFluidToolboxInputBPLibrary::GetIsInputKeyPressed(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputKeyPressed(LocalPlayerIndex, KeyboardKey, GamePadKey, Outcome);
}

void UFluidToolboxInputBPLibrary::GetIsInputKeyReleased(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputKeyReleased(LocalPlayerIndex, KeyboardKey, GamePadKey, HeldTimeMin, HeldTimeMax, bAllowRepeats, Outcome);
}

void UFluidToolboxInputBPLibrary::GetIsInputKeyPressedHeld(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxInputSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInputSubsystem>();
	Subsystem->GetIsInputKeyPressedHeld(LocalPlayerIndex, KeyboardKey,GamePadKey, Outcome);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Subsystem : Begin | End

void UFluidToolboxInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	for (int i = 0; i < 8; i++)
	{
		CurrentInputProfileAssets.Add(nullptr);
		CurrentInputProfileNames.Add("");
	}
}

void UFluidToolboxInputSubsystem::Deinitialize()
{
	InputHeldTimes.Empty();
	InputHeldStates;
	InputDidRepeats;
	CurrentInputProfileAssets.Empty();
	CurrentInputProfileNames.Empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Subsystem : Actions

void UFluidToolboxInputSubsystem::SetCurrentInputProfile(int LocalPlayerIndex, UFluidToolboxInputProfiles* InputProfileAsset, FName ProfileToUseByName)
{
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex,0, 7);
	CurrentInputProfileAssets[LocalPlayerIndex] = InputProfileAsset;
	CurrentInputProfileNames[LocalPlayerIndex] = ProfileToUseByName;
}

void UFluidToolboxInputSubsystem::GetInputAction(int LocalPlayerIndex, FName ActionName, FFluidToolboxInputAction& InputAction_Out)
{
	InputAction_Out = FFluidToolboxInputAction();
	if (CurrentInputProfileAssets[LocalPlayerIndex] == nullptr || !CurrentInputProfileAssets[LocalPlayerIndex]->Profiles.Contains(CurrentInputProfileNames[LocalPlayerIndex]))
		return;

	auto InputActions = CurrentInputProfileAssets[LocalPlayerIndex]->Profiles[CurrentInputProfileNames[LocalPlayerIndex]].Actions;
	if (InputActions.Contains(ActionName))
		InputAction_Out = InputActions[ActionName];
}

void UFluidToolboxInputSubsystem::GetIsInputActionDown(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	FFluidToolboxInputAction InputAction;
	GetInputAction(LocalPlayerIndex, ActionName, InputAction);

	Outcome = EFluidToolboxInputInputOutput::Failure;
	GetIsInputKeyDown(LocalPlayerIndex,InputAction.KeyboardKey, InputAction.GamePadKey, Outcome);
}

void UFluidToolboxInputSubsystem::GetIsInputActionPressed(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	FFluidToolboxInputAction InputAction;
	GetInputAction(LocalPlayerIndex, ActionName, InputAction);

	Outcome = EFluidToolboxInputInputOutput::Failure;
	GetIsInputKeyPressed(LocalPlayerIndex,InputAction.KeyboardKey, InputAction.GamePadKey, Outcome);
}

void UFluidToolboxInputSubsystem::GetIsInputActionReleased(int LocalPlayerIndex, FName ActionName, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	FFluidToolboxInputAction InputAction;
	GetInputAction(LocalPlayerIndex, ActionName, InputAction);

	Outcome = EFluidToolboxInputInputOutput::Failure;
	GetIsInputKeyReleased(LocalPlayerIndex,InputAction.KeyboardKey, InputAction.GamePadKey, HeldTimeMin, HeldTimeMax, bAllowRepeats, Outcome);
}

void UFluidToolboxInputSubsystem::GetIsInputActionPressedHeld(int LocalPlayerIndex, FName ActionName, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome)
{
	FFluidToolboxInputAction InputAction;
	GetInputAction(LocalPlayerIndex, ActionName, InputAction);

	Outcome = EFluidToolboxInputInputPressedHeldOutput::FailureBoth;
	GetIsInputKeyPressedHeld(LocalPlayerIndex,InputAction.KeyboardKey, InputAction.GamePadKey, Outcome);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Subsystem : Axis Input

void UFluidToolboxInputSubsystem::GetAxisInput(int LocalPlayerIndex, FName AxisName, float& AxisX_Out, float& AxisY_Out)
{
	//Set Default Outs
	AxisX_Out = 0;
	AxisY_Out = 0;

	//Get Input Axis
	FFluidToolboxInputAxis* InputAxis;
	{
		if (CurrentInputProfileAssets[LocalPlayerIndex] == nullptr || !CurrentInputProfileAssets[LocalPlayerIndex]->Profiles.Contains(CurrentInputProfileNames[LocalPlayerIndex]))
			return;

		auto InputActions = CurrentInputProfileAssets[LocalPlayerIndex]->Profiles[CurrentInputProfileNames[LocalPlayerIndex]].Axis;
		if (!InputActions.Contains(AxisName))
			return;
	
		InputAxis = &InputActions[AxisName];
	}
	
	//Get Player Controller
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
	if (PlayerController == nullptr)
		return;
	

	//WADS
	if (InputAxis->bCaptureKeyboardWadsKeys)
	{
		if (PlayerController->IsInputKeyDown(EKeys::W)) AxisY_Out = 1;
		if (PlayerController->IsInputKeyDown(EKeys::S)) AxisY_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::A)) AxisX_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::D)) AxisX_Out = 1;
		if (AxisX_Out != 0 || AxisY_Out != 0)
			return;
	}
	
	//Arrow
	if (InputAxis->bCaptureKeyboardDirectionKeys)
	{
		if (PlayerController->IsInputKeyDown(EKeys::Up)) AxisY_Out = 1;
		if (PlayerController->IsInputKeyDown(EKeys::Down)) AxisY_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::Left)) AxisX_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::Right)) AxisX_Out = 1;
		if (AxisX_Out != 0 || AxisY_Out != 0)
			return;
	}

	//Mouse
	if (InputAxis->bCaptureMouse)
	{
		PlayerController->GetInputMouseDelta(AxisX_Out, AxisY_Out);
		if (AxisX_Out != 0 || AxisY_Out != 0)
		{
			//Modified By Input Profile Asset
			if (CurrentInputProfileAssets[LocalPlayerIndex] && CurrentInputProfileAssets[LocalPlayerIndex]->Profiles.Contains(CurrentInputProfileNames[LocalPlayerIndex]))
			{
				const auto CurrentInputActionSet = CurrentInputProfileAssets[LocalPlayerIndex]->Profiles[CurrentInputProfileNames[LocalPlayerIndex]];
				AxisX_Out *= CurrentInputActionSet.MouseSettings.SensitivityX;
				AxisX_Out *= CurrentInputActionSet.MouseSettings.bIsInvertedX ? -1 : 1;
				AxisY_Out *= CurrentInputActionSet.MouseSettings.SensitivityY;
				AxisY_Out *= CurrentInputActionSet.MouseSettings.bIsInvertedY ? -1 : 1;
			}

			if (AxisX_Out != 0 || AxisY_Out != 0)
				return;
		}
	}

	//GamePad Left Stick
	if (InputAxis->bCaptureGamePadLeftStick)
	{
		PlayerController->GetInputAnalogStickState(EControllerAnalogStick::CAS_LeftStick, AxisX_Out, AxisY_Out);

		//Modified By Input Profile Asset
		if (CurrentInputProfileAssets[LocalPlayerIndex] && CurrentInputProfileAssets[LocalPlayerIndex]->Profiles.Contains(CurrentInputProfileNames[LocalPlayerIndex]))
		{
			const auto CurrentInputActionSet = CurrentInputProfileAssets[LocalPlayerIndex]->Profiles[CurrentInputProfileNames[LocalPlayerIndex]];
			AxisX_Out *= CurrentInputActionSet.GamePadJoystickLeftSettings.SensitivityX;
			AxisX_Out *= CurrentInputActionSet.GamePadJoystickLeftSettings.bIsInvertedX ? -1 : 1;
			AxisY_Out *= CurrentInputActionSet.GamePadJoystickLeftSettings.SensitivityY;
			AxisY_Out *= CurrentInputActionSet.GamePadJoystickLeftSettings.bIsInvertedY ? -1 : 1;
		}

		if (AxisX_Out != 0 || AxisY_Out != 0)
			return;
	}

	//GamePad Right Stick
	if (InputAxis->bCaptureGamePadRightStick)
	{
		PlayerController->GetInputAnalogStickState(EControllerAnalogStick::CAS_RightStick, AxisX_Out, AxisY_Out);

		//Modified By Input Profile Asset
		if (CurrentInputProfileAssets[LocalPlayerIndex] && CurrentInputProfileAssets[LocalPlayerIndex]->Profiles.Contains(CurrentInputProfileNames[LocalPlayerIndex]))
		{
			const auto CurrentInputActionSet = CurrentInputProfileAssets[LocalPlayerIndex]->Profiles[CurrentInputProfileNames[LocalPlayerIndex]];
			AxisX_Out *= CurrentInputActionSet.GamePadJoystickRightSettings.SensitivityX;
			AxisX_Out *= CurrentInputActionSet.GamePadJoystickRightSettings.bIsInvertedX ? -1 : 1;
			AxisY_Out *= CurrentInputActionSet.GamePadJoystickRightSettings.SensitivityY;
			AxisY_Out *= CurrentInputActionSet.GamePadJoystickRightSettings.bIsInvertedY ? -1 : 1;
		}

		if (AxisX_Out != 0 || AxisY_Out != 0)
			return;
	}

	//GamePad DPad
	if (InputAxis->bCaptureGamePadDPad)
	{
		if (PlayerController->IsInputKeyDown(EKeys::Gamepad_DPad_Up)) AxisY_Out = 1;
		if (PlayerController->IsInputKeyDown(EKeys::Gamepad_DPad_Down)) AxisY_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::Gamepad_DPad_Left)) AxisX_Out = -1;
		if (PlayerController->IsInputKeyDown(EKeys::Gamepad_DPad_Right)) AxisX_Out = 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Subsystem : Keys | Buttons

void UFluidToolboxInputSubsystem::GetIsInputKeyDown(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
	if (!PlayerController)
	{
		Outcome = EFluidToolboxInputInputOutput::Failure;
		return;
	}

	Outcome = EFluidToolboxInputInputOutput::Failure;
	if (PlayerController->IsInputKeyDown(KeyboardKey) || PlayerController->IsInputKeyDown(GamePadKey))
		Outcome = EFluidToolboxInputInputOutput::Success;

}

void UFluidToolboxInputSubsystem::GetIsInputKeyPressed(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
	if (!PlayerController)
	{
		Outcome = EFluidToolboxInputInputOutput::Failure;
		return;
	}

	AddInputKeyPairToProcess(LocalPlayerIndex,KeyboardKey, GamePadKey);

	Outcome = EFluidToolboxInputInputOutput::Failure;
	if (PlayerController->WasInputKeyJustPressed(KeyboardKey) || PlayerController->WasInputKeyJustPressed(GamePadKey))
		Outcome = EFluidToolboxInputInputOutput::Success;
}

void UFluidToolboxInputSubsystem::GetIsInputKeyReleased(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, float HeldTimeMin, float HeldTimeMax, bool bAllowRepeats, TEnumAsByte<EFluidToolboxInputInputOutput>& Outcome)
{
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
	if (!PlayerController)
	{
		Outcome = EFluidToolboxInputInputOutput::Failure;
		return;
	}

	AddInputKeyPairToProcess(LocalPlayerIndex,KeyboardKey, GamePadKey);

	//Pressed
	if (PlayerController->WasInputKeyJustPressed(KeyboardKey) || PlayerController->WasInputKeyJustPressed(GamePadKey))
	{
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputHeldStates[KeyboardKey] = true;
		InputHeldStates[GamePadKey] = true;
	}

	//Update
	if (InputHeldStates[KeyboardKey])
	{
		InputHeldTimes[KeyboardKey] += PlayerController->GetWorld()->GetDeltaSeconds();
		InputHeldTimes[GamePadKey] += PlayerController->GetWorld()->GetDeltaSeconds();
	}

	//Release
	Outcome = EFluidToolboxInputInputOutput::Failure;
	if (!InputDidRepeats[KeyboardKey] && !InputDidRepeats[GamePadKey])
		if (PlayerController->WasInputKeyJustReleased(KeyboardKey) || PlayerController->WasInputKeyJustReleased(GamePadKey))
			if (InputHeldTimes[KeyboardKey] >= HeldTimeMin && InputHeldTimes[KeyboardKey] <= HeldTimeMax)
				Outcome = EFluidToolboxInputInputOutput::Success;

	//Repeat
	if (InputHeldTimes[KeyboardKey] >= HeldTimeMin && InputHeldTimes[KeyboardKey] <= HeldTimeMax && bAllowRepeats)
	{
		Outcome = EFluidToolboxInputInputOutput::Success;
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputDidRepeats[KeyboardKey] = true;
		InputDidRepeats[GamePadKey] = true;
	}

	//No Key Down
	if (!PlayerController->IsInputKeyDown(KeyboardKey) && !PlayerController->IsInputKeyDown(GamePadKey))
	{
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputHeldStates[KeyboardKey] = false;
		InputHeldStates[KeyboardKey] = false;
		InputDidRepeats[KeyboardKey] = false;
		InputDidRepeats[GamePadKey] = false;
	}
}

void UFluidToolboxInputSubsystem::GetIsInputKeyPressedHeld(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey, TEnumAsByte<EFluidToolboxInputInputPressedHeldOutput>& Outcome)
{
	LocalPlayerIndex = FMath::Clamp(LocalPlayerIndex, 0, 7);
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);

	if (!PlayerController)
	{
		Outcome = EFluidToolboxInputInputPressedHeldOutput::FailureBoth;
		return;
	}

	AddInputKeyPairToProcess(LocalPlayerIndex,KeyboardKey, GamePadKey);

	//Pressed
	if (PlayerController->WasInputKeyJustPressed(KeyboardKey) || PlayerController->WasInputKeyJustPressed(GamePadKey))
	{
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputHeldStates[KeyboardKey] = true;
		InputHeldStates[GamePadKey] = true;
	}

	//Update
	if (InputHeldStates[KeyboardKey])
	{
		InputHeldTimes[KeyboardKey] += PlayerController->GetWorld()->GetDeltaSeconds();
		InputHeldTimes[GamePadKey] += PlayerController->GetWorld()->GetDeltaSeconds();
	}

	//Release
	Outcome = EFluidToolboxInputInputPressedHeldOutput::FailureBoth;
	if (!InputDidRepeats[KeyboardKey] && !InputDidRepeats[GamePadKey])
		if (PlayerController->WasInputKeyJustReleased(KeyboardKey) || PlayerController->WasInputKeyJustReleased(GamePadKey))
		{
			if (InputHeldTimes[KeyboardKey] >= HeldTimeMinA && InputHeldTimes[KeyboardKey] <= HeldTimeMaxA)
				Outcome = EFluidToolboxInputInputPressedHeldOutput::Pressed;
			else
				if (InputHeldTimes[KeyboardKey] >= HeldTimeMinB && InputHeldTimes[KeyboardKey] <= HeldTimeMaxB)
					Outcome = EFluidToolboxInputInputPressedHeldOutput::Held;
		}

	//Repeat
	if (InputHeldTimes[KeyboardKey] >= HeldTimeMinB && InputHeldTimes[KeyboardKey] <= HeldTimeMaxB)
	{
		Outcome = EFluidToolboxInputInputPressedHeldOutput::HeldRepeat;
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputDidRepeats[KeyboardKey] = true;
		InputDidRepeats[GamePadKey] = true;
	}

	//No Key Down
	if (!PlayerController->IsInputKeyDown(KeyboardKey) && !PlayerController->IsInputKeyDown(GamePadKey))
	{
		InputHeldTimes[KeyboardKey] = 0;
		InputHeldTimes[GamePadKey] = 0;
		InputHeldStates[KeyboardKey] = false;
		InputHeldStates[KeyboardKey] = false;
		InputDidRepeats[KeyboardKey] = false;
		InputDidRepeats[GamePadKey] = false;
	}
}

void UFluidToolboxInputSubsystem::AddInputKeyPairToProcess(int LocalPlayerIndex, FKey KeyboardKey, FKey GamePadKey)
{
	//Add To Array If Needed
	if (!InputHeldStates.Contains(KeyboardKey))
	{
		InputHeldTimes.Add(KeyboardKey);
		InputHeldStates.Add(KeyboardKey);
		InputDidRepeats.Add(KeyboardKey);
	}
	
	if (!InputHeldStates.Contains(GamePadKey))
	{
		InputHeldTimes.Add(GamePadKey);
		InputHeldStates.Add(GamePadKey);
		InputDidRepeats.Add(GamePadKey);
	}
}