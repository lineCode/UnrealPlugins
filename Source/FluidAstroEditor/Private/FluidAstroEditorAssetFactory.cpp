// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidAstroEditorAssetFactory.h"

/*=============================================================================
Fluid Astro Star Map Model HyqV3 Asset Factory
=============================================================================*/

UFluidAstroStarMapModelHyqV3AssetFactory::UFluidAstroStarMapModelHyqV3AssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidAstroStarMapModelHyqV3::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidAstroStarMapModelHyqV3AssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidAstroStarMapModelHyqV3* NewObjectAsset = NewObject<UFluidAstroStarMapModelHyqV3>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}