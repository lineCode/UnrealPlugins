// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidAISensorsSubsystem.h"
#include "Engine/SkeletalMeshSocket.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Developer Settings

UFluidAISensorsDeveloperSettings::UFluidAISensorsDeveloperSettings()
{
	DebugInput.AddKey = EKeys::Add;
	DebugInput.SubtractKey = EKeys::Subtract;
	DebugInput.ClearKey = EKeys::NumPadZero;
	DebugSensorTarget.TextSensedColor = FColor::Red;
	DebugSensorTarget.TextUnsensedColor = FColor::Green;
	DebugSensorTarget.TextScale = 1;
	DebugSensorTarget.bTextHasShadow = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Blueprint Function Library

UFluidAISensorsSubsystem* UFluidAISensorsBlueprintFunctionLibrary::GetFluidAISensorsSubsystem()
{
	if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWorld() && GEngine->GameViewport->GetWorld()->GetGameInstance())
		return GEngine->GameViewport->GetWorld()->GetGameInstance()->GetSubsystem<UFluidAISensorsSubsystem>();
	return  nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Main

void UFluidAISensorsSubsystem::Initialize(FSubsystemCollectionBase & Collection)
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	InitializeSensors();
	InitializeSensorTargets();
	bInitialized = true;
}

void UFluidAISensorsSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	//Update
	UpdateSensorsStart(DeltaTime);
	UpdateSensorsTransformBindings(DeltaTime);
	UpdateSensorTargetsStart(DeltaTime);
	UpdateSensorTargetsTransformBindings(DeltaTime);
	DebugSensorsRadiusAndAngleAll();
	DebugSensorsRadiusAndAngleSelected();
	DebugSensorTargetsAll();
	UpdateSensorsEnd(DeltaTime);
	UpdateSensorTargetsEnd(DeltaTime);
	
}

bool UFluidAISensorsSubsystem::IsTickable() const
{
	//Validate
	return !IsTemplate() && bInitialized;
}

bool UFluidAISensorsSubsystem::IsTickableInEditor() const
{
	return true;
}

