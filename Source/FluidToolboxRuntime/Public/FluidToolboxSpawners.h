// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "FluidToolboxSpawners.generated.h"

USTRUCT(BlueprintType)
struct FLUIDTOOLBOXRUNTIME_API FFluidToolboxSpawnerSpawn
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> SpawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SpawnCountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int SpawnCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnRadius;
};

//Fluid Toolbox Spawner : Simple Navigable Radius
UCLASS()
class FLUIDTOOLBOXRUNTIME_API AFluidToolboxSpawnerSimpleNavigableRadius : public AActor
{
	GENERATED_BODY()

public:

	AFluidToolboxSpawnerSimpleNavigableRadius();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FFluidToolboxSpawnerSpawn> Spawns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDisableAllSpawns;
};
