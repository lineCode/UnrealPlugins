// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "FluidToolboxCharacter.generated.h"

//Constants
const float FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_CARDINAL_DIRECTION_TO_ANGLE_OFFSETS[4] = {0.0f, -90.0f, -180.0f, 90.0f};
const float FLUID_TOOLBOX_CHARACTER_ORIENTATION_WARPING_ANGLE_MAX_SKIP_DELTA = 135.0f;

UCLASS()
class FLUIDTOOLBOXRUNTIME_API AFluidToolboxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AFluidToolboxCharacter();
	virtual void BeginPlay() override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

//Helpers

	UFUNCTION(BlueprintCallable)
		float HelperGetVelocityRate() const;
	UFUNCTION(BlueprintCallable)
		float HelperGetRotationRelativeToVelocity() const;
	UFUNCTION(BlueprintCallable)
		void HelperMoveRelativeToCameraByLocalPlayerIndex(const FVector Direction, const float Scale, const bool bForce, const int LocalPlayerIndex);
	UFUNCTION(BlueprintCallable)
		void HelperSetMovementJumpProperties(const int MaxJumpCount, const float GravityScale, const float BrakingDecelerationFalling, const float AirControl, const float AirControlBoostMultiplier, const float AirControlBoostVelocityThreshold, const float FallingLateralFriction, const float JumpMaxTime, const float JumpZVelocity);
	UFUNCTION(BlueprintCallable)
		void HelperSetIsStrafing(const bool bStrafe) const;
	UFUNCTION(BlueprintCallable)
		void HelperToggleIsStrafing() const;
	UFUNCTION(BlueprintCallable)
		bool HelperGetIsStrafing() const;
	UFUNCTION(BlueprintCallable)
		bool HelperGetIsInAir() const;

//Movement
	
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		void MovementMoveInDirection(const FVector Direction, const float Scale, const bool bForce);
	UFUNCTION(BlueprintCallable, Category="Fluid AI Utility Planner Additions")
		void MovementMoveToLocation(const FVector Location, const float Scale, const bool bForce);

//Actions
public:

	UFUNCTION(BlueprintCallable)
		void AddActions(TArray<FName> Names, TArray<UAnimMontage*> AnimMontages,  TArray<float> BlendOutTimes);
	UFUNCTION(BlueprintCallable)
		void DoAction(const FName Name, const bool StopAllActions);
	UFUNCTION(BlueprintCallable)
		void StopAllActions();
	UFUNCTION(BlueprintCallable)
		bool GetIsDoingAction();
	
	TMap<FName,UAnimMontage*> ActionMontages;
	TMap<FName,float> ActionBlendOutTimes;
	FName PreviousActionName;
	
//Orientation Warping
public:
	
	UFUNCTION(BlueprintCallable)
		void CalculateOrientationWarping(const float InterpSpeed,const float AngleMaxStepDelta, const int Mode);
	
	UPROPERTY(BlueprintReadWrite)
		uint8 OrientationWarpingCardinalDirection;
	UPROPERTY(BlueprintReadWrite)
		float OrientationWarpingAngle;
	

//Math (Static)
public:
	
	static float MathGetAngleDelta(const float StartAngle, const float EndAngle);
	static float MathGetAngleWrap(float Angle);
	static float MathGetFloatMoveToward(const float Start, const float End, const float MaxDelta);
};
