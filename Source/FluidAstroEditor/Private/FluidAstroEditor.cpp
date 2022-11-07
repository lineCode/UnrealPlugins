// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidAstroEditor.h"

#define LOCTEXT_NAMESPACE "FFluidAstroEditorModule"

/*=============================================================================
Fluid Astro Astro Module
=============================================================================*/

void FFluidAstroEditorModule::StartupModule()
{
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		EAssetTypeCategories::Type Catagory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));

		//Fluid Astro Star map Model HyqV3 Asset Type Actions
		const TSharedPtr<IAssetTypeActions> AstroStarMapModelHyqV3AssetTypeActions = MakeShareable(new FluidAstroStarMapModelHyqV3AssetTypeActions(Catagory));
		AssetTools.RegisterAssetTypeActions(AstroStarMapModelHyqV3AssetTypeActions.ToSharedRef());
		CreatedAssetTypeActions.Add(AstroStarMapModelHyqV3AssetTypeActions);
	}
}

void FFluidAstroEditorModule::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
	}
	CreatedAssetTypeActions.Empty();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFluidAstroEditorModule, FluidAstroEditor)