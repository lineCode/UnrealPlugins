// Author: nfgrep
// 2021

#include "FluidWaterComputeShaderWaterFFT.h"
#include "RenderGraph.h"
#include "RenderTargetPool.h"
#include "Engine/TextureRenderTarget2D.h"

/*=============================================================================
Fluid water Compute Shader FFT: Guass Distribution
=============================================================================*/

// Associate FFluidWaterComputeShaderFFTGaussDistribution With HLSL Shader Code As Compute Shader (Use Function MainCompute)
IMPLEMENT_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution, "/Plugin/FluidWater/Private/FluidWaterComputeShaderWaterFFT.usf", "ComputeGuassDistribution", SF_Compute);

// Determine If Shader Have Permutation Based On Platform
bool FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

// Static Helper Function To Run Compute Shader 
void FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution::EnqueueRenderCommand(UTextureRenderTarget2D* GuassDistributionRenderTargetRWTexture)
{
	TShaderMapRef<FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	UTextureRenderTarget2D* RenderTargetParam = GuassDistributionRenderTargetRWTexture;

	ENQUEUE_RENDER_COMMAND(ComputeShader)(
		[
			ComputeShader,
			RenderTargetParam
		](FRHICommandListImmediate& RHICmdList)
		{
			ComputeShader->BuildAndExecuteGraph(RHICmdList, RenderTargetParam);
		});
}

// Used To Build Render-Graph For Single Frame And Execute It
void FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution::BuildAndExecuteGraph(FRHICommandListImmediate &RHICmdList, UTextureRenderTarget2D* GuassDistributionRenderTargetRWTexture)
{
	//Get Our Global Compute Shader
	const TShaderMapRef<FFluidWaterComputeShaderWaterFFTInitializeGaussDistribution> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	
	//Create RDG Graph Builder (Used To Add Buffers And Passes To Render-Graph)
	FRDGBuilder GraphBuilder(RHICmdList);

	//Have GraphBuilder Allocate Memory For Our Shader Parameters (Ones We Declared in FFluidWaterComputeShader in .h)
	FParameters* PassParameters = GraphBuilder.AllocParameters<FParameters>();
	
	// Create Texture For Compute Shader To Write To
	FRDGTextureRef GuassDistributionTextureRef;
	{
		//Make A Texture Description
		const FRDGTextureDesc GuassDistributionTextureDesc = FRDGTextureDesc::Create2D(FIntPoint(GuassDistributionRenderTargetRWTexture->SizeX, GuassDistributionRenderTargetRWTexture->SizeY),PF_FloatRGBA, FClearValueBinding(), TexCreate_UAV, 1, 1);

		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		GuassDistributionTextureRef = GraphBuilder.CreateTexture(GuassDistributionTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc OutTextureUAVDesc(GuassDistributionTextureRef);
		
		//Set Pass Parameters Output Texture 
		PassParameters->GuassDistributionRWTexture = GraphBuilder.CreateUAV(OutTextureUAVDesc);
	}

	//Pass In Shared Resolution
	PassParameters->SharedResolution = GuassDistributionRenderTargetRWTexture->SizeX;
	
	//Add Global Shader As Pass To Render Graph ( 8,8,1 Is Threads And Must Match As In Shader );
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("Compute Pass"), ComputeShader, PassParameters, FIntVector(PassParameters->SharedResolution / 8, PassParameters->SharedResolution / 8, 1));

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> PooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(GuassDistributionTextureRef, &PooledComputeTarget);

	// Execute Graph Builder (The Compute Shader Basically)
	GraphBuilder.Execute();

	// Queue The UAV The Shader Wrote For Extraction
	RHICmdList.CopyToResolveTarget(PooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, GuassDistributionRenderTargetRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
}

/*=============================================================================
Fluid water Compute Shader FFT: Phillips Spectrum H0(K) HO(-K)
=============================================================================*/

// Associate FFluidWaterComputeShaderFFTGaussDistribution With HLSL Shader Code As Compute Shader (Use Function MainCompute)
IMPLEMENT_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus, "/Plugin/FluidWater/Private/FluidWaterComputeShaderWaterFFT.usf", "ComputePhillipsSpectrumH0KplusH0KMinus", SF_Compute);

// Determine If Shader Have Permutation Based On Platform
bool FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

