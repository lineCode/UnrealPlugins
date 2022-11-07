// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidWaterSubsystem.h"

/*=============================================================================
Fluid Water Subsystem : Initialize / DeInitialize / Tick
=============================================================================*/

void UFluidWaterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//Initialize Once
	if (bIsInitialized) return;
	bIsInitialized = true;
}

void UFluidWaterSubsystem::Deinitialize()
{
	bIsInitialized = false;
}

void UFluidWaterSubsystem::Tick(float DeltaTime)
{
	//Return If Not Initialized
	if (!bIsInitialized) return;
	
	//Tick Waves As FFT
	TickWavesFFT(DeltaTime);
}

bool UFluidWaterSubsystem::IsTickable() const
{
	return !IsTemplate() && bIsInitialized;
}

bool UFluidWaterSubsystem::IsTickableInEditor() const
{
	return false;
}

bool UFluidWaterSubsystem::IsTickableWhenPaused() const
{
	return false;
}

TStatId UFluidWaterSubsystem::GetStatId() const
{
	return TStatId();
}

/*=============================================================================
Fluid Water Subsystem : Waves
=============================================================================*/

static TAutoConsoleVariable<int32> CDebugFluidWaterHUDWavesRenderTargetsDebug(
	TEXT("Fluid.Water.Waves.HUD.Render.Targets.Debug"),
	0,
	TEXT("Fluid.Water.Waves.HUD.Render.Targets.Debug\n")
	TEXT("<=0: off \n")
	TEXT("  1: On\n"),
	ECVF_SetByConsole);

void UFluidWaterSubsystem::InitializeWavesAsFFT(const EFluidWaterResolution Resolution, UTextureRenderTarget2D* GaussDistributionRenderTarget,UTextureRenderTarget2D* PHKplusRenderTarget,UTextureRenderTarget2D* PHKminusRenderTarget, UTextureRenderTarget2D* FFTButterflyRenderTarget,UTextureRenderTarget2D* H0KplusTarget,UTextureRenderTarget2D* H0KminusTarget, UTextureRenderTarget2D* H0KplusH0KminusTarget, UTextureRenderTarget2D* DxRenderTarget,UTextureRenderTarget2D* DyRenderTarget,UTextureRenderTarget2D* DzRenderTarget, const FVector4 PhillipsSpectrumAVGF, const FVector2D PhillipsSpectrumW, const float PhillipsSpectrumLength)
{
	//Not Valid
	if (GaussDistributionRenderTarget == nullptr || FFTButterflyRenderTarget == nullptr || PHKminusRenderTarget == nullptr || PHKplusRenderTarget == nullptr)
		return;

	//Not Valid
	if (H0KplusTarget == nullptr || H0KminusTarget  == nullptr || H0KplusH0KminusTarget == nullptr)
		return;
	
	//Not Valid
	if (DxRenderTarget == nullptr || DyRenderTarget == nullptr || DzRenderTarget == nullptr)
		return;
	
	//Set Wave Mode
	WaveMode = FluidWaterWaveModeFFT;

	//Get World
	UWorld* World = GetWorld();
	
	//Get Resolution
	float ResolutionF = 0;
	if (Resolution == FluidWaterResolution64) ResolutionF = 64;
	else if (Resolution == FluidWaterResolution128) ResolutionF = 128;
	else if (Resolution == FluidWaterResolution256) ResolutionF = 256;
	else if (Resolution == FluidWaterResolution512) ResolutionF = 512;
	else if (Resolution == FluidWaterResolution1024) ResolutionF = 1024;
	else if (Resolution == FluidWaterResolution2048) ResolutionF = 2048;
	else if (Resolution == FluidWaterResolution4096) ResolutionF = 4096;
	else if (Resolution == FluidWaterResolution8192) ResolutionF = 8192;
	
	//Change Input Render Target Resolutions
	GaussDistributionRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	FFTButterflyRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	PHKplusRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	PHKminusRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	H0KplusTarget->ResizeTarget(ResolutionF,ResolutionF);
	H0KminusTarget->ResizeTarget(ResolutionF,ResolutionF);
	H0KplusH0KminusTarget->ResizeTarget(ResolutionF,ResolutionF);
	DxRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	DyRenderTarget->ResizeTarget(ResolutionF,ResolutionF);
	DzRenderTarget->ResizeTarget(ResolutionF,ResolutionF);

	//Set Internal Render Targets To Input Render Targets
	FFTGaussDistributionRenderTarget = GaussDistributionRenderTarget;
	FFTFFTButterflyRenderTarget = FFTButterflyRenderTarget;
	FFTPHKplusRenderTarget = PHKplusRenderTarget;
	FFTPHKminusRenderTarget = PHKminusRenderTarget;
	FFTH0KplusTarget = H0KplusTarget;
	FFTH0KminusTarget = H0KminusTarget;
	FFTH0KplusH0KminusTarget = H0KplusH0KminusTarget;
	FFTDxRenderTarget = DxRenderTarget;
	FFTDyRenderTarget = DyRenderTarget;
	FFTDzRenderTarget = DzRenderTarget;

	//FFT
	{
		//Compute Gauss Distribution (Only Needs To Be Done Once At Wave Initialization)
		FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution::EnqueueRenderCommand(FFTGaussDistributionRenderTarget);

		//Compute Gauss Distribution (Only Needs To Be Done Once At Wave Initialization)
		FFluidWaterComputeShaderWaterFFTButterfly::EnqueueRenderCommand(FFTButterflyRenderTarget);

		//Compute Waves (First Frame)
		SetWavesFFT(PhillipsSpectrumAVGF,PhillipsSpectrumW,PhillipsSpectrumLength);
	}
}

