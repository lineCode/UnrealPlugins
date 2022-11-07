#include "FluidToolboxInteractables.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Interactables Blueprint Library : Interactable Groups

void UFluidToolboxInteractablesGroupsBPLibrary::AddInteractableGroup(const FName NewGroupID, ECollisionChannel CollisionChannel, ETraceTypeQuery TraceTypeQeury, bool& bDidAdd_Out)
{
	//Default Outs
	bDidAdd_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;

	//Add Interactable Group
	if (!Subsystem->InteractableGroups.Contains(NewGroupID))
	{
		Subsystem->InteractableGroups.Add(NewGroupID,FFluidToolboxInteractablesGroup());
		Subsystem->InteractableGroups[NewGroupID].CollisionChannel = CollisionChannel;
		Subsystem->InteractableGroups[NewGroupID].TraceTypeQeury = TraceTypeQeury;
		bDidAdd_Out = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Interactables Blueprint Library : Interactables

void UFluidToolboxInteractablesGroupsBPLibrary::AddInteractableToInteractableGroup(const FName GroupID, const FName Type, USceneComponent* SceneComponent, bool& bDidAdd_Out)
{
	//Default Outs
	bDidAdd_Out = false;

	//Do Not Add Interactable If Actor Or Static Mesh Component Is Null
	if (SceneComponent == nullptr)
		return;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;
	
	//Add Actor To Interactable Group
	if (Subsystem->InteractableGroups.Contains(GroupID) && !Subsystem->InteractableGroups[GroupID].SceneComponents.Contains(SceneComponent))
	{
		//Add As Primitive Component
		if (SceneComponent->IsA(UPrimitiveComponent::StaticClass()))
		{
			UPrimitiveComponent* Component = (UPrimitiveComponent*)SceneComponent;
			Component->SetCollisionResponseToChannel(Subsystem->InteractableGroups[GroupID].CollisionChannel,ECollisionResponse::ECR_Overlap);
			Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Add(Component);
			bDidAdd_Out = true;
		}
		else Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Add(nullptr);

		//Add As Foliage Instanced Static Mesh Component
		if (SceneComponent->IsA(UFoliageInstancedStaticMeshComponent::StaticClass()))
		{
			UFoliageInstancedStaticMeshComponent* Component = (UFoliageInstancedStaticMeshComponent*)SceneComponent;
			Component->SetCollisionResponseToChannel(Subsystem->InteractableGroups[GroupID].CollisionChannel,ECollisionResponse::ECR_Overlap);
			Subsystem->InteractableGroups[GroupID].FoliageInstancedStaticMeshComponents.Add(Component);
			bDidAdd_Out = true;
		}
		else Subsystem->InteractableGroups[GroupID].FoliageInstancedStaticMeshComponents.Add(nullptr);

		//Add Scene Component
		{
			Subsystem->InteractableGroups[GroupID].SceneComponents.Add(SceneComponent);
			Subsystem->InteractableGroups[GroupID].Types.Add(Type);
			bDidAdd_Out = true;
		}
	}
}

void UFluidToolboxInteractablesGroupsBPLibrary::RemoveInteractableInInteractableGroup(const FName GroupID, USceneComponent* SceneComponent, bool& bDidRemove_Out)
{
	//Default Outs
	bDidRemove_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove Actor From Interactable Group
	if (Subsystem->InteractableGroups.Contains(GroupID))
	{
		const int Index = Subsystem->InteractableGroups[GroupID].SceneComponents.Find(SceneComponent);

		//Add Collision Response Channel To Group
		Subsystem->InteractableGroups[GroupID].PrimitiveComponents[Index]->SetCollisionResponseToChannel(Subsystem->InteractableGroups[GroupID].CollisionChannel,ECollisionResponse::ECR_Ignore);

		Subsystem->InteractableGroups[GroupID].SceneComponents.RemoveAt(Index);
		Subsystem->InteractableGroups[GroupID].PrimitiveComponents.RemoveAt(Index);
		Subsystem->InteractableGroups[GroupID].Types.RemoveAt(Index);
		bDidRemove_Out = true;
	}
}

void UFluidToolboxInteractablesGroupsBPLibrary::RemoveInteractablesInInteractableGroup(const FName GroupID, bool& bDidRemove_Out)
{
	//Default Outs
	bDidRemove_Out = false;
	
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;

	//Remove From Interactable Group
	if (Subsystem->InteractableGroups.Contains(GroupID))
	{
		Subsystem->InteractableGroups[GroupID].SceneComponents.Empty();
		Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Empty();
		Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Empty();
		Subsystem->InteractableGroups[GroupID].Types.Empty();
		bDidRemove_Out = true;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Interactables Blueprint Library : Interactable Broadcast

void UFluidToolboxInteractablesGroupsBPLibrary::DoInteractableBroadcast(AActor* CallerActor, const FFluidToolboxInteractable Interactable, const bool bIsPlayer)
{
	//Send To Actor
	AActor* Actor = Interactable.Actor;
	if (Actor != nullptr)
	{
		if (Actor->GetClass()->ImplementsInterface(UFluidToolboxInteractableBroadcastListenerInterface::StaticClass()))
		{
			//Cast<IFluidToolboxInteractableBroadcastListenerInterface>(Interactable.Actor)->OnActorInteractableBroadcastReceived(Interactable); //C++
			IFluidToolboxInteractableBroadcastListenerInterface::Execute_OnActorInteractableBroadcastReceived(Actor,CallerActor, Interactable, bIsPlayer); // Blueprints;
		}
	}

	//Send To Parent Actor
	AActor* ActorParent = Interactable.Actor->GetParentActor();
	if (ActorParent != nullptr)
	{
		if (ActorParent->GetClass()->ImplementsInterface(UFluidToolboxInteractableBroadcastListenerInterface::StaticClass()))
		{
			//Cast<IFluidToolboxInteractableBroadcastListenerInterface>(Interactable.Actor)->OnActorInteractableBroadcastReceived(Interactable); //C++
			IFluidToolboxInteractableBroadcastListenerInterface::Execute_OnActorInteractableBroadcastReceived(ActorParent,CallerActor, Interactable, bIsPlayer); // Blueprints;
		}
	}
	
	//Send To Attached Parent Actor
	AActor* ActorAttachedParent = Interactable.Actor->GetAttachParentActor();
	if (ActorAttachedParent != nullptr)
	{
		if (ActorAttachedParent->GetClass()->ImplementsInterface(UFluidToolboxInteractableBroadcastListenerInterface::StaticClass()))
		{
			//Cast<IFluidToolboxInteractableBroadcastListenerInterface>(Interactable.Actor)->OnActorInteractableBroadcastReceived(Interactable); //C++
			IFluidToolboxInteractableBroadcastListenerInterface::Execute_OnActorInteractableBroadcastReceived(ActorAttachedParent,CallerActor, Interactable, bIsPlayer); // Blueprints;
		}
	}

	//Send To Attached Scene Component
	USceneComponent* SceneComponent = Interactable.SceneComponent;
	if (SceneComponent != nullptr)
	{
		if (SceneComponent->GetClass()->ImplementsInterface(UFluidToolboxInteractableBroadcastListenerInterface::StaticClass()))
		{
			//Cast<IFluidToolboxInteractableBroadcastListenerInterface>(Interactable.Actor)->OnActorInteractableBroadcastReceived(Interactable); //C++
			IFluidToolboxInteractableBroadcastListenerInterface::Execute_OnActorInteractableBroadcastReceived(SceneComponent,CallerActor, Interactable, bIsPlayer); // Blueprints;
		}
	}

	//Send To Attached Primitive Component
	UPrimitiveComponent* PrimitiveComponent = Interactable.PrimitiveComponent;
	if (PrimitiveComponent != nullptr)
	{
		if (PrimitiveComponent->GetClass()->ImplementsInterface(UFluidToolboxInteractableBroadcastListenerInterface::StaticClass()))
		{
			//Cast<IFluidToolboxInteractableBroadcastListenerInterface>(Interactable.Actor)->OnActorInteractableBroadcastReceived(Interactable); //C++
			IFluidToolboxInteractableBroadcastListenerInterface::Execute_OnActorInteractableBroadcastReceived(PrimitiveComponent,CallerActor, Interactable, bIsPlayer); // Blueprints;
		}
	}
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Interactables Blueprint Library : Get Interactables

void UFluidToolboxInteractablesGroupsBPLibrary::GetInteractableInInteractableGroupClosestLineTraced(const FName GroupID, const FVector TraceLocation,const FVector ViewDirection, const float MaxDistance, const bool bTraceComplex, TArray<AActor*> ActorsToIgnore, FFluidToolboxInteractable& Interactable_Out, bool& bIsInteractableValid_Out)
{
	//Default Outs
	Interactable_Out = FFluidToolboxInteractable();
	Interactable_Out.InteractableIndex = -1;
	Interactable_Out.InstancedElementIndex = -1;
	bIsInteractableValid_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;

	//Return If Actor Group Is Not Valid
	if (!Subsystem->InteractableGroups.Contains(GroupID))
		return;
	TArray<UPrimitiveComponent*>* PrimitiveComponents = &Subsystem->InteractableGroups[GroupID].PrimitiveComponents;
	ETraceTypeQuery TraceTypeQuery = Subsystem->InteractableGroups[GroupID].TraceTypeQeury;
	
	//Do Hit Trace
	FHitResult HitResult;
	const bool bDidHit = UKismetSystemLibrary::LineTraceSingle(Subsystem->GetWorld(),TraceLocation,TraceLocation + ViewDirection * MaxDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, HitResult, false, FLinearColor::Red, FLinearColor::Red, 0);
	
	//Return Data If Hit Is Valid
	if (bDidHit)
	{
		if (PrimitiveComponents->Contains(HitResult.GetComponent()))
		{
			//Interactable Out
			const int Index = Interactable_Out.InteractableIndex = Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Find(HitResult.GetComponent());
			Interactable_Out.GroupID = GroupID;
			Interactable_Out.Type = Subsystem->InteractableGroups[GroupID].Types[Index];
			Interactable_Out.Actor = Subsystem->InteractableGroups[GroupID].SceneComponents[Index]->GetOwner();
			Interactable_Out.SceneComponent = HitResult.GetComponent();
			Interactable_Out.PrimitiveComponent = Subsystem->InteractableGroups[GroupID].PrimitiveComponents[Index];
			Interactable_Out.FoliageInstancedStaticMeshComponent = Subsystem->InteractableGroups[GroupID].FoliageInstancedStaticMeshComponents[Index];
			Interactable_Out.InstancedElementIndex = HitResult.Item;
		}
	}
}

void UFluidToolboxInteractablesGroupsBPLibrary::GetInteractableInInteractableGroupClosestSphereTraced(const FName GroupID, const FVector TraceLocation, const FVector TraceDirection, const FVector ViewLocation, const FVector ViewDirection, const float TraceMinDistance, const float TraceMaxDistance, const float TraceMinDot, const bool bTraceComplex, TArray<AActor*> ActorsToIgnore, FFluidToolboxInteractable& Interactable_Out, bool& bIsInteractableValid_Out)
{    
	//Default Outs
	Interactable_Out = FFluidToolboxInteractable();
	Interactable_Out.InteractableIndex = -1;
	Interactable_Out.InstancedElementIndex = -1;
	bIsInteractableValid_Out = false;

	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxInteractablesSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxInteractablesSubSystem>();
	if (Subsystem == nullptr) return;

	//Return If Actor Group Is Not Valid
	if (!Subsystem->InteractableGroups.Contains(GroupID))
		return;
	ETraceTypeQuery TraceTypeQuery = Subsystem->InteractableGroups[GroupID].TraceTypeQeury;
	
	//Do Hit Trace | Remove Hits That Under Min Trace Distance
	TArray<FHitResult> HitResults;
	const bool bDidHit = UKismetSystemLibrary::SphereTraceMulti(Subsystem->GetWorld(),TraceLocation,TraceLocation,TraceMaxDistance,TraceTypeQuery, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, 0);

	//Remove Hit Results That Don't Meet Min Distance
	for (int i = HitResults.Num(); i--; )
		if (HitResults[i].Distance < TraceMinDistance)
			HitResults.RemoveAt(i);
	
	//Get Best Interactable By Distance And Dot
	float CurrentScore = -1;
	for (int i = 0; i < HitResults.Num(); i++)
	{
		//Calculate Difference 
		const FVector DifferenceFromView = HitResults[i].GetActor()->GetActorLocation() - ViewLocation;
		const FVector DifferenceFromTrace = HitResults[i].GetActor()->GetActorLocation() - TraceLocation;

		// Calculate Direction
		const FVector DirectionFromView = (DifferenceFromView).GetSafeNormal();
		const FVector DirectionFromTrance = (DifferenceFromTrace).GetSafeNormal();
		
		//Get Dot Product
		const float DotFromView =  FVector::DotProduct(ViewDirection, DirectionFromView );
		const float DotFromTrace =  FVector::DotProduct(TraceDirection, DirectionFromTrance);
		if (DotFromTrace < TraceMinDot)
			continue;
		
		//Get Distance 
		float DistanceFromView = DifferenceFromView.Size();
		if (DistanceFromView < TraceMinDistance) DistanceFromView = 1;
		if (DistanceFromView > TraceMaxDistance) DistanceFromView = 0;

		//Get Score
		const float Score = (DotFromView + DistanceFromView);
		
		//Set Actor As Out If Score Is Better
		if (Score > CurrentScore && Subsystem->InteractableGroups[GroupID].SceneComponents.Contains(HitResults[i].GetComponent()))
		{
			//Set New Score
			CurrentScore  = Score;
			
			//Interactable Out
			const int Index = Interactable_Out.InteractableIndex = Subsystem->InteractableGroups[GroupID].PrimitiveComponents.Find(HitResults[i].GetComponent());
			Interactable_Out.GroupID = GroupID;
			Interactable_Out.Type = Subsystem->InteractableGroups[GroupID].Types[Index];
			Interactable_Out.Actor = Subsystem->InteractableGroups[GroupID].SceneComponents[Index]->GetOwner();
			Interactable_Out.SceneComponent = HitResults[i].GetComponent();
			Interactable_Out.PrimitiveComponent = Subsystem->InteractableGroups[GroupID].PrimitiveComponents[Index];
			Interactable_Out.FoliageInstancedStaticMeshComponent = Subsystem->InteractableGroups[GroupID].FoliageInstancedStaticMeshComponents[Index];
			Interactable_Out.InstancedElementIndex = HitResults[i].Item;
			bIsInteractableValid_Out = true;
		}
	}
}