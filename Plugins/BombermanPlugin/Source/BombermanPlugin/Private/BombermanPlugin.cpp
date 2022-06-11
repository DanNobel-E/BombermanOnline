// Copyright Epic Games, Inc. All Rights Reserved.

#include "BombermanPlugin.h"
#include "BBMWorldPreset.h"


#define LOCTEXT_NAMESPACE "FBombermanPluginModule"


void FBombermanPluginModule::StartupModule()
{
	

	FContentBrowserModule& ContentBrowser = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowser.GetAllAssetViewContextMenuExtenders().Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FBombermanPluginModule::ExtendContentBrowser));

	BBMUIName = FName("BBM World Preview");
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(BBMUIName, FOnSpawnTab::CreateRaw(this, &FBombermanPluginModule::CreateBBMWorldTab))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());
}

void FBombermanPluginModule::ShutdownModule()
{
	// Reset our existing extensibility managers

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(BBMUIName);

}

bool FBombermanPluginModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("bomberman")))
	{
		if (FParse::Command(&Cmd, TEXT("createworld")))
		{
			FString PresetPath = "Blueprint'/BombermanPlugin/Blueprints/Game/BP_BaseWorldPreset.BP_BaseWorldPreset'";
			FString TexturePath = FString(Cmd);

			CreateBBMWorld(PresetPath, TexturePath);

			return true;
		}

	}

	return false;
}

TSharedRef<SDockTab> FBombermanPluginModule::CreateBBMWorldTab(const FSpawnTabArgs& TabArgs)
{
	const TSharedRef<SDockTab> MainTab = SNew(SDockTab)
		.TabRole(ETabRole::MajorTab);

	if (!ToolsTabManager.IsValid())
	{

		ToolsTabManager = FGlobalTabmanager::Get()->NewTabManager(MainTab);
		ToolsTabManager->SetOnPersistLayout(
			FTabManager::FOnPersistLayout::CreateStatic(
				[](const TSharedRef<FTabManager::FLayout>& InLayout)
				{
					if (InLayout->GetPrimaryArea().Pin().IsValid())
					{
						FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, InLayout);
					}
				}
			)
		);

	}
	else
	{


		ensure(ToolLayout.IsValid());


	}

	
	TWeakPtr<FTabManager> ToolsTabManagerWeak = ToolsTabManager;
	// On tab close will save the layout if the debugging window itself is closed,
	// this handler also cleans up any floating debugging controls. If we don't close
	// all areas we need to add some logic to the tab manager to reuse existing tabs:
	MainTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateStatic(
		[](TSharedRef<SDockTab> Self, TWeakPtr<FTabManager> TabManager)
		{
			TSharedPtr<FTabManager> OwningTabManager = TabManager.Pin();
			if (OwningTabManager.IsValid())
			{
				FLayoutSaveRestore::SaveToConfig(GEditorLayoutIni, OwningTabManager->PersistLayout());
				OwningTabManager->CloseAllAreas();
			}
		}
		, ToolsTabManagerWeak
			));

	const FName FirstTabName = FName(TEXT("First"));
	const FName SecondTabName = FName(TEXT("Second"));
	const FName ThirdTabName = FName(TEXT("Third"));



	if (!ToolLayout.IsValid())
	{
		ToolsTabManager->RegisterTabSpawner(
			FirstTabName,
			FOnSpawnTab::CreateStatic(
				[](const FSpawnTabArgs&)->TSharedRef<SDockTab>
				{
					return SNew(SDockTab)
						.TabRole(ETabRole::PanelTab)
						.Label(NSLOCTEXT("First Tab", "TabTitle", "Details"))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
						[

							SNew(SObjectPropertyEntryBox)
							.DisplayBrowse(true)
							.DisplayThumbnail(true)
							.AllowedClass(UTexture2D::StaticClass())
							.EnableContentPicker(true)
							
							
								

							]
							+ SVerticalBox::Slot()
							[

								SNew(SObjectPropertyEntryBox)
								.DisplayBrowse(true)
								.DisplayThumbnail(true)
								.AllowedClass(UStaticMesh::StaticClass())
								.EnableContentPicker(true)
								

							]
							+ SVerticalBox::Slot()
							[

								SNew(SObjectPropertyEntryBox)
								.DisplayBrowse(true)
								.DisplayThumbnail(true)
								.AllowedClass(UStaticMesh::StaticClass())
								.EnableContentPicker(true)

							]




						];
				}
			)
		)
			.SetDisplayName(NSLOCTEXT("Bomberman World Preview", "TabTitle", "First Tab"))
					.SetTooltipText(NSLOCTEXT("BlueprintDebugger", "ExecutionFlowTooltipText", "Open the Blueprint Execution Flow tab."));


		ToolsTabManager->RegisterTabSpawner(
			SecondTabName,
			FOnSpawnTab::CreateStatic(
				[](const FSpawnTabArgs&)->TSharedRef<SDockTab>
				{
					return SNew(SDockTab)
						.TabRole(ETabRole::PanelTab)
						.Label(NSLOCTEXT("Second Tab", "TabTitle", "ViewPort"))
						[
							SNew(STextBlock).Text(FText::FromString("I'm Second Tab"))
						];
				}
			)
		)
			.SetDisplayName(NSLOCTEXT("Bomberman World Preview", "TabTitle", "Second Tab"))
			.SetTooltipText(NSLOCTEXT("BlueprintDebugger", "ExecutionFlowTooltipText", "Open the Blueprint Execution Flow tab."));


			ToolLayout = FTabManager::NewLayout("Standalone_BombermanWorldPreview_Layout")
					->AddArea
					(
						FTabManager::NewPrimaryArea()
						->SetOrientation(Orient_Vertical)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(.2f)
							->SetHideTabWell(false)
							->AddTab(FirstTabName, ETabState::OpenedTab)
							->AddTab(SecondTabName, ETabState::OpenedTab)
							->SetForegroundTab(FirstTabName)
						)
					);



	}

	ToolLayout = FLayoutSaveRestore::LoadFromConfig(GEditorLayoutIni, ToolLayout.ToSharedRef());

	TSharedRef<SWidget> TabContents = ToolsTabManager->RestoreFrom(ToolLayout.ToSharedRef(), TSharedPtr<SWindow>()).ToSharedRef();

	// build command list for tab restoration menu:
	TSharedPtr<FUICommandList> CommandList = MakeShareable(new FUICommandList());

	TWeakPtr<FTabManager> DebuggingToolsManagerWeak = ToolsTabManager;

	const auto ToggleTabVisibility = [](TWeakPtr<FTabManager> InDebuggingToolsManagerWeak, FName InTabName)
	{
		TSharedPtr<FTabManager> InDebuggingToolsManager = InDebuggingToolsManagerWeak.Pin();
		if (InDebuggingToolsManager.IsValid())
		{
			TSharedPtr<SDockTab> ExistingTab = InDebuggingToolsManager->FindExistingLiveTab(InTabName);
			if (ExistingTab.IsValid())
			{
				ExistingTab->RequestCloseTab();
			}
			else
			{
				InDebuggingToolsManager->TryInvokeTab(InTabName);
			}
		}
	};

	const auto IsTabVisible = [](TWeakPtr<FTabManager> InDebuggingToolsManagerWeak, FName InTabName)
	{
		TSharedPtr<FTabManager> InDebuggingToolsManager = InDebuggingToolsManagerWeak.Pin();
		if (InDebuggingToolsManager.IsValid())
		{
			return InDebuggingToolsManager->FindExistingLiveTab(InTabName).IsValid();
		}
		return false;
	};


	MainTab->SetContent(
		SNew(SBorder)
		.Padding(FMargin(0.f, 3.f))
		[
			TabContents
		]
	);

	return MainTab;

	
}


