
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/MeshComponent.h"
#include "Foliage/Public/FoliageInstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidToolboxController.generated.h"

class UFluidToolboxControllerSubsystem;

//Fluid Toolbox Controller Player Interface
UINTERFACE(BlueprintType)
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxControllerPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class FLUIDTOOLBOXRUNTIME_API IFluidToolboxControllerPlayerInterface
{
	GENERATED_BODY()
	
	public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerPlayerInterface")
		int OnFluidToolboxControllerPlayerBegin(AActor* PlayerActor, int LocalPlayerIndex, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerPlayerInterface")
		int OnFluidToolboxControllerPlayerEnd(AActor* PlayerActor, int LocalPlayerIndex, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerPlayerInterface")
		int OnFluidToolboxControllerPlayerTick(AActor* PlayerActor, int LocalPlayerIndex, float DeltaTime, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
};

//Fluid Toolbox Controller Mount Interface
UINTERFACE(BlueprintType)
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxControllerMountInterface : public UInterface
{
	GENERATED_BODY()
};

class FLUIDTOOLBOXRUNTIME_API IFluidToolboxControllerMountInterface
{
	GENERATED_BODY()
	
	public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerMountInterface")
		void OnFluidToolboxControllerMountMountedBegin(AActor* Owner, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerMountInterface")
		void OnFluidToolboxControllerMountMountedEnd(AActor* Owner, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxControllerMountInterface")
		void OnFluidToolboxControllerMountMountedInputAxis(AActor* Owner, FName InputType, float X, float Y, float DeltaTime, bool& bBoolToForceEventToFunctionIgnoreThis_Out);
};

//Fluid Toolbox Controller Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxControllerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		static void AddPlayerController(AActor* Actor, int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		static void RemovePlayerControllerByActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
		static void RemovePlayerControllerByLocalPlayerIndex(int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		static void AddMountController(AActor* Owner, AActor* Mount);
	UFUNCTION(BlueprintCallable)
		static void RemoveMountController(AActor* Owner);
	UFUNCTION(BlueprintCallable)
		static void ToggleMountController(AActor* Owner, AActor* Mount);
	UFUNCTION(BlueprintCallable)
		static void GetMountController(AActor* Owner, AActor*& Mount_Out);
};

//Fluid Toolbox Controller SubSystem
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxControllerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Toolbox Controller SubSystem
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	TMap<AActor*,AActor*> Mounts;
	TArray<AActor*> PlayerActors;
	TArray<int> PlayerLocalIndexs;
	TArray<bool> PlayerInitializeds;
	bool bInitialized;
};