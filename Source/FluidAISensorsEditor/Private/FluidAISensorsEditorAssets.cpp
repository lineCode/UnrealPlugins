// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAISensorsEditorAssets.h"

#include "FluidAISensorsSubsystem.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FluidAI Sensors Data Asset Editor Asset Factory

UFluidAISensorsDataAssetEditorAssetFactory::UFluidAISensorsDataAssetEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidAISensorsDataAsset::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidAISensorsDataAssetEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
   UFluidAISensorsDataAsset* NewObjectAsset = NewObject<UFluidAISensorsDataAsset>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FluidAI Sensors Data Asset Editor Asset Type Actions

FluidAISensorsDataAssetEditorAssetTypeActions::FluidAISensorsDataAssetEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FluidAISensorsDataAssetEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid AI Sensors Data Asset");
}

FColor FluidAISensorsDataAssetEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidAISensorsDataAssetEditorAssetTypeActions::GetSupportedClass() const
{
    return UFluidAISensorsDataAsset::StaticClass();
}

uint32 FluidAISensorsDataAssetEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}