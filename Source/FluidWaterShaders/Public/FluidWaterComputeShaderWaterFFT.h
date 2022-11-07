
#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "Engine/Texture2D.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShaderParameterStruct.h"

//Fluid water Compute Shader FFT: Guass Distribution
class FLUIDWATERSHADERS_API FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution : public FGlobalShader
{
	//Paramaters
	DECLARE_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution)
	SHADER_USE_PARAMETER_STRUCT(FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution, FGlobalShader)
		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, ) 
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, GuassDistributionRWTexture)
		SHADER_PARAMETER(float, SharedResolution)
	END_SHADER_PARAMETER_STRUCT()
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void EnqueueRenderCommand(UTextureRenderTarget2D* GuassDistributionRenderTargetRWTexture);

	void BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* GuassDistributionRenderTargetRWTexture);
};

//Fluid water Compute Shader FFT: Phillips Spectrum H0(K) HO(-K)
class FLUIDWATERSHADERS_API FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus : public FGlobalShader
{
	//Paramaters
	DECLARE_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus)
	SHADER_USE_PARAMETER_STRUCT(FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus, FGlobalShader)
		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, PHKplusRenderRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, PHKminusRenderRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, H0KplusRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, H0KminusRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, H0KplusH0KminusRWTexture)
		SHADER_PARAMETER(FVector2f, PhillipsSpectrumW)
		SHADER_PARAMETER(FVector4f, PhillipsSpectrumAVGF)
		SHADER_PARAMETER(float,PhillipsSpectrumLength)
		SHADER_PARAMETER(float, SharedResolution)
	END_SHADER_PARAMETER_STRUCT()
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void EnqueueRenderCommand(UTextureRenderTarget2D* PHKPlusRWTexture,UTextureRenderTarget2D* PHKminusRWTexture,UTextureRenderTarget2D* H0KplusRWTexture,UTextureRenderTarget2D* H0KminusRWTexture, UTextureRenderTarget2D* H0KplusH0KminusRWTexture, FVector4f PhillipsSpectrumAVGF, FVector2f PhillipsSpectrumW, float PhillipsSpectrumLength);

	void BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* PHKPlusRWTexture,UTextureRenderTarget2D* PHKminusRWTexture,UTextureRenderTarget2D* H0KplusRWTexture,UTextureRenderTarget2D* H0KminusRWTexture, UTextureRenderTarget2D* H0KplusH0KminusRWTexture, FVector4f PhillipsSpectrumAVGF, FVector2f PhillipsSpectrumW, float PhillipsSpectrumLength);
};

//Fluid water Compute Shader FFT: FFT Butterfly
class FLUIDWATERSHADERS_API FFluidWaterComputeShaderWaterFFTButterfly : public FGlobalShader
{
	//Paramaters
	DECLARE_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTButterfly)
	SHADER_USE_PARAMETER_STRUCT(FFluidWaterComputeShaderWaterFFTButterfly, FGlobalShader)
		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, FFTButterflyRWTexture)
		SHADER_PARAMETER(float, SharedResolution)
	END_SHADER_PARAMETER_STRUCT()
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void EnqueueRenderCommand(UTextureRenderTarget2D* FFTTexture);

	void BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* FFTTexture);
};

//Fluid water Compute Shader FFT: HeightFields Plus Displacement
class FLUIDWATERSHADERS_API FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement : public FGlobalShader
{
	//Paramaters
	DECLARE_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement)
	SHADER_USE_PARAMETER_STRUCT(FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement, FGlobalShader)
		BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, DxRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, DyRWTexture)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<FVector4>, DzRWTexture)
		SHADER_PARAMETER(float, SharedResolution)
	END_SHADER_PARAMETER_STRUCT()
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void EnqueueRenderCommand(UTextureRenderTarget2D* DxTexture, UTextureRenderTarget2D* DyTexture, UTextureRenderTarget2D* DzTexture);

	void BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DxRWTexture, UTextureRenderTarget2D* DyRWTexture, UTextureRenderTarget2D* DzRWTexture);
};