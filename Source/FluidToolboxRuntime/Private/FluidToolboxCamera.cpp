#include "FluidToolboxCamera.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Camera Blueprint Library : Camera Main (Clean Wrapper For Subsystem For Blueprint Users)

void UFluidToolboxCameraBPLibrary::CameraCreate(USceneComponent* ParentComponent, const int PlayerIndex, UFluidToolboxCameraProfiles* CameraDataSet, const FName CameraProfileID, const FName NewCameraID, bool& bIsThirdPerson_Out, bool& bDidCreate_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Not Valid Parent Component Or Camera With ID Already Created
	bDidCreate_Out = ParentComponent != nullptr && CameraDataSet != nullptr && CameraDataSet->Profiles.Contains(CameraProfileID) && !Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex) && !Subsystem->CameraCameraComponents.Contains(NewCameraID);
	if (!bDidCreate_Out)
		return;
	
	//Get Camera And Spring Arm Component
	const auto SpringArmComponent = NewObject<USpringArmComponent>(ParentComponent->GetOwner(),USpringArmComponent::StaticClass(), "CameraSpringArm");
	SpringArmComponent->OnComponentCreated();
	SpringArmComponent->RegisterComponent();
	const auto CameraComponent = NewObject<UCameraComponent>(ParentComponent->GetOwner(),UCameraComponent::StaticClass(),"Camera");
	CameraComponent->RegisterComponent();
	CameraComponent->OnComponentCreated();

	//Attach Camera And Spring Arm Default
	SpringArmComponent->AttachToComponent(ParentComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,"None");
	CameraComponent->AttachToComponent(SpringArmComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,"None");

	//Add Camera To Subsystem
	Subsystem->PlayerIndexToCameraIDs.Add(PlayerIndex,NewCameraID);
	Subsystem->CameraParentComponents.Add(NewCameraID,ParentComponent);
	Subsystem->CameraCameraComponents.Add(NewCameraID,CameraComponent);
	Subsystem->CameraSpringArmComponents.Add(NewCameraID,SpringArmComponent);

	//Set Camera Profile
	ApplyCameraProfile(CameraDataSet, CameraProfileID, NewCameraID,bDidCreate_Out);
	if (!bDidCreate_Out) CameraDestroyByCameraID(NewCameraID);
}

void UFluidToolboxCameraBPLibrary::CameraDestroyByCameraID(const FName CameraID)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created
	if (!Subsystem->CameraCameraComponents.Contains(CameraID))
		return;

	//Get Camera Components
	const auto CameraComponent = Subsystem->CameraCameraComponents[CameraID];
	const auto SpringArmComponent = Subsystem->CameraSpringArmComponents[CameraID];

	//Remove Camera From Subsystem
	Subsystem->CameraParentComponents.Remove(CameraID);
	Subsystem->CameraSpringArmComponents.Remove(CameraID);
	Subsystem->CameraCameraComponents.Remove(CameraID);

	//Destroy Camera Components
	CameraComponent->DestroyComponent();
	SpringArmComponent->DestroyComponent();
}

void UFluidToolboxCameraBPLibrary::CameraDestroyByPlayerIndex(const int PlayerIndex)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created With Player Index
	if (!Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex))
		return;

	//Destroy Camera
	CameraDestroyByCameraID(Subsystem->PlayerIndexToCameraIDs[PlayerIndex]);
}

void UFluidToolboxCameraBPLibrary::CameraInputByCameraID(const FName CameraID, const float X, const float Y)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created
	if (!Subsystem->CameraCameraComponents.Contains(CameraID))
		return;

	//Get Camera Components
	const auto ParentComponent = Subsystem->CameraParentComponents[CameraID];
	const auto CameraComponent = Subsystem->CameraCameraComponents[CameraID];
	const auto SpringArmComponent = Subsystem->CameraSpringArmComponents[CameraID];
	
	//Get As Pawn
	auto AsPawn = (APawn*)ParentComponent->GetOwner();
	if (AsPawn == nullptr) return;
	
	//Third Person
	if (SpringArmComponent->TargetArmLength != 0)
	{
		//X Input
		SpringArmComponent->AddRelativeRotation(FRotator(0,X,0));

		//Y Input
		auto R = SpringArmComponent->GetRelativeRotation();
		R.Pitch += Y;
		R.Pitch = FMath::Clamp(R.Pitch,-89.0f,89.0f);
		SpringArmComponent->SetRelativeRotation(R);

		//No Controller Rotation
		AsPawn->Controller->SetControlRotation(FRotator(0,0,0));
		SpringArmComponent->bUsePawnControlRotation = false;
	}

	//First Person
	else
	{
		auto R = AsPawn->GetControlRotation();
		R.Yaw += X;
		R.Pitch += Y;
		R.Pitch = FMath::Clamp(R.Pitch,-89.0f,89.0f);
		AsPawn->Controller->SetControlRotation(R);
		SpringArmComponent->bUsePawnControlRotation = true;
		SpringArmComponent->bInheritPitch = true;
		SpringArmComponent->bInheritYaw = true;
		SpringArmComponent->bInheritRoll = true;
	}
}

void UFluidToolboxCameraBPLibrary::CameraInputByPlayerIndex(const int PlayerIndex, const float X, const float Y)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created With Player Index
	if (!Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex))
		return;

	//Do Camera Input
	CameraInputByCameraID(Subsystem->PlayerIndexToCameraIDs[PlayerIndex],X,Y);
}

