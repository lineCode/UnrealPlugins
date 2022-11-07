
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "FluidToolboxCamera.generated.h"

//Fluid Toolbox Camera Data Set Profile Type
UENUM()
enum class EFluidToolboxCameraDataSetCameraProfileType : uint8
{
	ThirdPerson,
	FirstPerson,
};

//Fluid Toolbox Camera Profile
USTRUCT()
struct FFluidToolboxCameraProfile
{
	GENERATED_BODY()

//Profile Type
public:
	
	UPROPERTY(EditDefaultsOnly)
		EFluidToolboxCameraDataSetCameraProfileType ProfileType;

//Third Person
public:

	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Socket To Snap To (Required)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::FirstPerson", EditConditionHides))
		FName FirstPersonSocketToSnapTo;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Socket Offset (Optional)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::FirstPerson", EditConditionHides))
		FVector FirstPersonSocketOffset;
	
//Third Person
public:

	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Socket To Snap To (Required)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		FName ThirdPersonSocketToSnapTo;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Socket Offset (Optional)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		FVector ThirdPersonSocketOffset;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "SpringArm Distance", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		float ThirdPersonSpringArmDistance;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Camera Lag Max Distance (0 is Unreal default)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		float ThirdPersonCameraLagMaxDistance;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Camera Lag Max Time Step (0.016667 is Unreal default)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		float ThirdPersonCameraLagMaxTimeStep;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Camera Lag Speed (10 is Unreal default, 0 to disable)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		float ThirdPersonCameraLagSpeed;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Camera Lag Rotation Speed (10 is Unreal default, 0 to disable)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		float ThirdPersonCameraRotationLagSpeed;
	UPROPERTY(EditDefaultsOnly, meta=(displayName = "Camera Lag Sub Stepping Enabled (True is Unreal default)", EditCondition="ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson", EditConditionHides))
		bool bUseThirdPersonCameraLagSubStepping;
};

//Fluid Toolbox Camera Profiles
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxCameraProfiles : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FFluidToolboxCameraProfile> Profiles;
};

//Fluid Toolbox Camera Blueprint Library (Clean Wrapper For Subsystem For Blueprint Users)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxCameraBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

//Fluid Toolbox Camera Blueprint Library : Camera Main 
public:

	UFUNCTION(BlueprintCallable)
		static void CameraCreate(USceneComponent* ParentComponent, const int PlayerIndex, UFluidToolboxCameraProfiles* CameraDataSet, const FName CameraProfileID, const FName NewCameraID, bool& bIsThirdPerson_Out, bool& bDidCreate_Out);
	UFUNCTION(BlueprintCallable)
		static void CameraDestroyByCameraID(const FName CameraID);
	UFUNCTION(BlueprintCallable)
		static void CameraDestroyByPlayerIndex(const int PlayerIndex);
	UFUNCTION(BlueprintCallable)
		static void CameraInputByCameraID(const FName CameraID, const float X, const float Y);
	UFUNCTION(BlueprintCallable)
		static void CameraInputByPlayerIndex(const int PlayerIndex, const float X, const float Y);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraComponentsByCameraID(const FName CameraID, UCameraComponent*& CameraComponent_Out, USpringArmComponent*& SpringArmComponent_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraComponentsByPlayerIndex(const int PlayerIndex, UCameraComponent*& CameraComponent_Out, USpringArmComponent*& SpringArmComponent_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraLocationByCameraID(const FName CameraID, FVector& CameraLocation_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraLocationByPlayerIndex(const int PlayerIndex,FVector& CameraLocation_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraForwardByCameraID(const FName CameraID, FVector& CameraForward_Out);
	UFUNCTION(BlueprintCallable,BlueprintPure)
		static void GetCameraForwardByPlayerIndex(const int PlayerIndex,FVector& CameraForward_Out);
	UFUNCTION(BlueprintCallable)
		static void ApplyCameraProfile(UFluidToolboxCameraProfiles* CameraDataSet, const FName CameraProfileID, const FName CameraID, bool& bDidApply_Out);
};

//Fluid Toolbox Actors SubSystem (To Hold Data)
UCLASS()
class FLUIDTOOLBOXRUNTIME_API UFluidToolboxCameraSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	TMap<int,FName> PlayerIndexToCameraIDs;
	TMap<FName,USceneComponent*> CameraParentComponents;
	TMap<FName,UCameraComponent*> CameraCameraComponents;
	TMap<FName,USpringArmComponent*> CameraSpringArmComponents;
};