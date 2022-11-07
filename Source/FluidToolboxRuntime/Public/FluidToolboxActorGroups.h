
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidToolboxActorGroups.generated.h"

//Fluid Toolbox Actor Group
USTRUCT()
struct FFluidToolboxActorGroup
{
	GENERATED_BODY()

	TArray<AActor*> Actors;
};

//Fluid Toolbox Actor Groups Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxActorGroupsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Fluid Toolbox Actor Groups Blueprint Library : Main
public:

	UFUNCTION(BlueprintCallable)
		static void AddActorGroup(const FName NewGroupID, bool& bAddRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void RemoveActorGroup(const FName GroupID, bool& bAddRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void AddActorToActorGroup(const FName GroupID, AActor* Actor, bool bAddGroupIfNotAlreadyCreated, bool& bAddRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void AddUniqueActorToActorGroup(const FName GroupID, AActor* Actor, bool bAddGroupIfNotAlreadyCreated, bool& bAddRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void RemoveActorInActorGroup(const FName GroupID, AActor* Actor, bool& bDidRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void RemoveActorsInActorGroup(const FName GroupID, bool& bDidRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void DestroyActorsInActorGroup(const FName GroupID, bool& bDestroyRemove_Out);
	
//Fluid Toolbox Actor Groups Blueprint Library : Get
public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetActorsInActorGroup(const FName GroupID, TArray<AActor*>& Actors_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetActorInActorGroupClosest(const FName GroupID, const FVector Location, const FVector Direction, const float MaxDistance, const float MinDot, AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetActorInActorGroupLineTracedClosest(const FName GroupID, const FVector Location,const FVector Direction, const float MaxPickupDistance, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex,AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetActorInActorGroupSphereTracedClosest(const FName GroupID, const FVector Location, const FVector Direction, const float MaxDistance, const float MinDot, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out);
};

//Fluid Toolbox Actors SubSystem (To Hold Data)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxActorGroupsSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	TMap<FName,FFluidToolboxActorGroup> ActorGroups;
};