// Static Helper Function To Run Compute Shader 
void FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus::EnqueueRenderCommand(UTextureRenderTarget2D* PHKPlusRWTexture,UTextureRenderTarget2D* PHKminusRWTexture,UTextureRenderTarget2D* H0KplusRWTexture,UTextureRenderTarget2D* H0KminusRWTexture, UTextureRenderTarget2D* H0KplusH0KminusRWTexture, FVector4f PhillipsSpectrumAVGF, FVector2f PhillipsSpectrumW, float PhillipsSpectrumLength)
{
	TShaderMapRef<FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	ENQUEUE_RENDER_COMMAND(ComputeShader)(
		[
			ComputeShader,
			PHKPlusRWTexture,
			PHKminusRWTexture,
			H0KplusRWTexture,
			H0KminusRWTexture,
			H0KplusH0KminusRWTexture,
			PhillipsSpectrumAVGF,
			PhillipsSpectrumW,
			PhillipsSpectrumLength
		](FRHICommandListImmediate& RHICmdList)
		{
			ComputeShader->BuildAndExecuteGraph(RHICmdList,PHKPlusRWTexture,PHKminusRWTexture,H0KplusRWTexture,H0KminusRWTexture, H0KplusH0KminusRWTexture, PhillipsSpectrumAVGF,PhillipsSpectrumW,PhillipsSpectrumLength);
		});
}

