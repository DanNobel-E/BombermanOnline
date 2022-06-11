// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AssetRegistryModule.h"
#include "BBMWorldFactory.h"
#include "CallStackViewer.h"
#include "ContentBrowserModule.h"
#include "CoreMinimal.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/CoreMisc.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "PropertyCustomizationHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkspaceMenuStructureModule.h"
#include "WorkspaceMenuStructure.h"
#include "Widgets/SViewport.h"
#include "Widgets/Docking/SDockTabStack.h"
#include "WatchPointViewer.h"




class FBombermanPluginModule : public IModuleInterface, public FSelfRegisteringExec
{
public:
	/** Constructor */
	FBombermanPluginModule() { }
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	static FName BBMWorldPresetEditorAppIdentifier;

	FName BBMUIName;
	TSharedPtr<FTabManager> ToolsTabManager;
	TSharedPtr<FTabManager::FLayout> ToolLayout;
	TSharedRef<SDockTab> CreateBBMWorldTab(const FSpawnTabArgs& TabArgs);

	TSharedRef<FExtender> ExtendContentBrowser(const TArray<FAssetData>& Data);
	void PopulateContentBrowser(FMenuBuilder& MenuBuilder, const TArray<FAssetData> Data);
	void CreateBBMWorldFromAssetData(const TArray<FAssetData> Data);

	static TArray<UWorld*> CreateBBMWorldFromPreset(const TArray<FAssetData> Data);
	static UWorld* CreateBBMWorld(const FString& PresetPath, const FString& TexturePath);

	template<typename T>
	static T* ConvertBlueprintToClass(const FString& ObjectPath)
	{
		T* ClassInstance = nullptr;
		FStringAssetReference ObjectReference = ObjectPath;

		UObject* Object = LoadObject<UObject>(nullptr, *ObjectReference.ToString());

		if (!Object)
		{
			return ClassInstance;
		}

		Object = ObjectReference.ResolveObject();

		UBlueprint* Blueprint = Cast<UBlueprint>(Object);

		if (!Blueprint)
		{
			return ClassInstance;
		}

		ClassInstance = Cast<T>(Blueprint->GeneratedClass.GetDefaultObject());
		return ClassInstance;

	}


};