TSharedRef<FExtender> FBombermanPluginModule::ExtendContentBrowser(const TArray<FAssetData>& Data)
{
	TSharedRef<FExtender> Extender = MakeShared<FExtender>();
	Extender->AddMenuExtension(FName("CommonAssetActions"), EExtensionHook::After, nullptr, FMenuExtensionDelegate::CreateRaw(this, &FBombermanPluginModule::PopulateContentBrowser, Data));
	return Extender;
}

void FBombermanPluginModule::PopulateContentBrowser(FMenuBuilder& MenuBuilder, const TArray<FAssetData> Data)
{
	for (const FAssetData& Datum : Data)
	{
		UBBMWorldPreset* Preset = ConvertBlueprintToClass<UBBMWorldPreset>(Datum.ObjectPath.ToString());

		if (!Preset)
		{
			return;
		}
	}

	MenuBuilder.BeginSection(FName("World Preset Actions"), FText::FromString("World Preset"));
	MenuBuilder.AddMenuEntry(FText::FromString("Create BBM World"), FText::FromString("Creates a new BBM World Level, using classes specified on World Preset"), FSlateIcon(), FUIAction(FExecuteAction::CreateRaw(this, &FBombermanPluginModule::CreateBBMWorldFromAssetData, Data)));
	MenuBuilder.EndSection();

}

void FBombermanPluginModule::CreateBBMWorldFromAssetData(const TArray<FAssetData> Data)
{
	
	for (const FAssetData& Datum : Data)
	{
		CreateBBMWorld(Datum.ObjectPath.ToString(), "");

	}

}

TArray<UWorld*> FBombermanPluginModule::CreateBBMWorldFromPreset(const TArray<FAssetData> Data)
{
	TArray<UWorld*> Worlds = TArray<UWorld*>();

	for (const FAssetData& Datum : Data)
	{
		Worlds.Add(CreateBBMWorld(Datum.ObjectPath.ToString(), ""));

	}

	return Worlds;
}

UWorld* FBombermanPluginModule::CreateBBMWorld(const FString& PresetPath, const FString& TexturePath)
{
	
	UBBMWorldPreset* WorldPreset = ConvertBlueprintToClass<UBBMWorldPreset>(PresetPath);

	UBBMWorldFactory* NewBBMWorldFactory = NewObject<UBBMWorldFactory>();
	NewBBMWorldFactory->SetWorldPreset(WorldPreset);

	uint64 SuffixAssetName = FPlatformTime::Cycles64();
	FString AssetName = FString::Printf(TEXT("W_Level_%llu"), SuffixAssetName);
	UPackage* Package = CreatePackage(*FString::Printf(TEXT("/Game/%s"), *AssetName));
	bool OutOperationCanceled;

	UObject* NewWorldObject = NewBBMWorldFactory->FactoryCreateFile(NewBBMWorldFactory->SupportedClass, Package, *AssetName, EObjectFlags::RF_Standalone | EObjectFlags::RF_Public, TexturePath, nullptr, GWarn, OutOperationCanceled);
	FAssetRegistryModule::AssetCreated(NewWorldObject);

	UWorld* NewWorld = Cast<UWorld>(NewWorldObject);

	NewWorld->PostEditChange();
	NewWorld->MarkPackageDirty();

	return NewWorld;
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBombermanPluginModule, BombermanPlugin)