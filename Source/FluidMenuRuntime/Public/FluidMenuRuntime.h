// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "FluidMenuBaseUserWidget.h"
#include "FluidMenuBPLibrary.h"
#include "FluidMenuItemBaseUserWidget.h"

class FFluidMenuRuntimeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