bool UFluidAISensorsSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidAISensorsSubsystem::GetStatId() const
{
	return TStatId();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Sensors

void UFluidAISensorsSubsystem::InitializeSensors()
{
	// Initialize Sensor Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_AI_SENSORS_SENSORS_MAX; ++i)
		SensorsAvailable.push(i);
}

void UFluidAISensorsSubsystem::CreateSensor(const FName BatchGuidAsName, const FName DebugProfileName, const double Radius, const float Angle, const float AngleOffset, const bool bHasVision, const float UpdateInterval, int& Sensor_Out)
{
	//Invalid Sensor By Default
	Sensor_Out = -1;
	
	//Return If We Reached Max Sensor Count Or Sensor Is Not Valid
	if (SensorCount >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;
	
	//Create Sensor
	Sensor_Out = SensorsAvailable.front();
	SensorsAvailable.pop();
	SensorActives[Sensor_Out] = true;
	SensorJustCreateds[Sensor_Out] = true;
	SensorSetToDeletes[Sensor_Out] = false;
	++SensorCount;
	
	//Set Sensor Update Interval Data
	SensorUpdateIntervals[Sensor_Out] = UpdateInterval;
	SensorUpdateIntervalSteps[Sensor_Out] = FMath::FRandRange(0, SensorUpdateIntervals[Sensor_Out]);

	//Debug
	SensorDebugProfileNames[Sensor_Out] = DebugProfileName;

	//Transform
	SensorTransformActorBindings[Sensor_Out] = nullptr;
	SensorTransformSkeletalMeshComponentBindings[Sensor_Out] = nullptr;
	SensorTransformSocketNameBindings[Sensor_Out] = "";
	SensorTransformLocations[Sensor_Out] = FVector::ZeroVector;
	SensorTransformEulerAngles[Sensor_Out] = FVector::ZeroVector;

	//Radius | Angle
	SensorRadiuses[Sensor_Out] = Radius;
	SensorRadiusScales[Sensor_Out] = 1;
	SensorAngles[Sensor_Out] = Angle;
	SensorAngleOffsets[Sensor_Out] = AngleOffset;

	//Type
	bSensorHasVisions[Sensor_Out] = bHasVision;
	
	//Batch Guid
	SensorBatchGuidAsNames[Sensor_Out] = BatchGuidAsName;
}

void UFluidAISensorsSubsystem::CreateSensorByAssetModel(UFluidAISensorsDataAsset* Asset, const FName BatchGuidAsName, const FName ModelName, int& Sensor_Out)
{
	//Return If Not Valid
	Sensor_Out = -1;
	if (Asset == nullptr || !Asset->SensorModels.Contains(ModelName))
		return;

	//Create Sensor From Model
	const auto Model = Asset->SensorModels[ModelName];
	CreateSensor(BatchGuidAsName,Model.DebugProfileName,Model.Radius,Model.Angle,Model.AngleOffset,Model.bHasVision,Model.UpdateInterval,Sensor_Out);
}

void UFluidAISensorsSubsystem::CreateSensorByAssetModelWithAutoBindings(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ModelName, AActor* ActorTransformBinding, USkeletalMeshComponent* SkeletalMeshComponentTransformBinding, int& Sensor_Out)
{
	//Return If Not Valid
	Sensor_Out = -1;
	if (Asset == nullptr || !Asset->SensorModels.Contains(ModelName))
		return;

	//Create Sensor From Model
	const auto Model = Asset->SensorModels[ModelName];
	CreateSensor(BatchGuidAsName,Model.DebugProfileName,Model.Radius,Model.Angle,Model.AngleOffset,Model.bHasVision,Model.UpdateInterval,Sensor_Out);

	//Actor Binding
	if (Model.bTryAutoBindTransformToActor && ActorTransformBinding != nullptr)
		BindSensorTransformToActor(Sensor_Out,ActorTransformBinding);

	//Skeletal Mesh Component Binding
	if (Model.bTryAutoBindTransformToSkeletalMeshSocket && SkeletalMeshComponentTransformBinding != nullptr)
		BindSensorTransformToSkeletalMeshSocket(Sensor_Out,SkeletalMeshComponentTransformBinding,Model.TryAutoBindTransformSocketName);
		
}

void UFluidAISensorsSubsystem::CreateSensorsByAssetModelList(UFluidAISensorsDataAsset* Asset, const FName BatchGuidAsName, const FName ListName, TArray<int>& Sensors_Out)
{
	//Return If Not Valid
	Sensors_Out.Empty();
	if (Asset == nullptr || !Asset->SensorModelList.Contains(ListName))
		return;

	//Create Sensors From Model List
	const auto ListItems = Asset->SensorModelList[ListName].Items;
	for (int i = 0; i < ListItems.Num(); i++)
	{
		int Sensor = -1;
		CreateSensorByAssetModel(Asset,BatchGuidAsName,ListItems[i],Sensor);
		if (Sensor >= 0)
			Sensors_Out.Add(Sensor);
	}
}

void UFluidAISensorsSubsystem::CreateSensorsByAssetModelListWithAutoBindings(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ListName, AActor* ActorTransformBinding, USkeletalMeshComponent* SkeletalMeshComponentTransformBinding, TArray<int>& Sensors_Out)
{
	//Return If Not Valid
	Sensors_Out.Empty();
	if (Asset == nullptr || !Asset->SensorModelList.Contains(ListName))
		return;

	//Create Sensors From Model List
	const auto ListItems = Asset->SensorModelList[ListName].Items;
	for (int i = 0; i < ListItems.Num(); i++)
	{
		int Sensor = -1;
		CreateSensorByAssetModelWithAutoBindings(Asset,BatchGuidAsName,ListItems[i],ActorTransformBinding,SkeletalMeshComponentTransformBinding,Sensor);
		if (Sensor >= 0)
			Sensors_Out.Add(Sensor);
	}
}


void UFluidAISensorsSubsystem::SetSensorTransform(const int Sensor, const FVector Location, const FVector EulerAngle)
{
	//Return If Sensor Is Not Valid
	if (Sensor < 0 || Sensor >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;

	//Set Transform
	SensorTransformLocations[Sensor] = Location;
	SensorTransformEulerAngles[Sensor] = EulerAngle;
}

void UFluidAISensorsSubsystem::BindSensorTransformToActor(const int Sensor, AActor* Actor)
{
	//Return If Sensor Is Not Valid
	if (Sensor < 0 || Sensor >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;

	//Set Actor Binding
	SensorTransformActorBindings[Sensor] = Actor;

	//Null Out Other Bindings
	SensorTransformSkeletalMeshComponentBindings[Sensor] = nullptr;
}

void UFluidAISensorsSubsystem::BindSensorTransformToSkeletalMeshSocket(const int Sensor, USkeletalMeshComponent* SkeletalMeshComponent, const FName SocketName)
{
	//Return If Sensor Is Not Valid
	if (Sensor < 0 || Sensor >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;

	//Set Skeletal Mesh Binding
	SensorTransformSkeletalMeshComponentBindings[Sensor] = SkeletalMeshComponent;
	SensorTransformSocketNameBindings[Sensor] = SocketName;
	
	//Null Out Other Bindings
	SensorTransformActorBindings[Sensor] = nullptr;
}

void UFluidAISensorsSubsystem::DestroySensor(const int Sensor)
{
	//Return If Sensor Is Not Valid
	if (Sensor < 0 ||Sensor >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;
	
	SensorSetToDeletes[Sensor] = true;
}

void UFluidAISensorsSubsystem::DestroySensorsInBatch(const FName BatchGuidAsName)
{
	//Delete Sensors In Batch
	for (int i = 0; i < FLUID_AI_SENSORS_SENSORS_MAX; i++)
		if (SensorBatchGuidAsNames[i] == BatchGuidAsName)
			SensorSetToDeletes[i] = true;
}

void UFluidAISensorsSubsystem::GetSensorsInBatch(const FName BatchGuidAsName, TArray<int>& Sensors_Out)
{
	//Get Sensors In Batch
	for (int i = 0; i < FLUID_AI_SENSORS_SENSORS_MAX; i++)
		if (SensorBatchGuidAsNames[i] == BatchGuidAsName)
			Sensors_Out.Add(i);
}

void UFluidAISensorsSubsystem::SetSensorRadiusScale(const int Sensor, const double Scale)
{
	//Return If Sensor Is Not Valid
	if (Sensor < 0 ||Sensor >= FLUID_AI_SENSORS_SENSORS_MAX)
		return;

	SensorRadiusScales[Sensor] = Scale;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Sensor Targets

void UFluidAISensorsSubsystem::InitializeSensorTargets()
{
	// Initialize Sensor Targets Available Queue With All Possible Entities
	for (int i = 0; i < FLUID_AI_SENSORS_TARGETS_MAX; ++i)
		SensorTargetsAvailable.push(i);
}

void UFluidAISensorsSubsystem::CreateSensorTarget(const FName Name, const float UpdateInterval,  int& SensorTarget_Out)
{
	//Invalid Sensor Target By Default
	SensorTarget_Out = -1;
	
	//Return If We Reached Max Sensor Count Or Sensor Is Not Valid
	if (SensorTargetCount >= FLUID_AI_SENSORS_TARGETS_MAX)
		return;
	
	//Create Sensor Target
	SensorTarget_Out = SensorTargetsAvailable.front();
	SensorTargetsAvailable.pop();
	SensorTargetActives[SensorTarget_Out ] = true;
	SensorTargetJustCreateds[SensorTarget_Out ] = true;
	SensorTargetSetToDeletes[SensorTarget_Out ] = false;
	++SensorTargetCount;

	//Name
	SensorTargetNames[SensorTarget_Out] = Name;
	
	//Set Sensor Target Update Interval Data
	SensorTargetUpdateIntervals[SensorTarget_Out] = UpdateInterval;
	SensorTargetUpdateIntervalSteps[SensorTarget_Out] = FMath::FRandRange(0, SensorTargetUpdateIntervals[SensorTarget_Out]);
	
	//Transform
	SensorTargetTransformActorBindings[SensorTarget_Out] = nullptr;
	SensorTargetTransformSkeletalMeshComponentBindings[SensorTarget_Out] = nullptr;
	SensorTargetTransformSocketNameBindings[SensorTarget_Out] = "";
	SensorTargetTransformLocations[SensorTarget_Out] = FVector::ZeroVector;
}

void UFluidAISensorsSubsystem::SetSensorTargetTransform(const int SensorTarget, const FVector Location)
{
	//Return If Sensor Target Is Not Valid
	if (SensorTarget < 0 || SensorTarget >= FLUID_AI_SENSORS_TARGETS_MAX)
		return;

	//Set Transform
	SensorTargetTransformLocations[SensorTarget] = Location;
}

void UFluidAISensorsSubsystem::BindSensorTargetTransformToActor(const int SensorTarget, AActor* Actor)
{
	//Return If Sensor Target Is Not Valid
	if (SensorTarget < 0 || SensorTarget >= FLUID_AI_SENSORS_TARGETS_MAX)
		return;

	//Set Actor Binding
	SensorTargetTransformActorBindings[SensorTarget] = Actor;
	
	//Null Out Other Bindings
	SensorTargetTransformSkeletalMeshComponentBindings[SensorTarget] = nullptr;
}

void UFluidAISensorsSubsystem::BindSensorTargetTransformToSkeletalMeshSocket(const int SensorTarget, USkeletalMeshComponent* SkeletalMeshComponent, const FName SocketName)
{
	//Return If Sensor Target Is Not Valid
	if (SensorTarget < 0 || SensorTarget >= FLUID_AI_SENSORS_TARGETS_MAX)
		return;

	//Set Actor Binding
	SensorTargetTransformSkeletalMeshComponentBindings[SensorTarget] = SkeletalMeshComponent;
	SensorTargetTransformSocketNameBindings[SensorTarget] = SocketName;
	
	//Null Out Other Bindings
	SensorTargetTransformActorBindings[SensorTarget] = nullptr;
}


void UFluidAISensorsSubsystem::DestroySensorTarget(const int SensorTarget)
{
	//Return If Sensor Is Not Valid
	if (SensorTarget < 0 || SensorTarget >= FLUID_AI_SENSORS_TARGETS_MAX)
		return;
	
	SensorTargetSetToDeletes[SensorTarget] = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Update Sensors

void UFluidAISensorsSubsystem::UpdateSensorsStart(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_Start);
		{
			for (int i = 0; i <  FLUID_AI_SENSORS_SENSORS_MAX; i++)
				if (SensorActives[i])
					SensorUpdateIntervalSteps[i] -= DeltaTime;
		}
	}
}

void UFluidAISensorsSubsystem::UpdateSensorsEnd(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_End);
		{
			//Sensors
			for (int i = 0; i <  FLUID_AI_SENSORS_SENSORS_MAX; i++)
			{
				//Sensor Update Interval Step Reset
				SensorJustCreateds[i] = false;
				if (SensorActives[i] && SensorUpdateIntervalSteps[i] < 0)
					SensorUpdateIntervalSteps[i] = SensorUpdateIntervals[i];

				//Delete Sensor
				if (SensorSetToDeletes[i])
				{
					// Put Destroyed Sensor Back In Available Que (At The Back) | Decrease Sensor Count
					SensorsAvailable.push(i);
					SensorActives[i] = false;
					SensorSetToDeletes[i] = false;

					//Clear Transform Bindings
					SensorTransformActorBindings[i] = nullptr;
					SensorTransformSkeletalMeshComponentBindings[i] = nullptr;

					//Minus Sensor Count
					--SensorCount;
				}
			}
		}
	}
}

void UFluidAISensorsSubsystem::UpdateSensorsTransformBindings(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensors_Bindings);
		{
			for (int i = 0; i <  FLUID_AI_SENSORS_SENSORS_MAX; i++)
			{
				//Continue If Sensor Is Not Active Or Interval Steps Not 0
				if (!SensorActives[i] || SensorUpdateIntervalSteps[i] > 0 && !SensorJustCreateds[i])
					continue;

				//Actor
				if (SensorTransformActorBindings[i])
				{
					auto const Transform = SensorTransformActorBindings[i]->GetTransform();
					SensorTransformLocations[i] = Transform.GetLocation();
					SensorTransformEulerAngles[i] = Transform.GetRotation().Euler();
				}

				//Skeletal Mesh Socket
				else if (SensorTransformSkeletalMeshComponentBindings[i])
				{
					auto const Transform = SensorTransformSkeletalMeshComponentBindings[i]->GetSocketTransform(SensorTransformSocketNameBindings[i]);
					SensorTransformLocations[i] = Transform.GetLocation();
					SensorTransformEulerAngles[i] = Transform.GetRotation().Euler();
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Update Sensor Targets

void UFluidAISensorsSubsystem::UpdateSensorTargetsStart(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_Start);
		{
			for (int i = 0; i <  FLUID_AI_SENSORS_TARGETS_MAX; i++)
				if (SensorTargetActives[i])
					SensorTargetUpdateIntervalSteps[i] -= DeltaTime;
		}
	}
}

void UFluidAISensorsSubsystem::UpdateSensorTargetsEnd(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_End);
		{
			//Sensor Targets
			for (int i = 0; i <  FLUID_AI_SENSORS_TARGETS_MAX; i++)
			{
				//Sensor Target Update Interval Step Reset
				SensorTargetJustCreateds[i] = false;
				if (SensorTargetActives[i] && SensorTargetUpdateIntervalSteps[i] < 0)
					SensorTargetUpdateIntervalSteps[i] = SensorTargetUpdateIntervals[i];

				//Delete Sensor Target
				if (SensorTargetSetToDeletes[i])
				{
					//Put Destroyed Sensor Target Back In Available Que (At The Back)
					SensorTargetsAvailable.push(i);
					SensorTargetActives[i] = false;
					SensorTargetSetToDeletes[i] = false;

					//Clear Batch Guid
					SensorBatchGuidAsNames[i] = "";

					//Clear Bindings
					SensorTransformActorBindings[i] = nullptr;
					SensorTransformSkeletalMeshComponentBindings[i] = nullptr;

					//Decrease Sensor Target Count
					--SensorTargetCount;
				}
			}
		}
	}
}

void UFluidAISensorsSubsystem::UpdateSensorTargetsTransformBindings(const float DeltaTime)
{
	//Get Stats For This Code (Total)
	SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_Total);
	{
		//Get Stats For This Code
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Update_Sensor_Targets_Bindings);
		{
			for (int i = 0; i < FLUID_AI_SENSORS_TARGETS_MAX; i++)
			{
				//Continue If Agent Is Not Active Or Interval Steps Not 0
				if (!SensorTargetActives[i] || SensorTargetUpdateIntervalSteps[i] > 0 && !SensorTargetJustCreateds[i])
					continue;

				//Actor
				if (SensorTargetTransformActorBindings[i])
					SensorTargetTransformLocations[i] = SensorTargetTransformActorBindings[i]->GetActorLocation();

				//Skeletal Mesh Socket
				else if (SensorTargetTransformSkeletalMeshComponentBindings[i])
					SensorTargetTransformLocations[i] = SensorTargetTransformSkeletalMeshComponentBindings[i]->GetSocketTransform(SensorTargetTransformSocketNameBindings[i]).GetLocation();
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Debug Sensors

static TAutoConsoleVariable<float> CDebugFluidAISensorsSensorsDebug(
	TEXT("Fluid.AI.Sensors.Debug.Sensors"),
	0,
	TEXT("Fluid.AI.Sensors.Debug.Sensors\n"),
	ECVF_SetByConsole);

void UFluidAISensorsSubsystem::DebugSensorsRadiusAndAngleAll() const
{
	//1 == All
	if (CDebugFluidAISensorsSensorsDebug.GetValueOnAnyThread() == 1)
	{
		//Get Stats For This Code (Total)
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Total);
		{
			//Get Stats For This Code
			SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Radius_Angle);
			{
				//If Debug On
				if (CDebugFluidAISensorsSensorsDebug.GetValueOnAnyThread() == 1)
				{
					TArray<int> Sensors;
					for (int i = 0; i < FLUID_AI_SENSORS_SENSORS_MAX; i++)
						Sensors.Add(i);
					DebugSensorsRadiusAndAngle(Sensors);
				}
			}
		}
	}
}

void UFluidAISensorsSubsystem::DebugSensorsRadiusAndAngleSelected()
{
	//2 == Selected Debug Actors
	if (CDebugFluidAISensorsSensorsDebug.GetValueOnAnyThread() == 2)
	{
		//Get Stats For This Code (Total)
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Total);
		{
			//Get Stats For This Code
			SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Radius_Angle);
			{
				//Cache World
				UWorld* World = GetWorld();
				
				//Get Developer Settings Debug Input
				const auto DeveloperSettingsDebugInput = GetMutableDefault<UFluidAISensorsDeveloperSettings>()->DebugInput;
				
				//Get First Player Controller
				const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
				if (PlayerController)
				{
					//Get Pawn
					const APawn* Pawn = PlayerController->GetPawnOrSpectator();
					if (Pawn)
					{
						bool bAdd = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.AddKey);
						bool bSubtract = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.SubtractKey);
						bool bClear = PlayerController->WasInputKeyJustPressed(DeveloperSettingsDebugInput.ClearKey);

						//Add | Subtract
						if (bAdd || bSubtract)
						{
							//Source
							const FVector SourceLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
							const FVector SourceForward = PlayerController->PlayerCameraManager->GetCameraRotation().Quaternion().GetForwardVector();
							const double SourceDistance = 5000000;
							
							//Get Closest Sensor Target
							int ClosestSensor = -1;
							float ClosestDot = 0.75f;
							for (int i = 0; i < FLUID_AI_SENSORS_TARGETS_MAX; i++)
							{
								const double Distance =  FVector::Distance(SourceLocation,SensorTransformLocations[i]);
								FVector Direction = (SensorTransformLocations[i] - SourceLocation).GetSafeNormal();
								float Dot = FVector::DotProduct(SourceForward,Direction);
								
								if (SensorActives[i] && !SensorSetToDeletes[i] && Dot > ClosestDot && Distance <= SourceDistance)
								{
									ClosestSensor = i;
									ClosestDot = Dot;
								}
							}

							//Add | Subtract
							if (bAdd)
								DebugSensors.AddUnique(ClosestSensor);
							if (bSubtract)
								DebugSensors.Remove(ClosestSensor);
							
							//Add | Subtract All In Same Batch
							if (ClosestSensor >= 0 && SensorBatchGuidAsNames[ClosestSensor] != "")
							{
								for (int i = 0; i < FLUID_AI_SENSORS_TARGETS_MAX; i++)
								{
									if (SensorBatchGuidAsNames[i] ==  SensorBatchGuidAsNames[ClosestSensor])
									{
										//Add | Subtract
										if (bAdd)
											DebugSensors.AddUnique(i);
										if (bSubtract)
											DebugSensors.Remove(i);
									}
								}
							}
						}

						//Clear
						if (bClear)
							DebugSensors.Empty();
					}
				}

				//Remove Inactive Debug Sensors
				for (int i = DebugSensors.Num(); i --> 0;)
					if (!SensorActives[DebugSensors[i]])
						DebugSensors.RemoveAt(i);

				//Debug
				DebugSensorsRadiusAndAngle(DebugSensors);
			}
		}
	}
}

