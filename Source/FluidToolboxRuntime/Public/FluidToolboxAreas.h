// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationSystem.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "FluidToolboxAreas.generated.h"

//Fluid Area Spawner : Simple Navigable Radius
UCLASS()
class FLUIDTOOLBOXRUNTIME_API AFluidToolboxArea : public AActor
{
	GENERATED_BODY()

public:
	
	AFluidToolboxArea();
	virtual void BeginPlay() override;

	FVector Scale;
};
