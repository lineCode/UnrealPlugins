// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidToolboxEditorAssetTypeActions.h"
#include "FluidToolboxInput.h"
#include "FluidToolboxCamera.h"
#include "FluidToolboxFootsteps.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Camera Data Set Asset Type Actions

FluidToolboxCameraProfilesAssetTypeActions::FluidToolboxCameraProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FluidToolboxCameraProfilesAssetTypeActions::GetName() const
{
	return FText::FromString("Fluid Toolbox Camera Profiles");
}

FColor FluidToolboxCameraProfilesAssetTypeActions::GetTypeColor() const
{
	return FColor(0, 0, 0);
}

UClass* FluidToolboxCameraProfilesAssetTypeActions::GetSupportedClass() const
{
	return  UFluidToolboxCameraProfiles::StaticClass();
}

uint32 FluidToolboxCameraProfilesAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Input Asset Actions

FluidToolboxInputProfilesAssetTypeActions::FluidToolboxInputProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FluidToolboxInputProfilesAssetTypeActions::GetName() const
{
	return FText::FromString("Fluid Toolbox Input Profiles");
}

FColor FluidToolboxInputProfilesAssetTypeActions::GetTypeColor() const
{
	return FColor(0, 0, 0);
}

UClass* FluidToolboxInputProfilesAssetTypeActions::GetSupportedClass() const
{
	return  UFluidToolboxInputProfiles::StaticClass();
}

uint32 FluidToolboxInputProfilesAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Toolbox Footstep Asset Actions

FluidToolboxFootstepProfilesAssetTypeActions::FluidToolboxFootstepProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FluidToolboxFootstepProfilesAssetTypeActions::GetName() const
{
	return FText::FromString("Fluid Toolbox Footstep Profiles");
}

FColor FluidToolboxFootstepProfilesAssetTypeActions::GetTypeColor() const
{
	return FColor(0, 0, 0);
}

UClass* FluidToolboxFootstepProfilesAssetTypeActions::GetSupportedClass() const
{
	return  UFluidToolboxFootstepProfiles::StaticClass();
}

uint32 FluidToolboxFootstepProfilesAssetTypeActions::GetCategories()
{
	return MyAssetCategory;
}