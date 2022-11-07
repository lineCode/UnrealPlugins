// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "FluidAstroRuntime.h"
#include "FluidAstroEditorAssetFactory.h"
#include "FluidAstroEditorAssetTypeActions.h"
#include "FluidAstroStarMapModelHyqV3Editor.h"
#include "CoreMinimal.h"
#include "IAssetTypeActions.h"
#include "AssetToolsModule.h"
#include "Modules/ModuleManager.h"

class FFluidAstroEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};
