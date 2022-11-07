// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FluidAstroEditor.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "PropertyCustomizationHelpers.h"
#include "Slate/SObjectWidget.h"
#include "UnrealEd.h"

/**
 * 
 */

//Fluid Astro Star Map Model HyqV3  Editor
class FluidAstroStarMapModelHyqV3Editor : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
public:

	SLATE_BEGIN_ARGS(FluidAstroStarMapModelHyqV3Editor) { }
	SLATE_END_ARGS()

//Constructors
public:
	
	FluidAstroStarMapModelHyqV3Editor();
	~FluidAstroStarMapModelHyqV3Editor();

// FAssetEditorToolkit | FSerializableObject interface
public:
	
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;


//Register Tab Spawners
public:
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

//Initialize
public:
	
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidAstroStarMapModelHyqV3* AssetIn);
	void InitializeToolbar(FToolBarBuilder& ToolbarBuilder);
	
//Spawn Tabs
public:
	
	TSharedRef<SDockTab> SpawnTabEditorSettingsDetails(const FSpawnTabArgs& Args);

//Handles
public:
	
	void HandelToolbarLoadButtonPressed();
	
//Variables	
public:
	UFluidAstroStarMapModelHyqV3* Asset;
	FName AssetName;
	
	TSharedPtr<IDetailsView> EditorSettingsDetailView;
};