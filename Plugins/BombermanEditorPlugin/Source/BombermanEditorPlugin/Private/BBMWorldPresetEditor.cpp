#include "BBMWorldPresetEditor.h"
#include "Modules/ModuleManager.h"
#include "EditorStyleSet.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Editor.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "BombermanEditorPlugin.h"
#include "BBMWorldFactory.h"
#include "AssetRegistryModule.h"
#include "SEditorViewport.h"



#define LOCTEXT_NAMESPACE "BBMWorldPresetEditor"

//const FName FBBMWorldPresetEditor::ToolkitFName(TEXT("BBMWorldPresetEditor"));
const FName FBBMWorldPresetEditor::PropertiesTabId(TEXT("BBMWorldPresetEditor_Properties"));
const FName FBBMWorldPresetEditor::ViewportTabId(TEXT("BBMWorldPresetEditor_MainViewport"));



void FBBMWorldPresetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	// Add a new workspace menu category to the tab manager
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_BBMWorldPresetEditor", "BBM World Preset Editor"));

	// We register the tab manager to the asset editor toolkit so we can use it in this editor
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// Register the properties tab spawner within our tab manager
	// We provide the function with the identiefier for this tab and a shared pointer to the
	// SpawnPropertiesTab function within this editor class
	// Additionaly, we provide a name to be displayed, a category and the tab icon
	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FBBMWorldPresetEditor::SpawnPropertiesTab))
		.SetDisplayName(LOCTEXT("PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(ViewportTabId, FOnSpawnTab::CreateSP(this, &FBBMWorldPresetEditor::SpawnViewportTab))
		.SetDisplayName(LOCTEXT("ViewportTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef())
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

}

void FBBMWorldPresetEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	// Unregister the tab manager from the asset editor toolkit
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	// Unregister our custom tab from the tab manager, making sure it is cleaned up when the editor gets destroyed
	InTabManager->UnregisterTabSpawner(PropertiesTabId);
}

TSharedRef<SDockTab> FBBMWorldPresetEditor::SpawnPropertiesTab(const FSpawnTabArgs& Args)
{
	// Make sure we have the correct tab id
	check(Args.GetTabId() == PropertiesTabId);

	// Return a new slate dockable tab that contains our details view
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Properties"))
		.Label(LOCTEXT("GenericDetailsTitle", "Details"))
		.TabColorScale(GetTabColorScale())
		[
			// Provide the details view as this tab its content
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		[
			DetailsView.ToSharedRef()
		]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.VAlign(EVerticalAlignment::VAlign_Bottom)
			[
				SNew(SButton)
				.Text(FText::FromString("Generate World"))
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				.ContentPadding(FMargin(10,10))
				.OnPressed_Raw(this,&FBBMWorldPresetEditor::OnGenerateWorldClicked)
		]
		];
}


TSharedRef<SDockTab> FBBMWorldPresetEditor::SpawnViewportTab(const FSpawnTabArgs& Args)
{
	// Make sure we have the correct tab id
	check(Args.GetTabId() == ViewportTabId);

	if (!Viewport.IsValid())
	{
		Viewport = SNew(SBBMEditorViewport);
		ViewportClient = Viewport->ViewportClient;
	}
	

	// Return a new slate dockable tab that contains our details view
	ViewportTabRef = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("GenericEditor.Tabs.Details"))
		.Label(LOCTEXT("GenericViewportTitle", "Viewport"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			[

				Viewport.ToSharedRef()
				
			
			]
				

		];

	
		
	return ViewportTabRef.ToSharedRef();
}


FBBMWorldPresetEditor::~FBBMWorldPresetEditor()
{
	// On destruction we reset our tab and details view 
	DetailsView.Reset();
	PropertiesTab.Reset();
}

FName FBBMWorldPresetEditor::GetToolkitFName() const
{
	return FName(GetBaseToolkitName().ToString());
}

FText FBBMWorldPresetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "BBM World Preset Editor");
}

FText FBBMWorldPresetEditor::GetToolkitToolTipText() const
{
	return LOCTEXT("ToolTip", "BBM World Preset Editor");
}

FString FBBMWorldPresetEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "AnimationDatabase ").ToString();
}

FLinearColor FBBMWorldPresetEditor::GetWorldCentricTabColorScale() const
{
	return FColor::Red;
}

UBBMWorldPreset* FBBMWorldPresetEditor::GetWorldPreset()
{
	return WorldPreset;
}

void FBBMWorldPresetEditor::SetWorldPreset(UBBMWorldPreset* InWorldPreset)
{
	WorldPreset = InWorldPreset;
}

void FBBMWorldPresetEditor::SaveAsset_Execute()
{
	if (WorldPreset)
	{
		
		UPackage* Package = WorldPreset->GetPackage();
		SavePackageHelper(Package, Package->GetName());

	}

}


void FBBMWorldPresetEditor::InitBBMWorldPresetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UBBMWorldPreset* InWorldPreset)
{
	// Cache some values that will be used for our details view arguments
	const bool bIsUpdatable = true;
	const bool bAllowFavorites = true;
	const bool bIsLockable = false;

	// Set this InCustomAsset as our editing asset
	SetWorldPreset(InWorldPreset);
	WorldPreset->Modify();

	// Retrieve the property editor module and assign properties to DetailsView
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(bIsUpdatable, bIsLockable, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->ForceRefresh();
	
	
	
	// Create the layout of our custom asset editor
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("BBMEditor_v6")
		->AddArea
		(
			// Create a vertical area and spawn the toolbar
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(EOrientation::Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(PropertiesTabId, ETabState::OpenedTab)
					
				)
				->Split
				(   
					FTabManager::NewStack()
					->AddTab(ViewportTabId, ETabState::OpenedTab)
				)
			)
			

		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;

	// Initialize our custom asset editor
	FAssetEditorToolkit::InitAssetEditor(
		Mode,
		InitToolkitHost,
		BBMWorldPresetEditorAppIdentifier,
		StandaloneDefaultLayout,
		bCreateDefaultStandaloneMenu,
		bCreateDefaultToolbar,
		(UObject*)InWorldPreset);

	// Set the asset we are editing in the details view
	if (DetailsView.IsValid())
	{
		DetailsView->SetObject((UObject*)WorldPreset);
	}
}

void FBBMWorldPresetEditor::OnGenerateWorldClicked()
{
	if (WorldPreset)
	{
		UBBMWorldFactory* NewBBMWorldFactory = NewObject<UBBMWorldFactory>();
		NewBBMWorldFactory->SetWorldPreset(WorldPreset);

		uint64 SuffixAssetName = FPlatformTime::Cycles64();
		FString AssetName = FString::Printf(TEXT("W_Level_%llu"), SuffixAssetName);
		UPackage* Package = CreatePackage(*FString::Printf(TEXT("/Game/%s"), *AssetName));
		bool OutOperationCanceled;

		UObject* NewWorldObject = NewBBMWorldFactory->FactoryCreateFile(NewBBMWorldFactory->SupportedClass, Package, *AssetName, EObjectFlags::RF_Standalone | EObjectFlags::RF_Public, "", nullptr, GWarn, OutOperationCanceled);
		FAssetRegistryModule::AssetCreated(NewWorldObject);

		UWorld* NewWorld = Cast<UWorld>(NewWorldObject);

		NewWorld->PostEditChange();
		NewWorld->MarkPackageDirty();
		
		
		if (NewWorld)
		{
			Viewport->SetWorld(NewWorld);

		}

	}
}

#undef LOCTEXT_NAMESPACE