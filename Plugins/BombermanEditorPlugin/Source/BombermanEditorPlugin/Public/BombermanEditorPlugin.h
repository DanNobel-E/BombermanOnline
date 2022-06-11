// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetToolsModule.h"
#include "AssetTypeActions_BBMWorldPreset.h"
#include "BBMWorldPreset.h"
#include "CoreMinimal.h"
#include "IAssetTools.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

class IBBMWorldPresetEditor;

extern const FName BBMWorldPresetEditorAppIdentifier;

/**
 * Custom Asset editor module interface
 */
class BOMBERMANEDITORPLUGIN_API IBBMWorldPresetEditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
public:
	/**
	 * Creates a new custom asset editor.
	 */
	virtual TSharedRef<IBBMWorldPresetEditor> CreateBBMWorldPresetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UBBMWorldPreset* WorldPreset) = 0;
};


class FBombermanEditorPluginModule : public IBBMWorldPresetEditorModule
{
	
	public:
		/** Constructor */
		FBombermanEditorPluginModule() { }
		virtual void StartupModule() override;
		virtual void ShutdownModule() override;

		virtual TSharedRef<IBBMWorldPresetEditor> CreateBBMWorldPresetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UBBMWorldPreset* WorldPreset) override;

		/** Gets the extensibility managers for outside entities to extend custom asset editor's menus and toolbars */
		virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
		virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }
		
		void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

	private:
		TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
		TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
		TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
	
};
