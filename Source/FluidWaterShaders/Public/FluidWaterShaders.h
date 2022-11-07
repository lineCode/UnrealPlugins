// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidWaterComputeShaderWaterFFT.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFluidWaterShadersModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
