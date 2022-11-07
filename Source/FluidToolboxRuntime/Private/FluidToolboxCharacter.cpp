// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidToolboxCharacter.h"

AFluidToolboxCharacter::AFluidToolboxCharacter()
{
	//Defaults (Actor)
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Defaults (Character Movement)
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void AFluidToolboxCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFluidToolboxCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Character : Helpers

float AFluidToolboxCharacter::HelperGetVelocityRate() const
{
	return  GetVelocity().Size2D() / GetMovementComponent()->GetMaxSpeed();
}

float AFluidToolboxCharacter::HelperGetRotationRelativeToVelocity() const
{
	//Cache Velocity
	const FVector Velocity = GetVelocity();

	//Make Sure Its Big Enough
	if (Velocity.SizeSquared() < 0.0001f)
		return 0.0f;

	//Get Rotation Z
	float RotationZ = (GetActorRotation() - Velocity.ToOrientationRotator()).Yaw * -1.0f;

	//Warp Rotation
	if(RotationZ > 180.0f)
	{
		RotationZ -= 360.0f;
	}
	else if(RotationZ < -180.0f)
	{
		RotationZ += 360.0f;
	}

	//Return
	return RotationZ;
}

void AFluidToolboxCharacter::HelperMoveRelativeToCameraByLocalPlayerIndex(const FVector Direction, const float Scale, const bool bForce, const int LocalPlayerIndex)
{
	//Yaw Input
	const float InputYaw = FMath::RadiansToDegrees(FMath::Atan2(Direction.X,Direction.Y));

	//Get Player Controller
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),LocalPlayerIndex);
	if (!PlayerController)
		return;

	//Get Camera Yaw
	const float CameraYaw = PlayerController->PlayerCameraManager->GetCameraRotation().Yaw;

	//Move
	AddMovementInput(UKismetMathLibrary::CreateVectorFromYawPitch(InputYaw + CameraYaw,0,Direction.Size()),Scale,bForce);
}

void AFluidToolboxCharacter::HelperSetMovementJumpProperties(const int MaxJumpCount, const float GravityScale, const float BrakingDecelerationFalling, const float AirControl, const float AirControlBoostMultiplier, const float AirControlBoostVelocityThreshold, const float FallingLateralFriction, const float JumpMaxTime, const float JumpZVelocity)
{
	const auto ActorCharacterMovementComponent = GetCharacterMovement();
	ActorCharacterMovementComponent->GravityScale = GravityScale;
	ActorCharacterMovementComponent->BrakingDecelerationFalling = BrakingDecelerationFalling;
	ActorCharacterMovementComponent->AirControl = AirControl;
	ActorCharacterMovementComponent->AirControlBoostMultiplier = AirControlBoostMultiplier;
	ActorCharacterMovementComponent->AirControlBoostVelocityThreshold = AirControlBoostVelocityThreshold;
	ActorCharacterMovementComponent->FallingLateralFriction = FallingLateralFriction;
	ActorCharacterMovementComponent->JumpZVelocity = JumpZVelocity;
	JumpMaxCount = MaxJumpCount;
	JumpMaxHoldTime = JumpMaxTime;
}

void AFluidToolboxCharacter::HelperSetIsStrafing(const bool bStrafe) const
{
	GetCharacterMovement()->bOrientRotationToMovement = !bStrafe;
}

void AFluidToolboxCharacter::HelperToggleIsStrafing() const
{
	GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
}

bool AFluidToolboxCharacter::HelperGetIsStrafing() const
{
	return !GetCharacterMovement()->bOrientRotationToMovement;
}

