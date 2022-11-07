#include "FluidToolboxHitTrace.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Camera Blueprint Library : Camera Main (Clean Wrapper For Subsystem For Blueprint Users)
/// 
static TAutoConsoleVariable<float> CDebugFluidEntitiesCharacterHybridInteractionTraces(
	TEXT("Fluid.Entities.Character.Hybrid.Interaction.Trace.Debug"),
	0,
	TEXT("Fluid.Entities.Character.Hybrid.Interaction.Trace.Debug\n"),
	ECVF_SetByConsole);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Camera Blueprint Library : Camera Main (Clean Wrapper For Subsystem For Blueprint Users)

void UFluidToolboxHitTraceSubSystem::EntitiesCharacterInteractionSystem()
{
	//Get Stats For This Code
	SCOPE_CYCLE_COUNTER(STAT_FluidToolboxHitTraces_Interaction);
	{
		//Trace Variables
		TArray<AActor*> ActorsToIgnoire;
		TArray<FHitResult> HitResults;
		bool bDidHit = false;
    
		//Frame Variables
		FVector FrameLocation;
		FRotator FrameRotation;
    
		//Substep Variables
		FVector SubStepLocation;
		FRotator SubStepRotation;
		float SubStepAlpha;
		//Do Hit Tasks
		for (int i = InteractionTasks.Num() - 1; i >= 0; i--)
		{
			//Get Frame Data
			FrameLocation = InteractionTasks[i].SceneComponent->GetSocketLocation(InteractionTasks[i].SocketName);
			FrameRotation = InteractionTasks[i].SceneComponent->GetSocketRotation(InteractionTasks[i].SocketName);
   
			//Substep
			if (InteractionTasks[i].SubSteps > 0)
			{
				//Do Substeps
				for (float j = 0; j < InteractionTasks[i].SubSteps && j <= 12; j++)
				{
					//Substep
					SubStepAlpha = j / InteractionTasks[i].SubSteps;
					SubStepLocation = FMath::LerpStable(InteractionTasks[i].LastFrameLocation, FrameLocation, SubStepAlpha);
					SubStepRotation = FMath::LerpStable(InteractionTasks[i].LastFrameRotation, FrameRotation, SubStepAlpha);
            
					//Box Trace
					if (InteractionTasks[i].ShapeType == 1)
						bDidHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(),SubStepLocation,SubStepLocation, InteractionTasks[i].Extent,SubStepRotation, InteractionTasks[i].TraceTypeQuery, InteractionTasks[i].bIsComplex, ActorsToIgnoire, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread() > 0 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread());
                
					//Sphere Trace
					if (InteractionTasks[i].ShapeType == 2)
						bDidHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),SubStepLocation,SubStepLocation, InteractionTasks[i].Extent.X,InteractionTasks[i].TraceTypeQuery, InteractionTasks[i].bIsComplex, ActorsToIgnoire, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread() > 0 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread());
				}
        
				//Set Last Frame Data
				InteractionTasks[i].LastFrameLocation = FrameLocation;
				InteractionTasks[i].LastFrameRotation = FrameRotation;
				InteractionTasks[i].bDidFirstFrame = true;
			}
        
			//Single Frame
			else
			{
				//Box Trace
				if (InteractionTasks[i].ShapeType == 1)
					bDidHit = UKismetSystemLibrary::BoxTraceMulti(GetWorld(),FrameLocation,FrameLocation, InteractionTasks[i].Extent,FrameRotation, InteractionTasks[i].TraceTypeQuery, InteractionTasks[i].bIsComplex, ActorsToIgnoire, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread() > 0 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread());
            
				//Sphere Trace
				if (InteractionTasks[i].ShapeType == 2)
					bDidHit = UKismetSystemLibrary::SphereTraceMulti(GetWorld(),FrameLocation,FrameLocation, InteractionTasks[i].Extent.X, InteractionTasks[i].TraceTypeQuery, InteractionTasks[i].bIsComplex, ActorsToIgnoire, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread() > 0 ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, HitResults, false, FLinearColor::Red, FLinearColor::Red, CDebugFluidEntitiesCharacterHybridInteractionTraces.GetValueOnAnyThread());
			}

			//If Hit, Send Event To Interaction Graph
			if (bDidHit)
				for (int j = 0; j < HitResults.Num(); j++)
				{
					if (HitResults[j].GetActor()->GetClass()->ImplementsInterface(UFluidEntitiesCharacterHybridInteractionEvent::StaticClass()))
					{
						//Cast<IFluidEntitiesCharacterInteractionEvent>(HitResults[j].GetActor())->OnFluidEntitiesCharacterInteractionEvent(InteractionTasks[i].Entity,HitResults[j]);
						IFluidEntitiesCharacterHybridInteractionEvent::Execute_OnFluidEntitiesCharacterInteractionEvent(HitResults[j].GetActor(),InteractionTasks[i].Entity,HitResults[j]);
					}
					else if (HitResults[j].GetComponent()->GetClass()->ImplementsInterface(UFluidEntitiesCharacterHybridInteractionEvent::StaticClass()))
					{
						//Cast<IFluidEntitiesCharacterInteractionEvent>(HitResults[j].GetComponent())->OnFluidEntitiesCharacterInteractionEvent(InteractionTasks[i].Entity,HitResults[j]);
						IFluidEntitiesCharacterHybridInteractionEvent::Execute_OnFluidEntitiesCharacterInteractionEvent(HitResults[j].GetComponent(),InteractionTasks[i].Entity,HitResults[j]);
					}
				

				}

			//Remove Interaction Task
			if (!InteractionTasks[i].bContinuous)
				InteractionTasks.RemoveAt(i);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Entities Character Hybrid Entities Interaction Tasks

