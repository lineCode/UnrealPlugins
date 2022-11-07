#include "FluidToolboxController.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fluid Toolbox Controller Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)

void UFluidToolboxControllerBPLibrary::AddPlayerController(AActor* Actor, int LocalPlayerIndex)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Add If Parameters Are Is Invalid
	if (Actor == nullptr || LocalPlayerIndex < 0 || LocalPlayerIndex  > 7)
		return;

	//Don't Add If Actor Does Not Implement Interface 
	if (!Actor->GetClass()->ImplementsInterface(UFluidToolboxControllerPlayerInterface::StaticClass()))
		return;
	
	//Don't Add If Already Added
	if (Subsystem->PlayerLocalIndexs.Contains(LocalPlayerIndex))
		return;

	//Add Player
	Subsystem->PlayerActors.Add(Actor);
	Subsystem->PlayerLocalIndexs.Add(LocalPlayerIndex);
	Subsystem->PlayerInitializeds.Add(false);
}

void UFluidToolboxControllerBPLibrary::RemovePlayerControllerByActor(AActor* Actor)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Add If Parameters Are Is Invalid
	if (Actor == nullptr)
		return;

	//Remove Player
	int Index;
	bool bFound = Subsystem->PlayerActors.Find(Actor,Index);
	if (bFound)
	{
		//Call Begin  Interface (Blueprints)
		bool bBoolToForceEventToFunctionIgnoreThis;
		IFluidToolboxControllerPlayerInterface::Execute_OnFluidToolboxControllerPlayerBegin(Actor,Actor,Subsystem->PlayerLocalIndexs[Index], bBoolToForceEventToFunctionIgnoreThis);

		//Remove
		Subsystem->PlayerActors.RemoveAt(Index);
		Subsystem->PlayerLocalIndexs.RemoveAt(Index);
		Subsystem->PlayerInitializeds.RemoveAt(Index);
	}
}

void UFluidToolboxControllerBPLibrary::RemovePlayerControllerByLocalPlayerIndex(int LocalPlayerIndex)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Add If Parameters Are Is Invalid
	if (LocalPlayerIndex < 0 || LocalPlayerIndex  > 7)
		return;

	//Remove Player
	int Index;
	bool bFound = Subsystem->PlayerLocalIndexs.Find(LocalPlayerIndex,Index);
	if (bFound)
	{
		//Call End Interface (Blueprints)
		bool bBoolToForceEventToFunctionIgnoreThis;
		IFluidToolboxControllerPlayerInterface::Execute_OnFluidToolboxControllerPlayerEnd(Subsystem->PlayerActors[Index],Subsystem->PlayerActors[Index],LocalPlayerIndex, bBoolToForceEventToFunctionIgnoreThis);

		//Remove
		Subsystem->PlayerActors.RemoveAt(Index);
		Subsystem->PlayerLocalIndexs.RemoveAt(Index);
		Subsystem->PlayerInitializeds.RemoveAt(Index);
	}
}

void UFluidToolboxControllerBPLibrary::AddMountController(AActor* Owner, AActor* Mount)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Add If Parameters Are Is Invalid
	if (Owner == nullptr || Mount == nullptr)
		return;

	//Don't Add If Actor Does Not Implement Interface 
	if (!Mount->GetClass()->ImplementsInterface(UFluidToolboxControllerMountInterface::StaticClass()))
		return;

	//Add Mount
	if (Subsystem->Mounts.Contains(Owner))
		Subsystem->Mounts[Owner] = Mount;
	else Subsystem->Mounts.Add(Owner,Mount);

	//Call End Interface (Blueprints)
	bool bBoolToForceEventToFunctionIgnoreThis;
	IFluidToolboxControllerMountInterface::Execute_OnFluidToolboxControllerMountMountedBegin(Mount,Owner,bBoolToForceEventToFunctionIgnoreThis);

}

void UFluidToolboxControllerBPLibrary::RemoveMountController(AActor* Owner)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Remove If Parameters Are Is Invalid
	if (Owner == nullptr)
		return;

	//Don't Remove If Mount Is Not Valid
	if (!Subsystem->Mounts.Contains(Owner))
		return;

	//Call End Interface (Blueprints)
	bool bBoolToForceEventToFunctionIgnoreThis;
	IFluidToolboxControllerMountInterface::Execute_OnFluidToolboxControllerMountMountedEnd(Subsystem->Mounts[Owner],Owner,bBoolToForceEventToFunctionIgnoreThis);

	//Remove Mount
	Subsystem->Mounts.Remove(Owner);
}

void UFluidToolboxControllerBPLibrary::ToggleMountController(AActor* Owner, AActor* Mount)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Toggle If Parameters Are Is Invalid
	if (Owner == nullptr || Mount == nullptr)
		return;

	//Toggle Mounted Controller
	if (Subsystem->Mounts.Contains(Owner)) RemoveMountController(Owner);
	else AddMountController(Owner,Mount);
}

void UFluidToolboxControllerBPLibrary::GetMountController(AActor* Owner, AActor*& Mount_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxControllerSubsystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxControllerSubsystem>();
	if (Subsystem == nullptr) return;

	//Don't Get If Parameters Are Not Valid
	if (Owner == nullptr)
		return;

	//Toggle Mounted Controller
	if (Subsystem->Mounts.Contains(Owner)) Mount_Out = Subsystem->Mounts[Owner];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Controller Subsystem : Main

void UFluidToolboxControllerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	bInitialized = true;
}

void UFluidToolboxControllerSubsystem::Deinitialize()
{
	bInitialized = false;
}

void UFluidToolboxControllerSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;

	//Bool To Force Event To Function In Blueprints
	bool bBoolToForceEventToFunctionIgnoreThis;
	
	//Update Players
	for (int i = 0; i < PlayerActors.Num(); i++)
	{
		//Call Begin Play Interface (Blueprints) (Done Here In Tick To Ensure Actors Begin Is Finished)
		if (!PlayerInitializeds[i])
		{
			IFluidToolboxControllerPlayerInterface::Execute_OnFluidToolboxControllerPlayerBegin(PlayerActors[i],PlayerActors[i],PlayerLocalIndexs[i],bBoolToForceEventToFunctionIgnoreThis);
			PlayerInitializeds[i] = true;
		}

		//Call Tick Interface (Blueprints)
		IFluidToolboxControllerPlayerInterface::Execute_OnFluidToolboxControllerPlayerTick(PlayerActors[i],PlayerActors[i],PlayerLocalIndexs[i],DeltaTime,bBoolToForceEventToFunctionIgnoreThis);
	}
}

bool UFluidToolboxControllerSubsystem::IsTickable() const
{
	return !IsTemplate() && bInitialized;
}

bool UFluidToolboxControllerSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidToolboxControllerSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidToolboxControllerSubsystem::GetStatId() const
{
	return TStatId();
}