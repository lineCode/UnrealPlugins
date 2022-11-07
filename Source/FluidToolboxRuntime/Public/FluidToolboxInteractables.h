
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Components/MeshComponent.h"
#include "Foliage/Public/FoliageInstancedStaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidToolboxInteractables.generated.h"

//Fluid Toolbox Interactables Return
USTRUCT(BlueprintType)
struct FFluidToolboxInteractable
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		AActor* Actor;
	UPROPERTY(BlueprintReadOnly)
		USceneComponent* SceneComponent;
	UPROPERTY(BlueprintReadOnly)
		UPrimitiveComponent* PrimitiveComponent;
	UPROPERTY(BlueprintReadOnly)
		UFoliageInstancedStaticMeshComponent* FoliageInstancedStaticMeshComponent;
	UPROPERTY(BlueprintReadOnly)
		FName GroupID;
	UPROPERTY(BlueprintReadOnly)
		FName Type;
	UPROPERTY(BlueprintReadOnly)
		int InteractableIndex;
	UPROPERTY(BlueprintReadOnly)
		int InstancedElementIndex;
};

//Fluid Toolbox Interactables Group
USTRUCT()
struct FFluidToolboxInteractablesGroup
{
	GENERATED_BODY()

	ECollisionChannel CollisionChannel;
	ETraceTypeQuery TraceTypeQeury;
	TArray<USceneComponent*> SceneComponents;
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	TArray<UFoliageInstancedStaticMeshComponent*> FoliageInstancedStaticMeshComponents;
	TArray<FName> Types;
};

//Fluid Toolbox Interactables Notification Receiver Interface
UINTERFACE(BlueprintType)
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInteractableBroadcastListenerInterface : public UInterface
{
	GENERATED_BODY()
};
class FLUIDTOOLBOXRUNTIME_API IFluidToolboxInteractableBroadcastListenerInterface
{
	GENERATED_BODY()
	public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FluidToolboxInteractableBroadcastListnerInterface")
		int32 OnActorInteractableBroadcastReceived(AActor* CallerActor, const FFluidToolboxInteractable Interactable, const bool bIsPlayer);
};

//Fluid Toolbox Interactables Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInteractablesGroupsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Fluid Toolbox Interactables Blueprint Library : Interactable Groups
public:

	UFUNCTION(BlueprintCallable)
		static void AddInteractableGroup(const FName NewGroupID, ECollisionChannel CollisionChannel, ETraceTypeQuery TraceTypeQeury, bool& bDidAdd_Out);

//Fluid Toolbox Interactables Blueprint Library : Interactables As Actors
public:
	
	UFUNCTION(BlueprintCallable)
		static void AddInteractableToInteractableGroup(const FName GroupID, const FName Type, USceneComponent* SceneComponent, bool& bDidAdd_Out);
	UFUNCTION(BlueprintCallable)
		static void RemoveInteractableInInteractableGroup(const FName GroupID, USceneComponent* SceneComponent, bool& bDidRemove_Out);
	UFUNCTION(BlueprintCallable)
		static void RemoveInteractablesInInteractableGroup(const FName GroupID, bool& bDidRemove_Out);

//Fluid Toolbox Interactables Blueprint Library : Interactable Broadcast
public:

	UFUNCTION(BlueprintCallable)
		static void DoInteractableBroadcast(AActor* CallerActor, const FFluidToolboxInteractable Interactable, const bool bIsPlayer);
	
//Fluid Toolbox Interactables Blueprint Library : Get Interactables
public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetInteractableInInteractableGroupClosestLineTraced(const FName GroupID, const FVector TraceLocation, const FVector ViewDirection, const float MaxDistance, const bool bTraceComplex, TArray<AActor*> ActorsToIgnore, FFluidToolboxInteractable& Interactable_Out, bool& bIsInteractableValid_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetInteractableInInteractableGroupClosestSphereTraced(const FName GroupID, const FVector TraceLocation, const FVector TraceDirection, const FVector ViewLocation, const FVector ViewDirection, const float TraceMinDistance, const float TraceMaxDistance, const float TraceMinDot, const bool bTraceComplex,TArray<AActor*> ActorsToIgnore, FFluidToolboxInteractable& Interactable_Out, bool& bIsInteractableValid_Out);
};

//Fluid Toolbox Interactables SubSystem (To Hold Data)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxInteractablesSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	TMap<FName,FFluidToolboxInteractablesGroup> InteractableGroups;
};