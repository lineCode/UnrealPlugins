// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidAstroEditor.h"
#include "AssetTypeActions_Base.h"

//Fluid Astro Star Map Model HyqV3 Asset Type Actions
class FLUIDASTROEDITOR_API FluidAstroStarMapModelHyqV3AssetTypeActions : public FAssetTypeActions_Base
{
public:
    FluidAstroStarMapModelHyqV3AssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
   
    private:
    EAssetTypeCategories::Type MyAssetCategory;
};