/*void UFluidEntitiesCharacterSubsystem::AddInteractionTaskByNotify(USceneComponent* SceneComponent, FString String, int Entity)
{
    //Default Variables
    FName SocketName;
    int SubSteps;
    bool bIsValid;
    
    //Parse The Notify
    ParseInteractionNotify(String, SocketName,SubSteps,bIsValid);
    
    //If Valid, Add It As Taks
    if (bIsValid)
        AddInteractionTask(SceneComponent,SocketName,SubSteps);
}*/

void UFluidToolboxHitTraceSubSystem::AddInteractionTask(USceneComponent* SceneComponent,FName SocketName, ETraceTypeQuery TraceTypeQuery, int SubSteps, int Entity,bool bContinuous, bool bIsComplex)
{
    //Return If Not Valid Scene Component
    if (SceneComponent == nullptr)
        return;
    
    //Remove Same Task If Already Exists
    RemoveInteractionTask(SceneComponent,SocketName);
	
    //Create Hit Task If Valid Socket
    if (SceneComponent->DoesSocketExist(SocketName))
    {
        //Universal Properties
        FFluidEntitiesCharacterHybridInteractionTask InteractionTask = FFluidEntitiesCharacterHybridInteractionTask();
        InteractionTask.SceneComponent = SceneComponent;
        InteractionTask.SubSteps = SubSteps;
        InteractionTask.SocketName = SocketName;
		InteractionTask.Entity = Entity;
    	InteractionTask.TraceTypeQuery = TraceTypeQuery;
    	InteractionTask.bContinuous = bContinuous;
    	InteractionTask.bIsComplex = bIsComplex;
    	
        //Get Socket Scale
        FVector SocketScale = SceneComponent->GetSocketTransform(SocketName,ERelativeTransformSpace::RTS_World).GetScale3D() * 5;
        
        //As Box
        if (SocketScale.Y > 0 && SocketScale.Z > 0)
        {
            InteractionTask.ShapeType = 1;
            InteractionTask.Extent = FVector(SocketScale.X,SocketScale.Y,SocketScale.Z);
        }
        
        //As Sphere
        else
        {
            InteractionTask.ShapeType = 2;
            InteractionTask.Extent = FVector(SocketScale.X,SocketScale.X,SocketScale.X);
        }
        
        //Add It !!!!
        InteractionTasks.Add(InteractionTask);
    }
}

void UFluidToolboxHitTraceSubSystem::RemoveInteractionTask(USceneComponent* SceneComponent,FName SocketName)
{
    for (int i = 0; i < InteractionTasks.Num(); i ++)
    {
        if (InteractionTasks[i].SceneComponent == SceneComponent && InteractionTasks[i].SocketName == SocketName)
            InteractionTasks.RemoveAt(i);
            return;
    }
}

/*void UFluidEntitiesCharacterSubsystem::ParseInteractionNotify(FString String, FName& SocketName_Out, int& SubSteps_Out, bool& bIsValid_Out)
{
    //Defaults
    SocketName_Out = "";
    SubSteps_Out = 0;
    bIsValid_Out = false;
    
    //Parse String Into Array
    TArray<FString> Parsed;
    String.ParseIntoArray(Parsed, TEXT(","), true);
    
    //Check Is Valid
    if (!String.Contains("hit",ESearchCase::IgnoreCase) || Parsed.Num() < 3)
        return;
    
    //Set Data
    SocketName_Out = FName(*Parsed[1]);
    SubSteps_Out = FCString::Atoi(*Parsed[2]);
    bIsValid_Out = true;
}*/