// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"

//Fluid Toolbox Camera Profiles Asset Actions
class FLUIDTOOLBOXEDITOR_API FluidToolboxCameraProfilesAssetTypeActions : public FAssetTypeActions_Base
{
    public:
    FluidToolboxCameraProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
   
    private:
    EAssetTypeCategories::Type MyAssetCategory;
};

//Fluid Toolbox Input Asset Actions
class FLUIDTOOLBOXEDITOR_API FluidToolboxInputProfilesAssetTypeActions : public FAssetTypeActions_Base
{
public:
    FluidToolboxInputProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
   
private:
    EAssetTypeCategories::Type MyAssetCategory;
};

//Fluid Toolbox Footstep Asset Actions
class FLUIDTOOLBOXEDITOR_API FluidToolboxFootstepProfilesAssetTypeActions : public FAssetTypeActions_Base
{
public:
    FluidToolboxFootstepProfilesAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
   
private:
    EAssetTypeCategories::Type MyAssetCategory;
};