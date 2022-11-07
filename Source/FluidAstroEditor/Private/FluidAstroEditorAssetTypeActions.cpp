// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAstroEditorAssetTypeActions.h"

/*=============================================================================
Fluid Astro Star Map Model HyqV3 Asset Type Actions
=============================================================================*/

FluidAstroStarMapModelHyqV3AssetTypeActions::FluidAstroStarMapModelHyqV3AssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FluidAstroStarMapModelHyqV3AssetTypeActions::GetName() const
{
	return FText::FromString("Fluid Astro Star Map Model HyqV3");
}

FColor FluidAstroStarMapModelHyqV3AssetTypeActions::GetTypeColor() const
{
	return FColor(0, 0, 0);
}

UClass* FluidAstroStarMapModelHyqV3AssetTypeActions::GetSupportedClass() const
{
	return UFluidAstroStarMapModelHyqV3::StaticClass();
}

uint32 FluidAstroStarMapModelHyqV3AssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

void FluidAstroStarMapModelHyqV3AssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UFluidAstroStarMapModelHyqV3* Asset = Cast<UFluidAstroStarMapModelHyqV3>(*ObjIt))
		{
			TSharedRef< FluidAstroStarMapModelHyqV3Editor> NewGraphEditor(new  FluidAstroStarMapModelHyqV3Editor());
			NewGraphEditor->Initialize(Mode, EditWithinLevelEditor, Asset);
		}
	}
}
