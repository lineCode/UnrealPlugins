
#include "FluidToolboxBlueprintSystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System Blueprint Library : Management

void UFluidToolboxBlueprintSystemBPLibrary::AddBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->AddBlueprintSystem(BlueprintSystemClass);
}

void UFluidToolboxBlueprintSystemBPLibrary::AddBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->AddBlueprintSystems(BlueprintSystemClasses);
}

void UFluidToolboxBlueprintSystemBPLibrary::RemoveBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->RemoveBlueprintSystem(BlueprintSystemClass);
}

void UFluidToolboxBlueprintSystemBPLibrary::RemoveBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->RemoveBlueprintSystems(BlueprintSystemClasses);
}

void UFluidToolboxBlueprintSystemBPLibrary::RemoveBlueprintSystemsAll()
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->RemoveBlueprintSystemsAll();
}

void UFluidToolboxBlueprintSystemBPLibrary::GetBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem* BlueprintSystem_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->GetBlueprintSystem(BlueprintSystemClass, BlueprintSystem_Out);
}

void UFluidToolboxBlueprintSystemBPLibrary::GetBlueprintSystemUnsafe(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem*& BlueprintSystem_Out)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->GetBlueprintSystemUnsafe(BlueprintSystemClass, BlueprintSystem_Out);
}

void UFluidToolboxBlueprintSystemBPLibrary::SetBlueprintSystemTickEnabled(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, bool bIsEnabled)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SetBlueprintSystemTickEnabled(BlueprintSystemClass,bIsEnabled);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System Blueprint Library : Messaging

void UFluidToolboxBlueprintSystemBPLibrary::SendMessageToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, FName Message)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessageToBlueprintSystem(BlueprintSystemClass, Message);
}

void UFluidToolboxBlueprintSystemBPLibrary::SendMessagesToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, TArray<FName> Messages)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessagesToBlueprintSystem( BlueprintSystemClass, Messages);
}

void UFluidToolboxBlueprintSystemBPLibrary::SendMessageToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, FName Message)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessageToBlueprintSystems(BlueprintSystemClasses, Message);
}

void UFluidToolboxBlueprintSystemBPLibrary::SendMessagesToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, TArray<FName> Messages)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessagesToBlueprintSystems(BlueprintSystemClasses, Messages);
}

void UFluidToolboxBlueprintSystemBPLibrary::SendMessageToBlueprintSystemsAll(FName Message)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessageToBlueprintSystemsAll(Message);
}

void UFluidToolboxBlueprintSystemBPLibrary::SendMessagesToBlueprintSystemsAll(TArray<FName> Messages)
{
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	UFluidToolboxBlueprintSystemSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxBlueprintSystemSubSystem>();
	Subsystem->SendMessagesToBlueprintSystemsAll(Messages);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System

void UFluidToolboxBlueprintSystem::OnStart_Implementation()
{
}

void UFluidToolboxBlueprintSystem::OnEnd_Implementation()
{
}

void UFluidToolboxBlueprintSystem::OnTick_Implementation(float DeltaTime)
{
}

void UFluidToolboxBlueprintSystem::OnMessageReceived_Implementation(FName Message)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System Subsystem : Main

void UFluidToolboxBlueprintSystemSubSystem::Initialize(FSubsystemCollectionBase & Collection)
{
	//Return If Already Initialized | Set Initialized
	if (bInitialized) return;
	bInitialized = true;
}

void UFluidToolboxBlueprintSystemSubSystem::Deinitialize()
{
	//Remove Systems All
	RemoveBlueprintSystemsAll();
}


void UFluidToolboxBlueprintSystemSubSystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bInitialized) return;
	
	//Tick Blueprint Systems
	UFluidToolboxBlueprintSystem* System;
	double StatTimeStart = 0;
	double StatTimeEnd = 0;
	for (const TPair<FName,UFluidToolboxBlueprintSystem*>& Pair : BlueprintSystems)
	{
		System = Pair.Value;

		if(!System->bTickEnabled)
			continue;
		
		System->TickIntervalStep -= 1 * DeltaTime;
		if (System->TickIntervalStep <= 0)
		{
			System->TickIntervalStep = System->TickInterval;
			
			StatTimeStart = FPlatformTime::Seconds();
			System->OnTick(DeltaTime);
			StatTimeEnd = FPlatformTime::Seconds();
			System->TickTimeToComplete = StatTimeEnd-StatTimeStart;
		}
	}
}

bool UFluidToolboxBlueprintSystemSubSystem::IsTickable() const
{
	//Validate
	return !IsTemplate() && bInitialized;
}

bool UFluidToolboxBlueprintSystemSubSystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidToolboxBlueprintSystemSubSystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidToolboxBlueprintSystemSubSystem::GetStatId() const
{
	return TStatId();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System Subsystem : Management

void UFluidToolboxBlueprintSystemSubSystem::AddBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass)
{
	//Return As System Is Already Added
	if (BlueprintSystems.Contains(BlueprintSystemClass->GetFName()))
		return;

	//Add System
	const auto System = NewObject<UFluidToolboxBlueprintSystem>(this,BlueprintSystemClass);
	System->bTickEnabled = true;
	System->TickIntervalStep = 0;
	System->OnStart();
	BlueprintSystems.Add(BlueprintSystemClass->GetFName(), System);
}