bool AFluidToolboxCharacter::HelperGetIsInAir() const
{
	return GetCharacterMovement()->IsFalling();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Character : Movement

void AFluidToolboxCharacter::MovementMoveInDirection(const FVector Direction, const float Scale, const bool bForce)
{
	if (GetIsDoingAction())
		return;
		
	//Add Movement Input
	AddMovementInput(Direction.GetSafeNormal(),Scale, bForce);
}

void AFluidToolboxCharacter::MovementMoveToLocation(const FVector Location, const float Scale, const bool bForce)
{
	if (GetIsDoingAction())
		return;
	
	MovementMoveInDirection((Location - GetActorLocation()).GetSafeNormal(),Scale, bForce);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Character : Actions

void AFluidToolboxCharacter::AddActions(TArray<FName> Names, TArray<UAnimMontage*> AnimMontages, TArray<float> BlendOutTimes)
{
	// Arrays Not Same Length So Return
	if (Names.Num() != AnimMontages.Num() || Names.Num() != BlendOutTimes.Num())
		return;

	for (int i = 0; i < Names.Num(); i++)
	{
		if (ActionMontages.Contains(Names[i]))
			continue;
		
		ActionMontages.Add(Names[i],AnimMontages[i]);
		ActionBlendOutTimes.Add(Names[i],BlendOutTimes[i]);
	}
}

void AFluidToolboxCharacter::DoAction(const FName Name, const bool StopAllActions)
{
	//Cache AnimInstance
	auto AnimInstance =  GetMesh()->GetAnimInstance();
	
	//Get Is Doing Action And Return If We Are And We Do Not Want To Stop
	const bool bIsDoingAction = AnimInstance->IsAnyMontagePlaying();
	if (bIsDoingAction && !StopAllActions)
		return;

	// Do Action
	if (!ActionMontages.Contains(Name))
		return;
	AnimInstance->Montage_Play(ActionMontages[Name],1,EMontagePlayReturnType::MontageLength,0,StopAllActions);
	PreviousActionName = Name;
}

void AFluidToolboxCharacter::StopAllActions()
{
	//Cache AnimInstance
	auto AnimInstance =  GetMesh()->GetAnimInstance();
	
	//Get Is Doing Action And Return If We Are
	const bool bIsDoingAction = AnimInstance->IsAnyMontagePlaying();
	if (!bIsDoingAction)
		return;

	AnimInstance->StopAllMontages(ActionBlendOutTimes[PreviousActionName]);
}

bool AFluidToolboxCharacter::GetIsDoingAction()
{
	//Return Is Doing Action
	return GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Character : Orientation Warping

void AFluidToolboxCharacter::CalculateOrientationWarping(const float InterpSpeed,const float AngleMaxStepDelta, const int Mode)
{
	//0 Cardinal Direction (None)
	if (Mode == 0)
		return;
		
	//Get Delta Time
	const float DeltaTime = GetWorld()->DeltaTimeSeconds;
		
	//Set Angle 
	float Angle = HelperGetRotationRelativeToVelocity();

	//1 Cardinal Direction (Forward Animation)
	if (Mode == 1)
		OrientationWarpingCardinalDirection = 0;
					
	//2 Cardinal Direction (Forward And Bck Animation)
	if (Mode == 2)
	{
		OrientationWarpingCardinalDirection = 0;
		if (Angle > 135 || Angle < -45)
		{
			OrientationWarpingCardinalDirection = 2;
			Angle += 180;
		}
		else
			OrientationWarpingCardinalDirection = 0;
	}
			
	//4 Cardinal Direction (Forward, Back,Left,Right Animations)
	else if (Mode == 3)
	{
		float OffsetDir = 0;
		
		switch (OrientationWarpingCardinalDirection)
		{
		case 0: //Forward
			{
				if (Angle > AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = Angle  > FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 2 : 1;
				else if (Angle  < -AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = Angle  < -FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 2 : 3;
			} break;

		case 1: //Right
			{
				OffsetDir = Angle  - 90.0f;
				if (OffsetDir < -180.0f)
					OffsetDir += 360.0f;

				if (OffsetDir > AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir > FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 3 : 2;
				else if (OffsetDir < -AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir < -FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 3 : 0;
			} break;

		case 2: //Back
			{
				OffsetDir = Angle  - 180.0f;
				if (OffsetDir < -180.0f)
					OffsetDir += 360.0f;
				else if (OffsetDir > 180.0f)
					OffsetDir -= 360.0f;

				if (OffsetDir > AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir > FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 0 : 3;
				else if (OffsetDir < -AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir < -FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 0 : 1;
			} break;

		case 3: //Left
			{
				OffsetDir = Angle + 90;
				if (OffsetDir > 180.0f)
					OffsetDir -= 360.0f;

				if (OffsetDir > AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir > FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 1 : 0;
					
				else if (OffsetDir < -AngleMaxStepDelta)
					OrientationWarpingCardinalDirection = OrientationWarpingCardinalDirection = OffsetDir < -FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA ? 1 : 2;
			} break;
		}

		//Get Angle Offset
		const auto AngleOffset = FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_CARDINAL_DIRECTION_TO_ANGLE_OFFSETS[OrientationWarpingCardinalDirection];
				
		//Set Angle Based On Max Step Delta
		if (FMath::Abs(AFluidToolboxCharacter::MathGetAngleDelta(-AngleOffset, AFluidToolboxCharacter::MathGetAngleWrap(Angle))) <  AngleMaxStepDelta)
			Angle = AFluidToolboxCharacter::MathGetAngleWrap(Angle + AngleOffset);
	}
							
	//Apply Angle (Use RInterp To Over FInterpTo As Rotation Goes Shortest Path)
	OrientationWarpingAngle = UKismetMathLibrary::RInterpTo(FRotator(0,OrientationWarpingAngle,0),FRotator(0,Angle,0),DeltaTime,InterpSpeed).Yaw;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Character Math

float AFluidToolboxCharacter::MathGetAngleDelta(const float StartAngle, const float EndAngle)
{
	float Delta = EndAngle - StartAngle;
	if (Delta > 180.0)   Delta -= 360.0f;
	else if (Delta < -180.0f) Delta += 360.0f;
	return Delta;
}

float AFluidToolboxCharacter::MathGetAngleWrap(float Angle)
{
	Angle = FMath::Fmod(Angle + 180.0f, 360.0f);
	if (Angle < 0.0f) Angle += 360.0f;
	Angle -= 180.0f;
	return Angle;
}

float AFluidToolboxCharacter::MathGetFloatMoveToward(const float Start, const float End, const float MaxDelta)
{
	if (FMath::Abs(End - Start) <= MaxDelta)
		return End;
	else
		return Start + FMath::Sign(End - Start) * MaxDelta;
}