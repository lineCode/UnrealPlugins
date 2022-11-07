// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "FluidToolboxEditorAssetFactory.generated.h"

//Fluid Toolbox Camera Profiles Asset Factory
UCLASS()
class FLUIDTOOLBOXEDITOR_API UFluidToolboxCameraProfilesAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Toolbox Input Profiles Asset Factory
UCLASS()
class FLUIDTOOLBOXEDITOR_API UFluidToolboxInputProfilesAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Toolbox Footstep Profiles Asset Factory
UCLASS()
class FLUIDTOOLBOXEDITOR_API UFluidToolboxFootstepProfilesAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};