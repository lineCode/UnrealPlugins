// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FluidInventorySubsystem.h"
#include "FluidInventoryUI.h"
#include "Modules/ModuleManager.h"

class FFluidInventoryRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
