// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidInventoryEditorAsset.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Data Set Editor Asset Factory

UFluidInventoryDataSetEditorAssetFactory::UFluidInventoryDataSetEditorAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SupportedClass = UFluidInventoryDataSet::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

UObject* UFluidInventoryDataSetEditorAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    UFluidInventoryDataSet* NewObjectAsset = NewObject<UFluidInventoryDataSet>(InParent, Class, Name, Flags | RF_Transactional);
    return NewObjectAsset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Data Set Editor Asset Type Actions

FluidInventoryDataSetEditorAssetTypeActions::FluidInventoryDataSetEditorAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FluidInventoryDataSetEditorAssetTypeActions::GetName() const
{
    return FText::FromString("Fluid Inventory Data Set");
}

FColor FluidInventoryDataSetEditorAssetTypeActions::GetTypeColor() const
{
    return FColor(0, 0, 0);
}

UClass* FluidInventoryDataSetEditorAssetTypeActions::GetSupportedClass() const
{
    return  UFluidInventoryDataSet::StaticClass();
}

uint32 FluidInventoryDataSetEditorAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FluidInventoryDataSetEditorAssetTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UFluidInventoryDataSet* Asset = Cast<UFluidInventoryDataSet>(*ObjIt))
		{
			TSharedRef<FluidInventoryEditorDataSet> Editor(new FluidInventoryEditorDataSet());
			Editor->Initialize(Mode, EditWithinLevelEditor, Asset);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Fluid Inventory Editor DataSet

const FName DetailsID(TEXT("FluidArchitectCoreDetails"));

FluidInventoryEditorDataSet::FluidInventoryEditorDataSet()
{
}

FluidInventoryEditorDataSet::~FluidInventoryEditorDataSet()
{
}

void FluidInventoryEditorDataSet::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	//Details
	InTabManager->RegisterTabSpawner(DetailsID, FOnSpawnTab::CreateSP(this, &FluidInventoryEditorDataSet::SpawnTabDetails))
		.SetDisplayName(FText::FromString("Details"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GenericEditor.Tabs.Properties"));
}

void FluidInventoryEditorDataSet::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	InTabManager->UnregisterTabSpawner(DetailsID);
}

FName FluidInventoryEditorDataSet::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidInventoryEditorDataSet::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidInventoryEditorDataSet::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidInventoryEditorDataSet::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidInventoryEditorDataSet::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidInventoryEditorDataSet::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidInventoryEditorDataSet::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidInventoryEditorDataSet::SaveAsset_Execute()
{
	//Sort Inventory Archetype Alphabetically
	{
		TArray<FName> InventoryArchetypeKeys;
		Asset->InventoryArchetypes.GetKeys(InventoryArchetypeKeys);
		InventoryArchetypeKeys.Sort([](const FName& A, const FName& B) { return A.ToString() < B.ToString(); });

		auto InventoryArchetypesNew = TMap<FName,FFluidInventoryArchetype>();
		for (int i = 0; i < InventoryArchetypeKeys.Num(); i++)
			InventoryArchetypesNew.Add(InventoryArchetypeKeys[i],Asset->InventoryArchetypes[InventoryArchetypeKeys[i]]);
		Asset->InventoryArchetypes = InventoryArchetypesNew;
	}
	
	//Sort Item Archetypes Alphabetically
	{
		TArray<FName> ItemArchetypesKeys;
		Asset->ItemArchetypes.GetKeys(ItemArchetypesKeys);
		ItemArchetypesKeys.Sort([](const FName& A, const FName& B) { return A.ToString() < B.ToString(); });

		auto ItemArchetypesNew = TMap<FName,FFluidInventoryItemArchetype>();
		for (int i = 0; i < ItemArchetypesKeys.Num(); i++)
			ItemArchetypesNew.Add(ItemArchetypesKeys[i],Asset->ItemArchetypes[ItemArchetypesKeys[i]]);
		Asset->ItemArchetypes = ItemArchetypesNew;
	}

	//Sort Item Archetype List Alphabetically
	{
		TArray<FName> ItemArchetypeListKeys;
		Asset->ItemArchetypeLists.GetKeys(ItemArchetypeListKeys);
		ItemArchetypeListKeys.Sort([](const FName& A, const FName& B) { return A.ToString() < B.ToString(); });

		auto ItemArchetypesNew = TMap<FName,FFluidInventoryItemArchetypeList>();
		for (int i = 0; i < ItemArchetypeListKeys.Num(); i++)
			ItemArchetypesNew.Add(ItemArchetypeListKeys[i],Asset->ItemArchetypeLists[ItemArchetypeListKeys[i]]);
		Asset->ItemArchetypeLists = ItemArchetypesNew;
	}

	//Save Asset
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, true, false);
}

void FluidInventoryEditorDataSet::AddReferencedObjects(FReferenceCollector& Collector)
{
}

void FluidInventoryEditorDataSet::Initialize(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,  UFluidInventoryDataSet* AssetIn)
{
	//Get Asset Name
	Asset = AssetIn;
	AssetName = Asset->GetFName();
	InitializeDetailsView();

	// Layout
	FName LayoutID = Asset->GetClass()->GetFName();
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();
	
	//Details View
	Area->Split(FTabManager::NewStack()
		->AddTab(DetailsID, ETabState::OpenedTab,ESidebarLocation::None,10.0f));
	Layout->AddArea(Area);

	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidInventory",Layout,false,true,Asset);
	
	//Set Details View Object To Asset
	DetailsView->SetObject(Asset);
}

void FluidInventoryEditorDataSet::InitializeDetailsView()
{
	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;
	Args.bAllowSearch = false;
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	DetailsView = PropertyModule.CreateDetailView(Args);
	//DetailsView->OnFinishedChangingProperties().AddRaw(this, &FluidArchitectCoreEditorProfile::OnFinishedChangingProperties);
}

TSharedRef<SDockTab> FluidInventoryEditorDataSet::SpawnTabDetails(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabColorScale(GetTabColorScale())
		[
			DetailsView.ToSharedRef()
		];
}
