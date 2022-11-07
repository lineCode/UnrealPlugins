// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "FluidWaterRuntime.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "GameFramework/HUD.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"
#include "FluidWaterSubsystem.generated.h"

/**
 * 
 */

//Fluid Water Wave Mode
UENUM(Blueprintable)
enum EFluidWaterWaveMode
{
	FluidWaterWaveModeNone						UMETA(DisplayName = "None"),
	FluidWaterWaveModeFFT						UMETA(DisplayName = "FFT Waves"),
	FluidWaterWaveModeGerstnerAndPoints			UMETA(DisplayName = "Gerstener Waves + Point Waves")
};

//Fluid Water Wave Mode
UENUM(Blueprintable)
enum EFluidWaterResolution
{
	FluidWaterResolution64						UMETA(DisplayName = "64x64"),
	FluidWaterResolution128						UMETA(DisplayName = "128x128"),
	FluidWaterResolution256						UMETA(DisplayName = "256x256"),
	FluidWaterResolution512						UMETA(DisplayName = "512x512"),
	FluidWaterResolution1024					UMETA(DisplayName = "1024x1024"),
	FluidWaterResolution2048					UMETA(DisplayName = "2048x2048"),
	FluidWaterResolution4096					UMETA(DisplayName = "4096x4096"),
	FluidWaterResolution8192					UMETA(DisplayName = "8192x8192")
};

//Fluid Water Subsystem
UCLASS(BlueprintType)
class FLUIDWATERRUNTIME_API UFluidWaterSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

//Fluid Narrator Subsystem : Initialize / DeInitialize / Tick
public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void Tick(float DeltaTime) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	TStatId GetStatId() const override;
	
	bool bIsInitialized;

//Fluid Narrator Subsystem : Waves
public:

	UFUNCTION(BlueprintCallable)
		void InitializeWavesAsFFT(const EFluidWaterResolution Resolution, UTextureRenderTarget2D* GaussDistributionRenderTarget,UTextureRenderTarget2D* PHKplusRenderTarget,UTextureRenderTarget2D* PHKminusRenderTarget, UTextureRenderTarget2D* FFTButterflyRenderTarget,UTextureRenderTarget2D* H0KplusTarget,UTextureRenderTarget2D* H0KminusTarget, UTextureRenderTarget2D* H0KplusH0KminusTarget, UTextureRenderTarget2D* DxRenderTarget,UTextureRenderTarget2D* DyRenderTarget,UTextureRenderTarget2D* DzRenderTarget, const FVector4 PhillipsSpectrumAVGF, const FVector2D PhillipsSpectrumW, const float PhillipsSpectrumLength);
	UFUNCTION(BlueprintCallable)
		void InitializeWavesAsGerstnerPlusPoints(const EFluidWaterResolution Resolution);
	UFUNCTION(BlueprintCallable)
		void SetWavesFFT(const FVector4 PhillipsSpectrumAVGF, const FVector2D PhillipsSpectrumW, const float PhillipsSpectrumLength);
	UFUNCTION(BlueprintCallable)
		void TickWavesFFT(const float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void DrawHUDDebugFFT(AHUD* HUD);
	UFUNCTION(BlueprintInternalUseOnly)
		void HUDDrawRenderTarget(AHUD* HUD, UTextureRenderTarget2D* RenderTarget2D, FLinearColor BackgroundLinearColor, FString Header, FString Footer, float X, float Y, bool bIsDebugOnly);

	EFluidWaterWaveMode WaveMode;
	UTextureRenderTarget2D* FFTGaussDistributionRenderTarget;
	UTextureRenderTarget2D* FFTFFTButterflyRenderTarget;
	UTextureRenderTarget2D* FFTPHKplusRenderTarget;
	UTextureRenderTarget2D* FFTPHKminusRenderTarget;
	UTextureRenderTarget2D* FFTH0KplusTarget;
	UTextureRenderTarget2D* FFTH0KminusTarget;
	UTextureRenderTarget2D* FFTH0KplusH0KminusTarget;
	UTextureRenderTarget2D* FFTDxRenderTarget;
	UTextureRenderTarget2D* FFTDyRenderTarget;
	UTextureRenderTarget2D* FFTDzRenderTarget;
	FVector4 PhillipsSpectrumAVGFCurrent;
	FVector2D PhillipsSpectrumWCurrent;
	float PhillipsSpectrumLengthCurrent;
};
