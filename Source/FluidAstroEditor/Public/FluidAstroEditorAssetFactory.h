// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidAstroEditor.h"
#include "UnrealEd.h"
#include "FluidAstroEditorAssetFactory.generated.h"

//Fluid Astro Star Map Model HyqV3 Asset Factory
UCLASS()
class FLUIDASTROEDITOR_API UFluidAstroStarMapModelHyqV3AssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};