
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidToolboxHitTrace.generated.h"


//Fluid Entities Character Hybrid Stat Group
DECLARE_STATS_GROUP(TEXT("Fluid Toolbox Hit Trace"), STATGROUP_FluidToolboxHitTraces, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Fluid Toolbox Hit Traces"),STAT_FluidToolboxHitTraces_Interaction,STATGROUP_FluidToolboxHitTraces);

UINTERFACE(BlueprintType)
class FLUIDTOOLBOXRUNTIME_API UFluidEntitiesCharacterHybridInteractionEvent : public UInterface
{
	GENERATED_BODY()
};

class FLUIDTOOLBOXRUNTIME_API IFluidEntitiesCharacterHybridInteractionEvent
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnFluidEntitiesCharacterInteractionEvent(const int Entity, FHitResult HitResult);
};

struct FFluidEntitiesCharacterHybridInteractionTask
{
	ETraceTypeQuery TraceTypeQuery;
	USceneComponent* SceneComponent;
	FName SocketName;
	FVector Extent;
	FVector LastFrameLocation;
	FRotator LastFrameRotation;
	int Entity;
	int SubSteps;
	int ShapeType;
	bool bDidFirstFrame;
	bool bContinuous;
	bool bIsComplex;
};

//Fluid Toolbox Actors SubSystem (To Hold Data)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxHitTraceSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//Fluid Entities Character Hybrid Entities Interaction Task
	public:

	void EntitiesCharacterInteractionSystem();
	//UFUNCTION(BlueprintCallable)
	//	void AddInteractionTaskByNotify(USceneComponent* SceneComponent, FString String,int Entity);
	UFUNCTION(BlueprintCallable)
		void AddInteractionTask(USceneComponent* SceneComponent,FName SocketName, ETraceTypeQuery TraceTypeQuery, int SubSteps, int Entity,bool bContinuous, bool bIsComplex);
	UFUNCTION(BlueprintCallable)
		void RemoveInteractionTask(USceneComponent* SceneComponent,FName SocketName);
	//UFUNCTION(BlueprintCallable)
	//	void ParseInteractionNotify(FString String, FName& SocketName_Out, int& SubSteps_Out, bool& bIsValid_Out);
	
	TArray<FFluidEntitiesCharacterHybridInteractionTask> InteractionTasks;
};