void UFluidToolboxBlueprintSystemSubSystem::AddBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses)
{
	//Add Blueprint Systems
	for (int i = 0; i < BlueprintSystemClasses.Num(); i++)
		AddBlueprintSystem(BlueprintSystemClasses[i]);
}

void UFluidToolboxBlueprintSystemSubSystem::RemoveBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass)
{
	//Return As System Is Not Added
	if (!BlueprintSystems.Contains(BlueprintSystemClass->GetFName()))
		return;

	//Remove System
	const auto System = BlueprintSystems[BlueprintSystemClass->GetFName()];
	BlueprintSystems.Remove(BlueprintSystemClass->GetFName());
	System->OnEnd();
	System->ConditionalBeginDestroy();
}

void UFluidToolboxBlueprintSystemSubSystem::RemoveBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses)
{
	//Remove Blueprint Systems
	for (int i = 0; i < BlueprintSystemClasses.Num(); i++)
		RemoveBlueprintSystem(BlueprintSystemClasses[i]);
}

void UFluidToolboxBlueprintSystemSubSystem::RemoveBlueprintSystemsAll()
{
	//Call OnEnd And Begin Destroy On All Systems
	for (const TPair<FName,UFluidToolboxBlueprintSystem*>& Pair : BlueprintSystems)
	{
		Pair.Value->OnEnd();
		Pair.Value->ConditionalBeginDestroy();
	}

	//Remove All Blueprint Systems
	BlueprintSystems.Empty();
}

void UFluidToolboxBlueprintSystemSubSystem::GetBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem* BlueprintSystem_Out)
{
	//Return As System Is Not Added
	if (!BlueprintSystems.Contains(BlueprintSystemClass->GetFName()))
		return;

	//Remove System
	BlueprintSystem_Out = BlueprintSystems[BlueprintSystemClass->GetFName()];
}

void UFluidToolboxBlueprintSystemSubSystem::GetBlueprintSystemUnsafe(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem*& BlueprintSystem_Out)
{
	//Remove System
	BlueprintSystem_Out = BlueprintSystems[BlueprintSystemClass->GetFName()];
}


void UFluidToolboxBlueprintSystemSubSystem::SetBlueprintSystemTickEnabled(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, bool bIsEnabled)
{
	//Return As System Is Not Added
	if (!BlueprintSystems.Contains(BlueprintSystemClass->GetFName()))
		return;

	//Remove System
	const auto System = BlueprintSystems[BlueprintSystemClass->GetFName()];
	System->bTickEnabled = bIsEnabled;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Blueprint System Subsystem : Messaging

void UFluidToolboxBlueprintSystemSubSystem::SendMessageToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, FName Message)
{
	//Return As System Is Not Added
	if (!BlueprintSystems.Contains(BlueprintSystemClass->GetFName()))
		return;

	//Send Message To System
	const auto System = BlueprintSystems[BlueprintSystemClass->GetFName()];
	System->OnMessageReceived(Message);
}

void UFluidToolboxBlueprintSystemSubSystem::SendMessagesToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, TArray<FName> Messages)
{
	//Send Messages To System
	for (int i = 0; i < Messages.Num(); i++)
		SendMessageToBlueprintSystem(BlueprintSystemClass,Messages[i]);
}

void UFluidToolboxBlueprintSystemSubSystem::SendMessageToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, FName Message)
{
	//Send Message To Systems
	for (int i = 0; i < BlueprintSystemClasses.Num(); i++)
	{
		//Continue As System Is Not Added
		if (!BlueprintSystems.Contains(BlueprintSystemClasses[i]->GetFName()))
			continue;

		//Send Message To System
		const auto System = BlueprintSystems[BlueprintSystemClasses[i]->GetFName()];
		System->OnMessageReceived(Message);
	}
}

void UFluidToolboxBlueprintSystemSubSystem::SendMessagesToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, TArray<FName> Messages)
{
	//Send Messages To Systems
	for (int i = 0; i < Messages.Num(); i++)
		SendMessageToBlueprintSystems(BlueprintSystemClasses,Messages[i]);
}

void UFluidToolboxBlueprintSystemSubSystem::SendMessageToBlueprintSystemsAll(FName Message)
{
	//Send Message To Blueprint Systems All
	for (const TPair<FName,UFluidToolboxBlueprintSystem*>& Pair : BlueprintSystems)
		Pair.Value->OnMessageReceived(Message);
}

void UFluidToolboxBlueprintSystemSubSystem::SendMessagesToBlueprintSystemsAll(TArray<FName> Messages)
{
	//Send Messages To Blueprint Systems All
	for (int i = 0; i < Messages.Num(); i++)
		SendMessageToBlueprintSystemsAll(Messages[i]);
}