void UFluidAISensorsSubsystem::DebugSensorsRadiusAndAngle(const TArray<int> Sensors) const
{
	//Get Developer Settings Debug Profiles
	const auto DeveloperSettingsDebugProfiles = GetMutableDefault<UFluidAISensorsDeveloperSettings>()->DebugSensorProfiles;
				
	//Cache World
	const UWorld* World = GetWorld();

	//Each Sensor
	for (int i = 0; i < Sensors.Num(); i++)
	{
		//Get Sensor
		const int Sensor = Sensors[i];
		
		//Continue If Sensor Is Not Active Or Interval Steps Not 0
		if (!SensorActives[Sensor] || SensorUpdateIntervalSteps[Sensor] > 0 && !SensorJustCreateds[Sensor])
			continue;
						
		//Read Shared Data
		const FVector Location = SensorTransformLocations[Sensor];
		const float UpdateIntervals = SensorUpdateIntervals[Sensor];
		const double Radius = SensorRadiuses[Sensor] * SensorRadiusScales[Sensor];

		//Debug Parameters
		const FName SensorDebugProfileName = SensorDebugProfileNames[Sensor];
		FColor Color = FColor::White;
		float Thickness = 5;
		int Segments = 120;
		int DepthPriority = 1;
		bool bIs3D = false;
		if (DeveloperSettingsDebugProfiles.Contains(SensorDebugProfileName))
		{
			Color = DeveloperSettingsDebugProfiles[SensorDebugProfileName].Color;
			Thickness = DeveloperSettingsDebugProfiles[SensorDebugProfileName].Thickness;
			Segments = DeveloperSettingsDebugProfiles[SensorDebugProfileName].Segments;
			DepthPriority = DeveloperSettingsDebugProfiles[SensorDebugProfileName].DepthPriority;
			bIs3D = DeveloperSettingsDebugProfiles[SensorDebugProfileName].bIs3D;
		}

		//LifeTime
		const float LifeTime = FMath::Clamp(UpdateIntervals,0,0.5f);

		//Radius
		DrawDebugCircle(World,Location + FVector(0,0,0),Radius,Segments, Color,false,LifeTime,DepthPriority,Thickness,FVector(1,0,0),FVector(0,1,0),false);

		if (bIs3D)
		{
			DrawDebugCircle(World,Location + FVector(0,0,0),Radius,Segments, Color,false,LifeTime,DepthPriority,Thickness,FVector(1,0,0),FVector(0,0,1),false);
			DrawDebugCircle(World,Location + FVector(0,0,0),Radius,Segments, Color,false,LifeTime,DepthPriority,Thickness,FVector(0,1,0),FVector(0,0,1),false);
		}
						
		//Angle
		{
			//Read Data
			const FVector EulerAngle = SensorTransformEulerAngles[Sensor];
			//const FVector Forward = SensorForwards[Sensor];
			const float AngleOffset = SensorAngleOffsets[Sensor];
			const float Angle = SensorAngles[Sensor];
							
			//Get Angle (0.5 To Convert It To 180 Degrees From 360 So We can Get Left And Right Side Separately To Draw)
			//Based On Euler Angle And Offset
			float AngleLeft = (Angle * 0.5f) - EulerAngle.Z + AngleOffset; // 0.5 To Convert It To 180 Degrees From 360 As We Only Need Half Of Circle As We Use Two Angles (One On Left And One On Right)
			float AngleRight = (-Angle * 0.5f) - EulerAngle.Z + AngleOffset;
							
			//Convert To Radians
			AngleLeft = FMath::DegreesToRadians(AngleLeft);
			AngleRight = FMath::DegreesToRadians(AngleRight);
							
			//Get Angle Locations
			FVector AngleLeftLocation = Location +  FVector(FMath::Sin(AngleLeft),FMath::Cos(AngleLeft),0) * Radius;
			FVector AngleRightLocation  = Location + FVector(FMath::Sin(AngleRight),FMath::Cos(AngleRight),0) * Radius;
			//FVector AngleCenterLocation = Location + Forward * Radius;
							
			//Draw Angle
			DrawDebugLine(World,Location,AngleLeftLocation,Color,false,LifeTime,DepthPriority,Thickness);
			DrawDebugLine(World,Location,AngleRightLocation,Color,false,LifeTime,DepthPriority,Thickness);
							
			//DrawDebugLine(World,Location,AngleCenterLocation,Color,false,-1,DepthPriority,Thickness);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid AI Sensors Subsystem : Debug Sensors

static TAutoConsoleVariable<float> CDebugFluidAISensorsTargetsDebug(
	TEXT("Fluid.AI.Sensors.Debug.Targets"),
	0,
	TEXT("Fluid.AI.Sensors.Debug.Targets\n"),
	ECVF_SetByConsole);

void UFluidAISensorsSubsystem::DebugSensorTargetsAll() const
{
	//1 == All
	if (CDebugFluidAISensorsTargetsDebug.GetValueOnAnyThread() == 1)
	{
		//Get Stats For This Code (Total)
		SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Total);
		{
			//Get Stats For This Code
			SCOPE_CYCLE_COUNTER(STAT_FluidAISensors_Debug_Sensor_Targets);
			{
				//Get Developer Settings Debug Sensor Targets
				const auto DeveloperSettingsDebugTargets = GetMutableDefault<UFluidAISensorsDeveloperSettings>()->DebugSensorTarget;
				
				//Cache World
				const UWorld* World = GetWorld();
				
				//Sensor Targets
				for (int i = 0; i < FLUID_AI_SENSORS_TARGETS_MAX; i++)
				{
					//Get Sensor Target
					const int SensorTarget = i;
		
					//Continue If Sensor Target Is Not Active Or Interval Steps Not 0
					if (!SensorTargetActives[SensorTarget] || SensorTargetUpdateIntervalSteps[SensorTarget] > 0 && !SensorTargetJustCreateds[SensorTarget])
						continue;

					//Get Target Location
					const FString Name = SensorTargetNames[SensorTarget].ToString();
					const FVector Location = SensorTargetTransformLocations[SensorTarget];
					const float LifeTime = SensorTargetUpdateIntervals[SensorTarget];

					//Text
					const FColor Color = DeveloperSettingsDebugTargets.TextUnsensedColor;
					const float TextScale = DeveloperSettingsDebugTargets.TextScale;
					const bool bDrawShadow = DeveloperSettingsDebugTargets.bTextHasShadow;

					//Draw
					DrawDebugString(World,Location + FVector(0,0,10),Name,0,Color,LifeTime,bDrawShadow,TextScale);
				}
			}
		}
	}
}