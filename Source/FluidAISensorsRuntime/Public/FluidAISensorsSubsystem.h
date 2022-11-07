// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <queue>
#include "CoreMinimal.h"
#include "CoreFwd.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataAsset.h"
#include "Engine/DeveloperSettings.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "FluidAISensorsSubsystem.generated.h"

//Forward Declarations
class UFluidAISensorsSubsystem;

//Fluid AI Sensors Update Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid AI Sensors"), STATGROUP_FluidAISensors, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensors Start"),STAT_FluidAISensors_Update_Sensors_Start, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensors End"),STAT_FluidAISensors_Update_Sensors_End, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensors Tranform Bindings"),STAT_FluidAISensors_Update_Sensors_Bindings, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensors Total"),STAT_FluidAISensors_Update_Sensors_Total,STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensor Targets Start"),STAT_FluidAISensors_Update_Sensor_Targets_Start, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensor Targets End"),STAT_FluidAISensors_Update_Sensor_Targets_End, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensor Targets Tranform Bindings"),STAT_FluidAISensors_Update_Sensor_Targets_Bindings, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Update Sensor Targets Total"),STAT_FluidAISensors_Update_Sensor_Targets_Total,STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Debug Total"),STAT_FluidAISensors_Debug_Total,STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Debug Sensor Radius | Angle"),STAT_FluidAISensors_Debug_Radius_Angle, STATGROUP_FluidAISensors);
DECLARE_CYCLE_STAT(TEXT("Fluid AI Sensors : Debug Sensor Targets"),STAT_FluidAISensors_Debug_Sensor_Targets, STATGROUP_FluidAISensors);

//Fluid AI Sensors Global Constants
const int FLUID_AI_SENSORS_SENSORS_MAX = 10000;
const int FLUID_AI_SENSORS_TARGETS_MAX = 10000;

//Fluid AI Sensors Blueprint Function Library
UCLASS()
class FLUIDAISENSORSRUNTIME_API UFluidAISensorsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static UFluidAISensorsSubsystem* GetFluidAISensorsSubsystem();
};

//Fluid AI Sensors Sensor Model
USTRUCT(BlueprintType)
struct FLUIDAISENSORSRUNTIME_API FFluidAISensorsSensorModel
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category="Main")
		FName DebugProfileName;
	UPROPERTY(EditDefaultsOnly, Category="Main")
		double Radius;
	UPROPERTY(EditDefaultsOnly, Category="Main")
		float Angle;
	UPROPERTY(EditDefaultsOnly, Category="Main")
		float AngleOffset;
	UPROPERTY(EditDefaultsOnly, Category="Main")
		float UpdateInterval;
	UPROPERTY(EditDefaultsOnly, Category="Main")
		bool bHasVision;
	UPROPERTY(EditDefaultsOnly, Category="Transform Binding")
		bool bTryAutoBindTransformToActor;
	UPROPERTY(EditDefaultsOnly, Category="Transform Binding")
		bool bTryAutoBindTransformToSkeletalMeshSocket;
	UPROPERTY(EditDefaultsOnly, Category="Transform Binding")
		FName TryAutoBindTransformSocketName;
};

//Fluid AI Sensors Sensor Name List
USTRUCT(BlueprintType)
struct FLUIDAISENSORSRUNTIME_API FFluidAISensorsNameList
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TArray<FName> Items;
};

//Fluid AI Sensors Data Asset
UCLASS(BlueprintType)
class FLUIDAISENSORSRUNTIME_API UFluidAISensorsDataAsset : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidAISensorsSensorModel> SensorModels;
	UPROPERTY(EditDefaultsOnly)
		TMap<FName,FFluidAISensorsNameList> SensorModelList;
};

//Fluid AI Sensors Developer Settings Debug Profile
USTRUCT()
struct FLUIDAISENSORSRUNTIME_API FFluidAISensorsDeveloperSettingsDebugSensorProfile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		FColor Color;
	UPROPERTY(EditDefaultsOnly)
		float Thickness;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 20, ClampMax = 360))
		int Segments;
	UPROPERTY(EditDefaultsOnly)
		int DepthPriority;
	UPROPERTY(EditDefaultsOnly)
		bool bIs3D;
};

