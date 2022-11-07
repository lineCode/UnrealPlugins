// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"
#include "FluidAISensorsRuntime.h"
#include "FluidAISensorsEditorAssets.generated.h"

//FluidAI Sensors Data Asset Editor Asset Factory
UCLASS()
class UFluidAISensorsDataAssetEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//FluidAI Sensors Data Asset Editor Asset Type Actions
class FluidAISensorsDataAssetEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidAISensorsDataAssetEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
   
private:
	EAssetTypeCategories::Type MyAssetCategory;
};