void UFluidToolboxCameraBPLibrary::GetCameraComponentsByCameraID(const FName CameraID, UCameraComponent*& CameraComponent_Out, USpringArmComponent*& SpringArmComponent_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created
	if (!Subsystem->CameraCameraComponents.Contains(CameraID))
		return;

	//Get Camera Components
	CameraComponent_Out = Subsystem->CameraCameraComponents[CameraID];
	SpringArmComponent_Out = Subsystem->CameraSpringArmComponents[CameraID];
}

void UFluidToolboxCameraBPLibrary::GetCameraComponentsByPlayerIndex(const int PlayerIndex, UCameraComponent*& CameraComponent_Out, USpringArmComponent*& SpringArmComponent_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created With Player Index
	if (!Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex))
		return;

	//Get Camera Components
	GetCameraComponentsByCameraID(Subsystem->PlayerIndexToCameraIDs[PlayerIndex],CameraComponent_Out,SpringArmComponent_Out);
}

void UFluidToolboxCameraBPLibrary::GetCameraLocationByCameraID(const FName CameraID, FVector& CameraLocation_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created
	if (!Subsystem->CameraCameraComponents.Contains(CameraID))
		return;

	//Get Camera Location
	CameraLocation_Out = Subsystem->CameraCameraComponents[CameraID]->GetComponentLocation();
}

void UFluidToolboxCameraBPLibrary::GetCameraLocationByPlayerIndex(const int PlayerIndex, FVector& CameraLocation_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created With Player Index
	if (!Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex))
		return;

	//Get Camera Location
	GetCameraLocationByCameraID(Subsystem->PlayerIndexToCameraIDs[PlayerIndex],CameraLocation_Out);
}


void UFluidToolboxCameraBPLibrary::GetCameraForwardByCameraID(const FName CameraID, FVector& CameraForward_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created
	if (!Subsystem->CameraCameraComponents.Contains(CameraID))
		return;

	//Get Camera Forward
	CameraForward_Out = Subsystem->CameraCameraComponents[CameraID]->GetForwardVector();
}

void UFluidToolboxCameraBPLibrary::GetCameraForwardByPlayerIndex(const int PlayerIndex, FVector& CameraForward_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Camera Is Not Created With Player Index
	if (!Subsystem->PlayerIndexToCameraIDs.Contains(PlayerIndex))
		return;

	//Get Camera Components
	GetCameraForwardByCameraID(Subsystem->PlayerIndexToCameraIDs[PlayerIndex],CameraForward_Out);
}

void UFluidToolboxCameraBPLibrary::ApplyCameraProfile(UFluidToolboxCameraProfiles* CameraDataSet, const FName CameraProfileID, const FName CameraID, bool& bDidApply_Out)
{
	//Get Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());;
	UFluidToolboxCameraSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxCameraSubSystem>();
	
	//Profile Does Not Exist Or Camera Not Created
	bDidApply_Out = CameraDataSet != nullptr && CameraDataSet->Profiles.Contains(CameraProfileID) && Subsystem->CameraCameraComponents.Contains(CameraID);
	if (!bDidApply_Out)
		return;
	
	//Get Camera profile
	const FFluidToolboxCameraProfile* CameraProfile = &CameraDataSet->Profiles[CameraProfileID];
	const bool bIsThirdPerson = CameraProfile->ProfileType == EFluidToolboxCameraDataSetCameraProfileType::ThirdPerson;
	
	//Get Camera Components
	const auto ParentComponent = Subsystem->CameraParentComponents[CameraID];
	const auto SpringArmComponent = Subsystem->CameraSpringArmComponents[CameraID];

	//Socket Not Valid So Return
	if (!ParentComponent->DoesSocketExist(bIsThirdPerson ?  CameraProfile->ThirdPersonSocketToSnapTo : CameraProfile->FirstPersonSocketToSnapTo))
	{
		bDidApply_Out = false;
		return;
	}
	
	//Snap To Socket | Arm Length
	SpringArmComponent->AttachToComponent(ParentComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale,bIsThirdPerson ?  CameraProfile->ThirdPersonSocketToSnapTo : CameraProfile->FirstPersonSocketToSnapTo);
	SpringArmComponent->TargetArmLength = bIsThirdPerson ? CameraProfile->ThirdPersonSpringArmDistance : 0;

	//Rotation
	SpringArmComponent->bUsePawnControlRotation = !bIsThirdPerson;
	SpringArmComponent->SetUsingAbsoluteRotation(bIsThirdPerson);

	//Offset
	SpringArmComponent->SocketOffset  = bIsThirdPerson ? CameraProfile->ThirdPersonSocketOffset : CameraProfile->FirstPersonSocketOffset;
	
	//Camera Lag
	SpringArmComponent->CameraLagMaxDistance = bIsThirdPerson ? CameraProfile->ThirdPersonCameraLagMaxDistance : 0;
	SpringArmComponent->CameraLagMaxTimeStep = bIsThirdPerson ? CameraProfile->ThirdPersonCameraLagMaxTimeStep : 0;
	SpringArmComponent->CameraLagSpeed = bIsThirdPerson ? CameraProfile->ThirdPersonCameraLagSpeed : 0;
	SpringArmComponent->CameraRotationLagSpeed = bIsThirdPerson ? CameraProfile->ThirdPersonCameraRotationLagSpeed : 0;
	SpringArmComponent->bUseCameraLagSubstepping = bIsThirdPerson ? CameraProfile->bUseThirdPersonCameraLagSubStepping : false;
	SpringArmComponent->bEnableCameraLag = SpringArmComponent->CameraLagSpeed > 0;
	SpringArmComponent->bEnableCameraRotationLag = SpringArmComponent->CameraRotationLagSpeed > 0;
}