//Fluid AI Sensors Developer Settings Debug Sensor Target
USTRUCT()
struct FLUIDAISENSORSRUNTIME_API FFluidAISensorsDeveloperSettingsDebugSensorTarget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		FColor TextSensedColor;
	UPROPERTY(EditDefaultsOnly)
		FColor TextUnsensedColor;
	UPROPERTY(EditDefaultsOnly)
		float TextScale;
	UPROPERTY(EditDefaultsOnly)
		bool bTextHasShadow;
};

//Fluid AI Sensors Developer Settings Debug Profile
USTRUCT()
struct FLUIDAISENSORSRUNTIME_API FFluidAISensorsDeveloperSettingsDebugInput
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
		FKey AddKey;
	UPROPERTY(EditDefaultsOnly)
		FKey SubtractKey;
	UPROPERTY(EditDefaultsOnly)
		FKey ClearKey;
};

//Fluid AI Sensors Developer Settings
UCLASS(Config=Game, defaultconfig, meta = (DisplayName="Fluid AI Sensors"))
class  FLUIDAISENSORSRUNTIME_API UFluidAISensorsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UFluidAISensorsDeveloperSettings();

	UPROPERTY(Config,EditDefaultsOnly)
		TMap<FName,FFluidAISensorsDeveloperSettingsDebugSensorProfile> DebugSensorProfiles;
	UPROPERTY(Config,EditDefaultsOnly)
		FFluidAISensorsDeveloperSettingsDebugSensorTarget DebugSensorTarget;
	UPROPERTY(Config,EditDefaultsOnly)
		FFluidAISensorsDeveloperSettingsDebugInput DebugInput;
};

//Fluid AI Sensors Subsystem
UCLASS(BlueprintType)
class FLUIDAISENSORSRUNTIME_API UFluidAISensorsSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid AI Sensors Subsystem : Main
public:
	
	void Initialize(FSubsystemCollectionBase & Collection) override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	bool bInitialized;
	
