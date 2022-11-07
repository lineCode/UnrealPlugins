// Fill out your copyright notice in the Description page of Project Settings.


#include "FluidToolboxEditorAssetFactory.h"
#include "FluidToolboxInput.h"
#include "FluidToolboxCamera.h"
#include "FluidToolboxFootsteps.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Toolbox Camera Data Set Asset Factory

UFluidToolboxCameraProfilesAssetFactory::UFluidToolboxCameraProfilesAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidToolboxCameraProfiles::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidToolboxCameraProfilesAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidToolboxCameraProfiles* NewObjectAsset = NewObject<UFluidToolboxCameraProfiles>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Toolbox Input Asset Asset Factory

UFluidToolboxInputProfilesAssetFactory::UFluidToolboxInputProfilesAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidToolboxInputProfiles::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidToolboxInputProfilesAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidToolboxInputProfiles* NewObjectAsset = NewObject<UFluidToolboxInputProfiles>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fluid Toolbox Footstep Asset Asset Factory

UFluidToolboxFootstepProfilesAssetFactory::UFluidToolboxFootstepProfilesAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidToolboxFootstepProfiles::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidToolboxFootstepProfilesAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidToolboxFootstepProfiles* NewObjectAsset = NewObject<UFluidToolboxFootstepProfiles>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}