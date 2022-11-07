#include "FluidAstroStarMapModelHyqV3Editor.h"

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Logging | Constant Tab Editor IDS
=============================================================================*/

const FName FluidAstroDetailsTabID(TEXT("FluidAstroDetailsTabID"));

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Constructors
=============================================================================*/

FluidAstroStarMapModelHyqV3Editor::FluidAstroStarMapModelHyqV3Editor()
{
}

FluidAstroStarMapModelHyqV3Editor::~FluidAstroStarMapModelHyqV3Editor()
{
	//UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : FAssetEditorToolkit | FSerializableObject interface
=============================================================================*/

FName FluidAstroStarMapModelHyqV3Editor::GetToolkitFName() const
{
	return FName(AssetName.ToString());
}

FText FluidAstroStarMapModelHyqV3Editor::GetBaseToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidAstroStarMapModelHyqV3Editor::GetToolkitName() const
{
	return FText::FromString(AssetName.ToString());
}

FText FluidAstroStarMapModelHyqV3Editor::GetToolkitToolTipText() const
{
	return FText::FromString(AssetName.ToString());
}

FLinearColor FluidAstroStarMapModelHyqV3Editor::GetWorldCentricTabColorScale() const
{
	return FLinearColor();
}

FString FluidAstroStarMapModelHyqV3Editor::GetWorldCentricTabPrefix() const
{
	return AssetName.ToString();
}

FString FluidAstroStarMapModelHyqV3Editor::GetDocumentationLink() const
{
	return AssetName.ToString();
}

void FluidAstroStarMapModelHyqV3Editor::SaveAsset_Execute()
{
	//Save Asset
	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Asset->GetPackage());
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
}

void FluidAstroStarMapModelHyqV3Editor::AddReferencedObjects(FReferenceCollector& Collector)
{
}

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Register Tab Spawners
=============================================================================*/

void FluidAstroStarMapModelHyqV3Editor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	
	InTabManager->RegisterTabSpawner( FluidAstroDetailsTabID, FOnSpawnTab::CreateSP(this, &FluidAstroStarMapModelHyqV3Editor::SpawnTabEditorSettingsDetails))
		.SetDisplayName(FText::FromString("Details"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GenericEditor.Tabs.Properties"));
}

void FluidAstroStarMapModelHyqV3Editor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner( FluidAstroDetailsTabID);
}

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Initialize
=============================================================================*/

void FluidAstroStarMapModelHyqV3Editor::Initialize(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UFluidAstroStarMapModelHyqV3* AssetIn)
{
	Asset = AssetIn;
	AssetName = Asset->GetFName();

	//Toolbar
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FluidAstroStarMapModelHyqV3Editor::InitializeToolbar));
		AddToolbarExtender(ToolbarExtender);
	}

	//Editor Settings Detail View
	{
		FDetailsViewArgs Args;
		Args.bHideSelectionTip = true;
		Args.NotifyHook = this;
		Args.bAllowSearch = false;
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		EditorSettingsDetailView = PropertyModule.CreateDetailView(Args);;
		EditorSettingsDetailView->SetObject(Asset);
	}
	
	// Layout | Area
	FName LayoutID = Asset->GetClass()->GetFName();
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout(LayoutID);
	const TSharedRef <FTabManager::FArea> Area = FTabManager::NewPrimaryArea();

	//Add Tabs
	Area->Split(FTabManager::NewStack()
		->AddTab(FluidAstroDetailsTabID, ETabState::OpenedTab, ESidebarLocation::None,0));
	
	//Add Area To Layout
	Layout->AddArea(Area);
	
	//Initialize Asset Editor
	FAssetEditorToolkit::InitAssetEditor(Mode,InitToolkitHost,"FluidNarrator",Layout,false,true,Asset);
}

void FluidAstroStarMapModelHyqV3Editor::InitializeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	
	//Generate
	ToolbarBuilder.BeginSection("ExtendToolbarItemGenerate");
	{
		//Simulate Button
		ToolbarBuilder.AddToolBarButton(
			FUIAction(
				FExecuteAction::CreateSP(this, &FluidAstroStarMapModelHyqV3Editor::HandelToolbarLoadButtonPressed),
				FIsActionChecked()
			),
			NAME_None,
			FText::FromString("Load"),
			FText::FromString("Load"),
			FSlateIcon(TEXT("EditorStyle"), "GenericPlay")
		);
	}
	ToolbarBuilder.EndSection();
}

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Spawn Tabs
=============================================================================*/

TSharedRef<SDockTab> FluidAstroStarMapModelHyqV3Editor::SpawnTabEditorSettingsDetails(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(FText::FromString("Details"))
	.TabColorScale(GetTabColorScale())
	[
		EditorSettingsDetailView.ToSharedRef()
	];
}

/*=============================================================================
Fluid Astro Star Map ModelHyqV3 Editor : Handles
=============================================================================*/

void FluidAstroStarMapModelHyqV3Editor::HandelToolbarLoadButtonPressed()
{
	Asset->LoadData();
}