void UFluidWaterSubsystem::InitializeWavesAsGerstnerPlusPoints(const EFluidWaterResolution Resolution)
{
	//Set Wave Mode
	WaveMode = FluidWaterWaveModeGerstnerAndPoints;
	
	//Clamp Resolution
	//Resolution = FMath::Clamp(Resolution,256,2048);

	//Gerstner + Point
	{
	}
}

void UFluidWaterSubsystem::SetWavesFFT(const FVector4 PhillipsSpectrumAVGF, const FVector2D PhillipsSpectrumW, const float PhillipsSpectrumLength)
{
	//Phillips Spectrum (Only Need To Calculate Once)
	PhillipsSpectrumAVGFCurrent = PhillipsSpectrumAVGF;
	PhillipsSpectrumWCurrent = PhillipsSpectrumW;
	PhillipsSpectrumLengthCurrent = PhillipsSpectrumLength;
	FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus::EnqueueRenderCommand(FFTPHKplusRenderTarget, FFTPHKminusRenderTarget,FFTH0KplusTarget,FFTH0KminusTarget, FFTH0KplusH0KminusTarget, FVector4f(PhillipsSpectrumAVGF.X,PhillipsSpectrumAVGF.Y,PhillipsSpectrumAVGF.Z,PhillipsSpectrumAVGF.W),FVector2f(PhillipsSpectrumW.X,PhillipsSpectrumW.Y),PhillipsSpectrumLength);
}

void UFluidWaterSubsystem::TickWavesFFT(const float DeltaTime)
{
	if (WaveMode != FluidWaterWaveModeFFT)
		return;

	//Height Fields And Displacement Textures
	FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement::EnqueueRenderCommand(FFTDxRenderTarget,FFTDyRenderTarget,FFTDzRenderTarget);
}

