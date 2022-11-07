// Copyright Epic Games, Inc. All Rights Reserved.

#include "FluidInventoryEditor.h"
#include "FluidInventoryEditorAsset.h"

#define LOCTEXT_NAMESPACE "FFluidInventoryEditorModule"

void FFluidInventoryEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    EAssetTypeCategories::Type Catagory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Fluid")), FText::FromString("Fluid"));
    TSharedPtr<IAssetTypeActions> Asset = MakeShareable(new FluidInventoryDataSetEditorAssetTypeActions(Catagory));
    AssetTools.RegisterAssetTypeActions(Asset.ToSharedRef());
    CreatedAssetTypeActions.Add(Asset);
}

void FFluidInventoryEditorModule::ShutdownModule()
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
	
IMPLEMENT_MODULE(FFluidInventoryEditorModule, FluidInventoryEditor)
