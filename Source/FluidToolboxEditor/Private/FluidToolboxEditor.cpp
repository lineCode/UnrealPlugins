// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidToolboxEditor.h"
#include "FluidToolboxEditorAssetTypeActions.h"

#define LOCTEXT_NAMESPACE "FFluidToolboxEditorModule"

void FFluidToolboxEditorModule::StartupModule()
{
    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        EAssetTypeCategories::Type Catagory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));

        //Camera Asset
        const TSharedPtr<IAssetTypeActions> FluidToolboxCameraAsset = MakeShareable(new FluidToolboxCameraProfilesAssetTypeActions(Catagory));
        AssetTools.RegisterAssetTypeActions(FluidToolboxCameraAsset.ToSharedRef());
        CreatedAssetTypeActions.Add(FluidToolboxCameraAsset);
        
        //Input Asset
        const TSharedPtr<IAssetTypeActions> FluidToolboxInputAsset = MakeShareable(new FluidToolboxInputProfilesAssetTypeActions(Catagory));
        AssetTools.RegisterAssetTypeActions(FluidToolboxInputAsset.ToSharedRef());
        CreatedAssetTypeActions.Add(FluidToolboxInputAsset);

        //Footstep Asset
        const TSharedPtr<IAssetTypeActions> FluidToolboxFootstepAsset = MakeShareable(new FluidToolboxFootstepProfilesAssetTypeActions(Catagory));
        AssetTools.RegisterAssetTypeActions(FluidToolboxFootstepAsset.ToSharedRef());
        CreatedAssetTypeActions.Add(FluidToolboxFootstepAsset);
    }
}

void FFluidToolboxEditorModule::ShutdownModule()
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
	
IMPLEMENT_MODULE(FFluidToolboxEditorModule, FluidToolboxEditor)