void UFluidWaterSubsystem::DrawHUDDebugFFT(AHUD* HUD)
{
	//If Wave Mode Is FFT
	if (WaveMode != FluidWaterWaveModeFFT || HUD == nullptr)
		return;
	
	//Colors
	FLinearColor DarkGray = FColor(10,10,10);
	FLinearColor Black = FColor(0,0,0);

	//FFT Render Targets (No Debug)
	if (CDebugFluidWaterHUDWavesRenderTargetsDebug.GetValueOnAnyThread() == 1)
	{
		//G
		HUDDrawRenderTarget(HUD,FFTGaussDistributionRenderTarget,DarkGray,"On Waves Init","G",0,1,false);
		HUDDrawRenderTarget(HUD,FFTFFTButterflyRenderTarget,DarkGray,"","iFFT (Butterfly)",0,0,false);  // Just To Add Box To Look Nice
		
		//Ho(K) + Ho(-K)
		HUDDrawRenderTarget(HUD,FFTH0KplusH0KminusTarget,Black,"On Waves Changed","Ho(K) + Ho(-K)",1,1,false);
		HUDDrawRenderTarget(HUD,nullptr,Black,"","",1,0,false); // Just To Add Box To Look Nice

		//H | D
		HUDDrawRenderTarget(HUD,FFTDxRenderTarget,DarkGray,"On Time Changed","Dx",2,1,false);
		HUDDrawRenderTarget(HUD,FFTDyRenderTarget,DarkGray,"","Dy",3,1,false); 
		HUDDrawRenderTarget(HUD,FFTDzRenderTarget,DarkGray,"","Dz",4,1,false);
		HUDDrawRenderTarget(HUD,nullptr,DarkGray,"","",2,0,false); // Just To Add Box To Look Nice
		HUDDrawRenderTarget(HUD,nullptr,DarkGray,"","",3,0,false); // Just To Add Box To Look Nice
		HUDDrawRenderTarget(HUD,nullptr,DarkGray,"","",4,0,false); // Just To Add Box To Look Nice
	}
	
	//FFT Render Targets (With Debug)
	if (CDebugFluidWaterHUDWavesRenderTargetsDebug.GetValueOnAnyThread() == 2)
	{
		//iFFT (Butterfly)
		HUDDrawRenderTarget(HUD,FFTFFTButterflyRenderTarget,DarkGray,"","iFFT (Butterfly)",1,1,false);
		
		//G
		HUDDrawRenderTarget(HUD,FFTGaussDistributionRenderTarget,DarkGray,"On Waves Init","G(K)",0,1,true);
		HUDDrawRenderTarget(HUD,FFTGaussDistributionRenderTarget,DarkGray,"","G(-K)",0,0,true);
		HUDDrawRenderTarget(HUD,FFTGaussDistributionRenderTarget,DarkGray,"","G(K) + G(-K)",1,0,false);
			
		//Phillips Spectrum
		HUDDrawRenderTarget(HUD,FFTPHKplusRenderTarget,Black,"On Waves Changed","Ph(K)",2,1,true);
		HUDDrawRenderTarget(HUD,FFTPHKminusRenderTarget,Black,"","Ph(-K)",2,0,true);

		//Ho(K) / Ho(-K) / Ho(K) + Ho(-K)
		HUDDrawRenderTarget(HUD,FFTH0KplusTarget,Black,"","Ho(K)",3,1,true);
		HUDDrawRenderTarget(HUD,FFTH0KminusTarget,Black,"","Ho(-K)",3,0,true);
		HUDDrawRenderTarget(HUD,FFTH0KplusH0KminusTarget,Black,"","Ho(K) + Ho(-K)",4,1,false);
		HUDDrawRenderTarget(HUD,nullptr,Black,"","",4,0,false); // Just To Add Box To Look Nice

		//H | D
		HUDDrawRenderTarget(HUD,FFTDxRenderTarget,DarkGray,"On Time Changed","Hx",5,1,true);
		HUDDrawRenderTarget(HUD,FFTDyRenderTarget,DarkGray,"","Hy",6,1,true);
		HUDDrawRenderTarget(HUD,FFTDzRenderTarget,DarkGray,"","Hz",7,1,true);
		HUDDrawRenderTarget(HUD,FFTDxRenderTarget,DarkGray,"","Dx",5,0,false);
		HUDDrawRenderTarget(HUD,FFTDyRenderTarget,DarkGray,"","Dy",6,0,false);
		HUDDrawRenderTarget(HUD,FFTDzRenderTarget,DarkGray,"","Dz",7,0,false);
	}
}

void UFluidWaterSubsystem::HUDDrawRenderTarget(AHUD* HUD, UTextureRenderTarget2D* RenderTarget2D, FLinearColor BackgroundLinearColor, FString Header, FString Footer, float X, float Y, bool bIsDebugOnly)
{
	//Not Valid
	if (X < 0 || HUD == nullptr)
		return;
	
	//Get Texture Screen Size
	const float TextureSize = 150;
	const float Padding = 10;

	//Get Viewport
	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
	
	//Get Positions
	const float XPos = (TextureSize + Padding * 2) * X;
	const float YPos = ViewportSize.Y - TextureSize - Padding * 2 - (TextureSize + Padding * 4) * Y;
	const float YPosT = YPos + TextureSize + 1;

	//Footer Text Color
	const FColor FooterTextColor = bIsDebugOnly ? FColor(255,80,80,255) : FColor::White;
	
	//Draw Hud
	HUD->DrawRect(BackgroundLinearColor,XPos,YPos - Padding * 2,TextureSize + Padding * 2, Padding * 2 + YPos + TextureSize);
	HUD->DrawText(Header,FColor::White,XPos + Padding,YPos - Padding * 2 + 3);
	if (RenderTarget2D)
		HUD->DrawTexture(RenderTarget2D,XPos + Padding,YPos,TextureSize,TextureSize,0,0,1,1);
	HUD->DrawText(Footer,FooterTextColor,XPos + Padding,YPosT);
}