// Used To Build Render-Graph For Single Frame And Execute It
void FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus::BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList,UTextureRenderTarget2D* PHKPlusRWTexture,UTextureRenderTarget2D* PHKminusRWTexture,UTextureRenderTarget2D* H0KplusRWTexture,UTextureRenderTarget2D* H0KminusRWTexture, UTextureRenderTarget2D* H0KplusH0KminusRWTexture, FVector4f PhillipsSpectrumAVGF, FVector2f PhillipsSpectrumW, float PhillipsSpectrumLength)
{
	//Get Our Global Compute Shader
	const TShaderMapRef<FFluidWaterComputeShaderWaterFFTPhillipsSpectrumH0KplusH0Kminus> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	
	//Create RDG Graph Builder (Used To Add Buffers And Passes To Render-Graph)
	FRDGBuilder GraphBuilder(RHICmdList);

	//Have GraphBuilder Allocate Memory For Our Shader Parameters (Ones We Declared in FFluidWaterComputeShader in .h)
	FParameters* PassParameters = GraphBuilder.AllocParameters<FParameters>();
	
	// Create Texture For Compute Shader To Write To
	FRDGTextureRef PHKplusTextureRef;
	FRDGTextureRef PHKminusTextureRef;
	FRDGTextureRef H0KplusTextureRef;
	FRDGTextureRef H0KminusTextureRef;
	FRDGTextureRef H0KplusH0KminusTextureRef;
	{
		//Make A Texture Description
		const FRDGTextureDesc SharedTextureDesc = FRDGTextureDesc::Create2D(FIntPoint(H0KplusH0KminusRWTexture->SizeX, H0KplusH0KminusRWTexture->SizeY),PF_FloatRGBA, FClearValueBinding(), TexCreate_UAV, 1, 1);

		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		PHKplusTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc PHKplusOutTextureUAVDesc(PHKplusTextureRef);
		PassParameters->PHKplusRenderRWTexture = GraphBuilder.CreateUAV(PHKplusOutTextureUAVDesc);

		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		PHKminusTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc PHKminusOutTextureUAVDesc(PHKminusTextureRef);
		PassParameters->PHKminusRenderRWTexture = GraphBuilder.CreateUAV(PHKminusOutTextureUAVDesc);
		
		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		H0KplusTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc H0KplusOutTextureUAVDesc(H0KplusTextureRef);
		PassParameters->H0KplusRWTexture = GraphBuilder.CreateUAV(H0KplusOutTextureUAVDesc);

		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		H0KminusTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc H0KminusOutTextureUAVDesc(H0KminusTextureRef);
		PassParameters->H0KminusRWTexture = GraphBuilder.CreateUAV(H0KminusOutTextureUAVDesc);
		
		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		H0KplusH0KminusTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc H0KplusH0KminusOutTextureUAVDesc(H0KplusH0KminusTextureRef);
		PassParameters->H0KplusH0KminusRWTexture = GraphBuilder.CreateUAV(H0KplusH0KminusOutTextureUAVDesc);
	}

	//Pass In Shared Resolution
	PassParameters->PhillipsSpectrumW = PhillipsSpectrumW;
	PassParameters->PhillipsSpectrumAVGF = PhillipsSpectrumAVGF;
	PassParameters->PhillipsSpectrumLength = PhillipsSpectrumLength;
	PassParameters->SharedResolution = H0KplusH0KminusRWTexture->SizeX;
	
	//Add Global Shader As Pass To Render Graph ( 8,8,1 Is Threads And Must Match As In Shader );
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("Compute Pass"), ComputeShader, PassParameters, FIntVector(PassParameters->SharedResolution/ 8, PassParameters->SharedResolution/ 8, 1));

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> PHKplusPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(PHKplusTextureRef, &PHKplusPooledComputeTarget);

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> PHKminusPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(PHKminusTextureRef, &PHKminusPooledComputeTarget);

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> H0KplusPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(H0KplusTextureRef, &H0KplusPooledComputeTarget);

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> H0KminusPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(H0KminusTextureRef, &H0KminusPooledComputeTarget);
	
	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> H0KplusH0KminusPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(H0KplusH0KminusTextureRef, &H0KplusH0KminusPooledComputeTarget);

	// Execute Graph Builder (The Compute Shader Basically)
	GraphBuilder.Execute();

	// Queue The UAV The Shader Wrote For Extraction
	RHICmdList.CopyToResolveTarget(PHKplusPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, PHKPlusRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(PHKminusPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, PHKminusRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(H0KplusPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, H0KplusRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(H0KminusPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, H0KminusRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(H0KplusH0KminusPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, H0KplusH0KminusRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
}

/*=============================================================================
Fluid water Compute Shader FFT: FFT Butterfly
=============================================================================*/

// Associate FFluidWaterComputeShaderFFTGaussDistribution With HLSL Shader Code As Compute Shader (Use Function MainCompute)
IMPLEMENT_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTButterfly, "/Plugin/FluidWater/Private/FluidWaterComputeShaderWaterFFT.usf", "ComputeFFTButterfly", SF_Compute);

// Determine If Shader Have Permutation Based On Platform
bool FFluidWaterComputeShaderWaterFFTButterfly::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

// Static Helper Function To Run Compute Shader 
void FFluidWaterComputeShaderWaterFFTButterfly::EnqueueRenderCommand(UTextureRenderTarget2D* FFTTexture)
{
	TShaderMapRef<FFluidWaterComputeShaderWaterFFTButterfly> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	ENQUEUE_RENDER_COMMAND(ComputeShader)(
		[
			ComputeShader,
			FFTTexture
		](FRHICommandListImmediate& RHICmdList)
		{
			ComputeShader->BuildAndExecuteGraph(RHICmdList, FFTTexture);
		});
}

// Used To Build Render-Graph For Single Frame And Execute It
void FFluidWaterComputeShaderWaterFFTButterfly::BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* FFTTexture)
{
	//Get Our Global Compute Shader
	const TShaderMapRef<FFluidWaterComputeShaderWaterFFTButterfly> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	
	//Create RDG Graph Builder (Used To Add Buffers And Passes To Render-Graph)
	FRDGBuilder GraphBuilder(RHICmdList);

	//Have GraphBuilder Allocate Memory For Our Shader Parameters (Ones We Declared in FFluidWaterComputeShader in .h)
	FParameters* PassParameters = GraphBuilder.AllocParameters<FParameters>();
	
	// Create Texture For Compute Shader To Write To
	FRDGTextureRef FFTTextureRef;
	{
		//Make A Texture Description
		const FRDGTextureDesc SharedTextureDesc = FRDGTextureDesc::Create2D(FIntPoint(FFTTexture->SizeX, FFTTexture->SizeY),PF_FloatRGBA, FClearValueBinding(), TexCreate_UAV, 1, 1);
		
		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		FFTTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc FFTButterflyOutTextureUAVDesc(FFTTextureRef);
		
		//Set Pass Parameters Output Texture
		PassParameters->FFTButterflyRWTexture = GraphBuilder.CreateUAV(FFTButterflyOutTextureUAVDesc);
	}

	//Pass In Shared Resolution
	PassParameters->SharedResolution = FFTTexture->SizeX;
	
	//Add Global Shader As Pass To Render Graph ( 8,8,1 Is Threads And Must Match As In Shader );
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("Compute Pass"), ComputeShader, PassParameters, FIntVector(PassParameters->SharedResolution/ 8, PassParameters->SharedResolution/ 8, 1));
	
	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> FFTPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(FFTTextureRef, &FFTPooledComputeTarget);

	// Execute Graph Builder (The Compute Shader Basically)
	GraphBuilder.Execute();

	// Queue The UAV The Shader Wrote For Extraction
	RHICmdList.CopyToResolveTarget(FFTPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, FFTTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
}

/*=============================================================================
Fluid water Compute Shader FFT: HeightFields Plus Displacement
=============================================================================*/

// Associate FFluidWaterComputeShaderFFTGaussDistribution With HLSL Shader Code As Compute Shader (Use Function MainCompute)
IMPLEMENT_GLOBAL_SHADER(FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement, "/Plugin/FluidWater/Private/FluidWaterComputeShaderWaterFFT.usf", "ComputeHeightFieldsPlusDisplacement", SF_Compute);

// Determine If Shader Have Permutation Based On Platform
bool FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

// Static Helper Function To Run Compute Shader 
void FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement::EnqueueRenderCommand(UTextureRenderTarget2D* DxTexture, UTextureRenderTarget2D* DyTexture, UTextureRenderTarget2D* DzTexture)
{
	TShaderMapRef<FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	ENQUEUE_RENDER_COMMAND(ComputeShader)(
		[
			ComputeShader,
			DxTexture,
			DyTexture,
			DzTexture
		](FRHICommandListImmediate& RHICmdList)
		{
			ComputeShader->BuildAndExecuteGraph(RHICmdList, DxTexture, DyTexture, DzTexture);
		});
}

// Used To Build Render-Graph For Single Frame And Execute It
void FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement::BuildAndExecuteGraph(FRHICommandListImmediate& RHICmdList, UTextureRenderTarget2D* DxRWTexture, UTextureRenderTarget2D* DyRWTexture, UTextureRenderTarget2D* DzRWTexture)
{
	//Get Our Global Compute Shader
	const TShaderMapRef<FFluidWaterComputeShaderWaterFFTHeightFieldsPlusDisplacement> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	
	//Create RDG Graph Builder (Used To Add Buffers And Passes To Render-Graph)
	FRDGBuilder GraphBuilder(RHICmdList);

	//Have GraphBuilder Allocate Memory For Our Shader Parameters (Ones We Declared in FFluidWaterComputeShader in .h)
	FParameters* PassParameters = GraphBuilder.AllocParameters<FParameters>();
	
	// Create Texture For Compute Shader To Write To
	FRDGTextureRef DxTextureRef;
	FRDGTextureRef DyTextureRef;
	FRDGTextureRef DzTextureRef;
	{
		//Make A Texture Description
		const FRDGTextureDesc SharedTextureDesc = FRDGTextureDesc::Create2D(FIntPoint(DxRWTexture->SizeX, DxRWTexture->SizeY),PF_FloatRGBA, FClearValueBinding(), TexCreate_UAV, 1, 1);
		
		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref | Pass As Parameter
		DxTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc DxOutTextureUAVDesc(DxTextureRef);
		PassParameters->DxRWTexture = GraphBuilder.CreateUAV(DxOutTextureUAVDesc);

		// Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		DyTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc DyOutTextureUAVDesc(DyTextureRef);
		PassParameters->DyRWTexture = GraphBuilder.CreateUAV(DyOutTextureUAVDesc);

		//Allocate Memory With Texture Description | Store It In A Reference | Make a UAV Description Using Texture Ref
		DzTextureRef = GraphBuilder.CreateTexture(SharedTextureDesc, TEXT("Compute_Out_Texture"));
		const FRDGTextureUAVDesc DzOutTextureUAVDesc(DzTextureRef);
		PassParameters->DzRWTexture = GraphBuilder.CreateUAV(DzOutTextureUAVDesc);
	}

	//Pass In Shared Resolution
	PassParameters->SharedResolution = DxRWTexture->SizeX;
	
	//Add Global Shader As Pass To Render Graph ( 8,8,1 Is Threads And Must Match As In Shader );
	FComputeShaderUtils::AddPass(GraphBuilder, RDG_EVENT_NAME("Compute Pass"), ComputeShader, PassParameters, FIntVector(PassParameters->SharedResolution/ 8, PassParameters->SharedResolution/ 8, 1));
	
	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> DxPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(DxTextureRef, &DxPooledComputeTarget);

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> DyPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(DyTextureRef, &DyPooledComputeTarget);

	//Copy Result Of Compute Shader From UAV to Pooled Render Target (As A Ptr)
	TRefCountPtr<IPooledRenderTarget> DzPooledComputeTarget;
	GraphBuilder.QueueTextureExtraction(DzTextureRef, &DzPooledComputeTarget);

	// Execute Graph Builder (The Compute Shader Basically)
	GraphBuilder.Execute();

	// Queue The UAV The Shader Wrote For Extraction
	RHICmdList.CopyToResolveTarget(DxPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, DxRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(DyPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, DyRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
	RHICmdList.CopyToResolveTarget(DzPooledComputeTarget.GetReference()->GetRenderTargetItem().TargetableTexture, DzRWTexture->GetRenderTargetResource()->TextureRHI, FResolveParams());
}