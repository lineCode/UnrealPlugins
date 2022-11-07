#include "FluidToolboxActorGroups.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Actor Groups Blueprint Library : Main

void UFluidToolboxActorGroupsBPLibrary::AddActorGroup(const FName NewGroupID, bool& bDidAdd_Out)
{
	//Set Default Outs
	bDidAdd_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Add Actor Group
	if (!Subsystem->ActorGroups.Contains(NewGroupID))
	{
		Subsystem->ActorGroups.Add(NewGroupID,FFluidToolboxActorGroup());
		bDidAdd_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::RemoveActorGroup(const FName GroupID, bool& bDidRemove_Out)
{
	//Set Default Outs
	bDidRemove_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove Actor Group
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		Subsystem->ActorGroups.Remove(GroupID);
		bDidRemove_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::AddActorToActorGroup(const FName GroupID, AActor* Actor, bool bAddGroupIfNotAlreadyCreated, bool& bDidAdd_Out)
{
	//Set Default Outs
	bDidAdd_Out = false;
	
	//Create Actor Group Just In Case
	if (bAddGroupIfNotAlreadyCreated)
	{
		bool bDummy = false;
		AddActorGroup(GroupID, bDummy);
	}
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Add Actor To Group
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		Subsystem->ActorGroups[GroupID].Actors.Add(Actor);
		bDidAdd_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::AddUniqueActorToActorGroup(const FName GroupID, AActor* Actor, bool bAddGroupIfNotAlreadyCreated, bool& bDidAdd_Out)
{
	//Set Default Outs
	bDidAdd_Out = false;
	
	//Create Actor Group Just In Case
	if (bAddGroupIfNotAlreadyCreated)
	{
		bool bDummy = false;
		AddActorGroup(GroupID, bDummy);
	}
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Add Unique Actor To Group
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		Subsystem->ActorGroups[GroupID].Actors.AddUnique(Actor);
		bDidAdd_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::RemoveActorInActorGroup(const FName GroupID, AActor* Actor, bool& bDidRemove_Out)
{
	//Set Default Outs
	bDidRemove_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove Actor From Group
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		Subsystem->ActorGroups[GroupID].Actors.Remove(Actor);
		bDidRemove_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::RemoveActorsInActorGroup(const FName GroupID, bool& bDidRemove_Out)
{
	//Set Default Outs
	bDidRemove_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove Actor From Actor Group
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		Subsystem->ActorGroups[GroupID].Actors.Empty();
		bDidRemove_Out = true;
	}
}

void UFluidToolboxActorGroupsBPLibrary::DestroyActorsInActorGroup(const FName GroupID, bool& bDidDestroy_Out)
{
	//Set Default Outs
	bDidDestroy_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Destroy Actors In Group
	TArray<AActor*> Actors;
	if (Subsystem->ActorGroups.Contains(GroupID))
	{
		//Get Actors And Remove Them From Group
		Actors = Subsystem->ActorGroups[GroupID].Actors;
		Subsystem->ActorGroups[GroupID].Actors.Empty();

		//Destroy Actors
		UWorld* World = Subsystem->GetWorld();
		for (int i = 0; i < Actors.Num(); i++)
			World->DestroyActor(Actors[i]);

		bDidDestroy_Out = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Actor Groups Blueprint Library : Get

void UFluidToolboxActorGroupsBPLibrary::GetActorsInActorGroup(const FName GroupID, TArray<AActor*>& Actors_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove Actor From Group
	if (Subsystem->ActorGroups.Contains(GroupID))
		Actors_Out = Subsystem->ActorGroups[GroupID].Actors;
}

void UFluidToolboxActorGroupsBPLibrary::GetActorInActorGroupClosest(const FName GroupID, const FVector Location, const FVector Direction, const float MaxDistance, const float MinDot, AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out)
{
	//Null Out By Default
	Actor_Out = nullptr;
	ActorDistance_Out = -1;
	bActorValid_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Return If Actor Group Is Not Valid
	if (!Subsystem->ActorGroups.Contains(GroupID))
		return;
	auto Actors = Subsystem->ActorGroups[GroupID].Actors;
	
	//Get Closest Actor
	float ClosestDistance = -1;
	float ClosestDot = -1;
	float NewDistance = -1;
	float NewDot = -1;
	FVector DotOffset = FVector::ZeroVector;
	
	for (int i = 0; i < Actors.Num(); i++)
	{
		//Get Distance
		NewDistance = FVector::Dist(Location,Actors[i]->GetActorLocation());

		//Get Dot
		DotOffset =  Actors[i]->GetActorLocation() - Location;
		DotOffset = DotOffset.GetSafeNormal();
		NewDot = FVector::DotProduct(Direction, DotOffset);

		//Set Closest Pickup
		if (ClosestDistance == - 1 && NewDistance < MaxDistance && NewDot >= MinDot || NewDistance < ClosestDistance && NewDistance < MaxDistance && NewDot > ClosestDot && NewDot >= MinDot)
		{
			Actor_Out = Actors[i];
			ActorDistance_Out = ClosestDistance = NewDistance;
			ClosestDot = NewDot;
			bActorValid_Out = true;
		}
	}
}

void UFluidToolboxActorGroupsBPLibrary::GetActorInActorGroupLineTracedClosest(const FName GroupID, const FVector Location, const FVector Direction, const float MaxPickupDistance, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out)
{
	//Null Out By Default
	Actor_Out = nullptr;
	ActorDistance_Out = -1;
	bActorValid_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Return If Actor Group Is Not Valid
	if (!Subsystem->ActorGroups.Contains(GroupID))
		return;
	auto Actors = Subsystem->ActorGroups[GroupID].Actors;
	
	//Do Hit Trace
	TArray<AActor*> ActorsToIgnoire;
	FHitResult HitResult;
	const bool bDidHit = UKismetSystemLibrary::LineTraceSingle(Subsystem->GetWorld(),Location,Location + Direction * MaxPickupDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnoire, EDrawDebugTrace::None, HitResult, false, FLinearColor::Red, FLinearColor::Red, 0);
	
	//Return Data If Hit Is Valid
	if (bDidHit)
	{
		if (Actors.Contains(HitResult.GetActor()))
		{
			Actor_Out = HitResult.GetActor();
			ActorDistance_Out = HitResult.Distance;
			bActorValid_Out = true;
		}
	}
}

void UFluidToolboxActorGroupsBPLibrary::GetActorInActorGroupSphereTracedClosest(const FName GroupID, const FVector Location, const FVector Direction, const float MaxDistance, const float MinDot, const ETraceTypeQuery TraceTypeQuery, const bool bTraceComplex, AActor*& Actor_Out, float& ActorDistance_Out, bool& bActorValid_Out)
{
	//Null Out By Default
	Actor_Out = nullptr;
	ActorDistance_Out = -1;
	bActorValid_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxActorGroupsSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxActorGroupsSubSystem>();
	if (Subsystem == nullptr) return;

	//Return If Actor Group Is Not Valid
	if (!Subsystem->ActorGroups.Contains(GroupID))
		return;
	auto Actors = Subsystem->ActorGroups[GroupID].Actors;
	
	//Do Hit Trace
	TArray<AActor*> ActorsToIgnoire;
	TArray<FHitResult> HitResults;
	const bool bDidHit = UKismetSystemLibrary::SphereTraceMulti(Subsystem->GetWorld(),Location,Location,MaxDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnoire, EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, 0);

	//Get Closest Actor
	float ClosestDistance = -1;
	float ClosestDot = -1;
	float NewDot = -1;
	FVector DotOffset = FVector::ZeroVector;
	
	for (int i = 0; i < HitResults.Num(); i++)
	{
		//Get Dot
		DotOffset = HitResults[i].Location - Location;
		DotOffset = DotOffset.GetSafeNormal();
		NewDot = FVector::DotProduct(Direction, DotOffset);
		
		if (ClosestDistance == -1 && NewDot >= ClosestDot || HitResults[i].Distance < ClosestDistance && NewDot >= ClosestDot && Actors.Contains(HitResults[i].GetActor()))
		{
			Actor_Out = HitResults[i].GetActor();
			ActorDistance_Out = ClosestDistance = HitResults[i].Distance;
			ClosestDot = NewDot;
			bActorValid_Out = true;
		}
	}
}