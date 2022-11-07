#include "FluidToolboxFootsteps.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Footsteps : Debugging

static TAutoConsoleVariable<float> CDebugFluidToolboxFootstepTrace(
	TEXT("Fluid.Toolbox.Footstep.Trace.Debug"),
	0,
	TEXT("Fluid.Toolbox.Footstep.Trace.Debug\n"),
	ECVF_SetByConsole);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Footsteps : Anim Notify

UFluidToolboxFootstepAnimNotify::UFluidToolboxFootstepAnimNotify() : Super()
{
	#if WITH_EDITORONLY_DATA
		NotifyColor = FColor(196, 142, 255, 255);
	#endif
}

void UFluidToolboxFootstepAnimNotify::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	//Return If Not Valid
    if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr || Animation == nullptr)
		return;
	
	//Get Location (Default Is Actor Location, Socket If Valid)
	FVector Location = MeshComp->GetOwner()->GetActorLocation();
	if (MeshComp->GetSocketByName(SocketName))
		Location = MeshComp->GetSocketLocation(SocketName);

	//Get Footstep Subsystem
	const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GEngine->GetCurrentPlayWorld());
	if (GameInstance == nullptr) return;
	UFluidToolboxFootstepSubSystem* Subsystem = GameInstance->GetSubsystem<UFluidToolboxFootstepSubSystem>();

	//Do Footstep
	Subsystem->DoFootstep(Archetype,Gait,Foot,Location);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Footsteps : Subsystem

void UFluidToolboxFootstepSubSystem::RegisterFootstepProfile(UFluidToolboxFootstepProfiles* FootstepProfiles)
{
	FootstepProfileses.AddUnique(FootstepProfiles);	
}

void UFluidToolboxFootstepSubSystem::DoFootstep(const FName Archetype, const FName Gait, const FName Foot, const FVector Location)
{
	//Sample Landscape
	FText PhysicsMaterialSurfaceTypeAsText;
	SampleForPhysicsAsset(Location,PhysicsMaterialSurfaceTypeAsText);
	
	//Construct | Debug Key
	const FName Key = FName(FString(Archetype.ToString()+ "." + PhysicsMaterialSurfaceTypeAsText.ToString() + "." + Gait.ToString() + "." + Foot.ToString()));
	if (CDebugFluidToolboxFootstepTrace.GetValueOnAnyThread() > 0)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Key.ToString());
	
	//Do Footstep
	for (int i = 0; i < FootstepProfileses.Num(); i++)
		if (FootstepProfileses[i]->Profiles.Contains(Key))
		{
			const FFluidToolboxFootstepProfile* Profile = &FootstepProfileses[i]->Profiles[Key];
			if (Profile->Sound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Profile->Sound,Location, FRotator(),FMath::RandRange(Profile->Volume.X,Profile->Volume.Y),FMath::RandRange(Profile->Pitch.X,Profile->Pitch.Y),0, Profile->SoundAttenuation, Profile->SoundConcurrency);

			return;
		}
}

void UFluidToolboxFootstepSubSystem::SampleForPhysicsAsset(const FVector Location, FText& PhysicsMaterialSurfaceTypeAsText_Out)
{
	//Data For Line Trace
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	FVector Start = Location;
	FVector End = Location + FVector(0,0,-1000);

	//Do Line Trace
	bool bDidHit = GetWorld()->LineTraceSingleByChannel(HitResult,Start,End,ECC_Visibility,CollisionQueryParams);

	//Debug Line Trace
	if (CDebugFluidToolboxFootstepTrace.GetValueOnAnyThread() > 0)
		DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,2);

	//Get Physics Material Type As Text
	PhysicsMaterialSurfaceTypeAsText_Out = FText::FromString("Default");
	if (bDidHit)
	{
		if (HitResult.PhysMaterial != nullptr)
		{
			const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPhysicalSurface"), true);
			if (!EnumPtr) return;
			PhysicsMaterialSurfaceTypeAsText_Out = EnumPtr->GetDisplayNameTextByValue((int64)HitResult.PhysMaterial->SurfaceType.GetValue());
		}

		//Debug Trace Location
		if (CDebugFluidToolboxFootstepTrace.GetValueOnAnyThread() > 0)
			DrawDebugSolidBox(GetWorld(),HitResult.Location + FVector(0,0,2.5f),FVector(5,5,5),FColor::Red,false,2);
	}
}