//Fluid AI Sensors Subsystem : Sensors
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeSensors();
	UFUNCTION(BlueprintCallable)
		void CreateSensor(const FName BatchGuidAsName, const FName DebugProfileName, const double Radius, const float Angle, const float AngleOffset, const bool bHasVision, const float UpdateInterval, int& Sensor_Out);
	UFUNCTION(BlueprintCallable)
		void CreateSensorByAssetModel(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ModelName, int& Sensor_Out);
	UFUNCTION(BlueprintCallable)
		void CreateSensorByAssetModelWithAutoBindings(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ModelName, AActor* ActorTransformBinding, USkeletalMeshComponent* SkeletalMeshComponentTransformBinding, int& Sensor_Out);
	UFUNCTION(BlueprintCallable)
		void CreateSensorsByAssetModelList(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ListName, TArray<int>& Sensors_Out);
	UFUNCTION(BlueprintCallable)
		void CreateSensorsByAssetModelListWithAutoBindings(UFluidAISensorsDataAsset* Asset,const FName BatchGuidAsName, const FName ListName,AActor* ActorTransformBinding, USkeletalMeshComponent* SkeletalMeshComponentTransformBinding, TArray<int>& Sensors_Out);
	UFUNCTION(BlueprintCallable)
		void SetSensorTransform(const int Sensor, const FVector Location, const FVector EulerAngle);
	UFUNCTION(BlueprintCallable)
		void BindSensorTransformToActor(const int Sensor, AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void BindSensorTransformToSkeletalMeshSocket(const int Sensor, USkeletalMeshComponent* SkeletalMeshComponent, const FName SocketName);
	UFUNCTION(BlueprintCallable)
		void DestroySensor(const int Sensor);
	UFUNCTION(BlueprintCallable)
		void DestroySensorsInBatch(const FName BatchGuidAsName);
	UFUNCTION(BlueprintCallable)
		void GetSensorsInBatch(const FName BatchGuidAsName, TArray<int>& Sensors_Out);
	UFUNCTION(BlueprintCallable)
		void SetSensorRadiusScale(const int Sensor, const double Scale);

	//Sensor Que
	std::queue<int> SensorsAvailable;
	int SensorCount;
	
	//Main
	bool SensorJustCreateds[FLUID_AI_SENSORS_SENSORS_MAX];
	bool SensorActives[FLUID_AI_SENSORS_SENSORS_MAX];
	bool SensorSetToDeletes[FLUID_AI_SENSORS_SENSORS_MAX];

	//Debug
	FName SensorDebugProfileNames[FLUID_AI_SENSORS_SENSORS_MAX];

	//Update
	float SensorUpdateIntervals[FLUID_AI_SENSORS_SENSORS_MAX];
	float SensorUpdateIntervalSteps[FLUID_AI_SENSORS_SENSORS_MAX];

	//Transform
	AActor* SensorTransformActorBindings[FLUID_AI_SENSORS_SENSORS_MAX];
	USkeletalMeshComponent* SensorTransformSkeletalMeshComponentBindings[FLUID_AI_SENSORS_SENSORS_MAX];
	FName SensorTransformSocketNameBindings[FLUID_AI_SENSORS_SENSORS_MAX];
	FVector SensorTransformLocations[FLUID_AI_SENSORS_SENSORS_MAX];
	FVector SensorTransformEulerAngles[FLUID_AI_SENSORS_SENSORS_MAX];

	//Radius | Angle
	double SensorRadiuses[FLUID_AI_SENSORS_SENSORS_MAX];
	double SensorRadiusScales[FLUID_AI_SENSORS_SENSORS_MAX];
	float SensorAngles[FLUID_AI_SENSORS_SENSORS_MAX];
	float SensorAngleOffsets[FLUID_AI_SENSORS_SENSORS_MAX];

	//Type
	bool bSensorHasVisions[FLUID_AI_SENSORS_SENSORS_MAX];

	//Baatch
	FName SensorBatchGuidAsNames[FLUID_AI_SENSORS_SENSORS_MAX];

//Fluid AI Sensors Subsystem : Sensor Targets
public:

	UFUNCTION(BlueprintInternalUseOnly)
		void InitializeSensorTargets();
	UFUNCTION(BlueprintCallable)
		void CreateSensorTarget(const FName Name, const float UpdateInterval,  int& SensorTarget_Out);
	UFUNCTION(BlueprintCallable)
		void SetSensorTargetTransform(const int SensorTarget, const FVector Location);
	UFUNCTION(BlueprintCallable)
		void BindSensorTargetTransformToActor(const int SensorTarget, AActor* Actor);
	UFUNCTION(BlueprintCallable)
		void BindSensorTargetTransformToSkeletalMeshSocket(const int SensorTarget, USkeletalMeshComponent* SkeletalMeshComponent, const FName SocketName);
	UFUNCTION(BlueprintCallable)
		void DestroySensorTarget(const int SensorTarget);

	//Sensor Target Que
	std::queue<int> SensorTargetsAvailable;
	int SensorTargetCount;
	
	//Main
	bool SensorTargetJustCreateds[FLUID_AI_SENSORS_TARGETS_MAX];
	bool SensorTargetActives[FLUID_AI_SENSORS_TARGETS_MAX];
	bool SensorTargetSetToDeletes[FLUID_AI_SENSORS_TARGETS_MAX];

	//Name 
	FName SensorTargetNames[FLUID_AI_SENSORS_TARGETS_MAX];
	
	//Update
	float SensorTargetUpdateIntervals[FLUID_AI_SENSORS_TARGETS_MAX];
	float SensorTargetUpdateIntervalSteps[FLUID_AI_SENSORS_TARGETS_MAX];
	
	//Transform
	FVector SensorTargetTransformLocations[FLUID_AI_SENSORS_TARGETS_MAX];
	AActor* SensorTargetTransformActorBindings[FLUID_AI_SENSORS_TARGETS_MAX];
	USkeletalMeshComponent* SensorTargetTransformSkeletalMeshComponentBindings[FLUID_AI_SENSORS_TARGETS_MAX];
	FName SensorTargetTransformSocketNameBindings[FLUID_AI_SENSORS_TARGETS_MAX];
	
//Fluid AI Sensors Subsystem : Update Sensors
public:

	void UpdateSensorsStart(const float DeltaTime);
	void UpdateSensorsEnd(const float DeltaTime);
	void UpdateSensorsTransformBindings(const float DeltaTime);

//Fluid AI Sensors Subsystem : Update Sensor Targets
public:

	void UpdateSensorTargetsStart(const float DeltaTime);
	void UpdateSensorTargetsEnd(const float DeltaTime);
	void UpdateSensorTargetsTransformBindings(const float DeltaTime);


//Fluid AI Sensors Subsystem : Debug Sensors
public:

	void DebugSensorsRadiusAndAngleAll() const;
	void DebugSensorsRadiusAndAngleSelected();
	void DebugSensorsRadiusAndAngle(const TArray<int> Sensors) const;

	TArray<int> DebugSensors;

//Fluid AI Sensors Subsystem : Debug Sensor Targets
public:

	void DebugSensorTargetsAll() const;
};