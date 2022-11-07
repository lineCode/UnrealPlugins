// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "AssetTypeActions_Base.h"
#include "FluidInventorySubsystem.h"
#include "IDetailsView.h"
#include "UObject/NoExportTypes.h"
#include "FluidInventoryEditorAsset.generated.h"

//Fluid Inventory Data Set Editor Asset Factory
UCLASS()
class UFluidInventoryDataSetEditorAssetFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};

//Fluid Inventory Data Set Editor Asset Type Actions
class FluidInventoryDataSetEditorAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FluidInventoryDataSetEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
   
private:
	EAssetTypeCategories::Type MyAssetCategory;
};

//Fluid Inventory Editor DataSet
class FLUIDINVENTORYEDITOR_API FluidInventoryEditorDataSet : public FAssetEditorToolkit, public FNotifyHook, public FGCObject
{
	
public:
	FluidInventoryEditorDataSet();
	~FluidInventoryEditorDataSet();

	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	// FAssetEditorToolkit | FSerializableObject interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void SaveAsset_Execute() override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	//Initialize
	void Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost,  UFluidInventoryDataSet* AssetIn);
	void InitializeDetailsView();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);

	//Spawn Tabs
	TSharedRef<SDockTab> SpawnTabDetails(const FSpawnTabArgs& Args);
	
protected:
	UFluidInventoryDataSet* Asset;
	FName AssetName;
	TSharedPtr<class IDetailsView> DetailsView;
};