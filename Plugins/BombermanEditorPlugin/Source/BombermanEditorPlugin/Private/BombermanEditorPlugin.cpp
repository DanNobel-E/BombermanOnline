// Copyright Epic Games, Inc. All Rights Reserved.

#include "BombermanEditorPlugin.h"
#include "BBMWorldPreset.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/IToolkit.h"
#include "BBMWorldPresetEditor.h"


#define LOCTEXT_NAMESPACE "FBombermanEditorPluginModule"

const FName BBMWorldPresetEditorAppIdentifier = FName(TEXT("BBMWorldPresetEditorApp"));

/*
*Called right after the module DLL has been loadedand the module object has been created
*/
void FBombermanEditorPluginModule::StartupModule()
{
    // Create new extensibility managers for our menu and toolbar
    MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
    ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_BBMWorldPreset()));
}

/**
* Called before the module is unloaded, right before the module object is destroyed.
*/
void FBombermanEditorPluginModule::ShutdownModule()
{
    // Reset our existing extensibility managers
    MenuExtensibilityManager.Reset();
    ToolBarExtensibilityManager.Reset();

    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        // Unregister our custom created assets from the AssetTools
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
        }
    }

    CreatedAssetTypeActions.Empty();
}

/**
* Creates a new custom asset editor for a MyCustomAsset
*/
TSharedRef<IBBMWorldPresetEditor> FBombermanEditorPluginModule::CreateBBMWorldPresetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UBBMWorldPreset* WorldPreset)
{
    // Initialize and spawn a new custom asset editor with the provided parameters
    TSharedRef<FBBMWorldPresetEditor> NewCustomAssetEditor(new FBBMWorldPresetEditor());
    NewCustomAssetEditor->InitBBMWorldPresetEditor(Mode, InitToolkitHost, WorldPreset);
    return NewCustomAssetEditor;
}

void FBombermanEditorPluginModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
    AssetTools.RegisterAssetTypeActions(Action);
    CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBombermanEditorPluginModule, BombermanEditorPlugin)