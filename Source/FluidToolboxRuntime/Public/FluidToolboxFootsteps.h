#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "UObject/ObjectMacros.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "Animation/AnimSequenceBase.h"

#if WITH_EDITOR
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif

#include "FluidToolboxFootsteps.generated.h"

class UAnimSequenceBase;
class USkeletalMeshComponent;
class USoundBase;

//Fluid Toolbox Camera Data Set Profile
USTRUCT()
struct FFluidToolboxFootstepProfile
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
		USoundBase* Sound;
	UPROPERTY(EditDefaultsOnly)
		USoundAttenuation* SoundAttenuation;
	UPROPERTY(EditDefaultsOnly)
		USoundConcurrency* SoundConcurrency;
	UPROPERTY(EditDefaultsOnly)
		FVector2D Volume;
	UPROPERTY(EditDefaultsOnly)
		FVector2D Pitch;
};

//Fluid Toolbox Footstep Profile
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxFootstepProfiles : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
		TMap<FName, FFluidToolboxFootstepProfile> Profiles;
};

//Animation Notify
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxFootstepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UFluidToolboxFootstepAnimNotify();
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		FName Archetype;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		FName Gait;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		FName Foot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		FName SocketName;
};

//Fluid Toolbox Footstep SubSystem (To Hold Data)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxFootstepSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
		void RegisterFootstepProfile(UFluidToolboxFootstepProfiles* FootstepProfiles);
	UFUNCTION(BlueprintCallable)
		void DoFootstep(const FName Archetype, const FName Gait, const FName Foot, const FVector Location);
	UFUNCTION(BlueprintInternalUseOnly)
		void SampleForPhysicsAsset(const FVector Location, FText& PhysicsMaterialSurfaceTypeAsText_Out);

	TArray<UFluidToolboxFootstepProfiles*> FootstepProfileses;
};