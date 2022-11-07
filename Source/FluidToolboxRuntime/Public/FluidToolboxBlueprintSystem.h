
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidToolboxBlueprintSystem.generated.h"

//Fluid Toolbox Blueprint System 
UCLASS(Blueprintable, BlueprintType, abstract)
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxBlueprintSystem : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnStart();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnEnd();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnTick(float DeltaTime);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnMessageReceived(FName Message);

	virtual void OnStart_Implementation();
	virtual void OnEnd_Implementation();
	virtual void OnTick_Implementation(float DeltaTime);
	virtual void OnMessageReceived_Implementation(FName Message);
	
	UPROPERTY(BlueprintReadWrite)
		bool bTickEnabled;
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
		float TickInterval;
	UPROPERTY(BlueprintReadOnly)
		float TickTimeToComplete;
	float TickIntervalStep;
};

//Fluid Toolbox Blueprint System Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxBlueprintSystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Fluid Toolbox Blueprint System Blueprint Library : Management
public:

	UFUNCTION(BlueprintCallable)
		static void AddBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass);
	UFUNCTION(BlueprintCallable)
		static void AddBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses);
	UFUNCTION(BlueprintCallable)
		static void RemoveBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass);
	UFUNCTION(BlueprintCallable)
		static void RemoveBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses);
	UFUNCTION(BlueprintCallable)
		static void RemoveBlueprintSystemsAll();
	UFUNCTION(BlueprintCallable)
		static void GetBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem* BlueprintSystem_Out);
	UFUNCTION(BlueprintCallable)
		static void GetBlueprintSystemUnsafe(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem*& BlueprintSystem_Out);
	UFUNCTION(BlueprintCallable)
		static void SetBlueprintSystemTickEnabled(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, bool bIsEnabled);

//Fluid Toolbox Blueprint System Blueprint Library : Messaging
public:
	
	UFUNCTION(BlueprintCallable)
		static void SendMessageToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, FName Message);
	UFUNCTION(BlueprintCallable)
		static void SendMessagesToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, TArray<FName> Messages);
	UFUNCTION(BlueprintCallable)
		static void SendMessageToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, FName Message);
	UFUNCTION(BlueprintCallable)
		static void SendMessagesToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, TArray<FName> Messages);
	UFUNCTION(BlueprintCallable)
		static void SendMessageToBlueprintSystemsAll(FName Message);
	UFUNCTION(BlueprintCallable)
		static void SendMessagesToBlueprintSystemsAll(TArray<FName> Messages);
	
};

//Fluid Toolbox Blueprint System SubSystem
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxBlueprintSystemSubSystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Toolbox Blueprint System SubSystem Main
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;

	bool bInitialized;

//Fluid Toolbox Blueprint System SubSystem : Management
public:
	
	UFUNCTION(BlueprintCallable)
		void AddBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass);
	UFUNCTION(BlueprintCallable)
        void AddBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses);
	UFUNCTION(BlueprintCallable)
		void RemoveBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass);
	UFUNCTION(BlueprintCallable)
        void RemoveBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses);
	UFUNCTION(BlueprintCallable)
		void RemoveBlueprintSystemsAll();
	UFUNCTION(BlueprintCallable)
		void GetBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem* BlueprintSystem_Out);
	UFUNCTION(BlueprintCallable)
		void GetBlueprintSystemUnsafe(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, UFluidToolboxBlueprintSystem*& BlueprintSystem_Out);
	UFUNCTION(BlueprintCallable)
		void SetBlueprintSystemTickEnabled(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, bool bIsEnabled);

	TMap<FName,UFluidToolboxBlueprintSystem*> BlueprintSystems;
	
//Fluid Toolbox Blueprint System SubSystem : Messaging
public:
	
	UFUNCTION(BlueprintCallable)
		void SendMessageToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, FName Message);
	UFUNCTION(BlueprintCallable)
		void SendMessagesToBlueprintSystem(TSubclassOf<UFluidToolboxBlueprintSystem> BlueprintSystemClass, TArray<FName> Messages);
	UFUNCTION(BlueprintCallable)
		void SendMessageToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, FName Message);
	UFUNCTION(BlueprintCallable)
		void SendMessagesToBlueprintSystems(TArray<TSubclassOf<UFluidToolboxBlueprintSystem>> BlueprintSystemClasses, TArray<FName> Messages);
	UFUNCTION(BlueprintCallable)
		void SendMessageToBlueprintSystemsAll(FName Message);
	UFUNCTION(BlueprintCallable)
		void SendMessagesToBlueprintSystemsAll(TArray<FName> Messages);
};
