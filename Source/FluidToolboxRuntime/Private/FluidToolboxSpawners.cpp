// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidToolboxSpawners.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Spawner : Simple Navigable Radius

AFluidToolboxSpawnerSimpleNavigableRadius::AFluidToolboxSpawnerSimpleNavigableRadius()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFluidToolboxSpawnerSimpleNavigableRadius::BeginPlay()
{
	Super::BeginPlay();

	//Not Valid Spawn
	if (bDisableAllSpawns || Spawns.Num() <= 0)
		return;

	//Spawn Data
	UWorld* World = GetWorld();
	FVector Location;
	FRotator Rotation = FRotator::ZeroRotator;

	//Create Spawn Parameters So We Always Spawn
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//Spawn
	for (int j = 0; j < Spawns.Num(); j++)
		{
		const auto Spawn = Spawns[j];
		const auto SpawnCount = UKismetMathLibrary::RandomIntegerInRange(Spawn.SpawnCountMin,Spawn.SpawnCountMax);
		
		for (int i = 0; i < SpawnCount; i++)
		{
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(World,GetActorLocation(),Location,Spawn.SpawnRadius);

			//Spawn Actor
			if (Spawn.SpawnActor)
				World->SpawnActor<AActor>(Spawn.SpawnActor,Location,Rotation,SpawnParameters);
		}
	}
}