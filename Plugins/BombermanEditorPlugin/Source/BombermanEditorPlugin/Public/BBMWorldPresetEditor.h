#pragma once


#include "BBMViewportHandler.h"
#include "CoreMinimal.h"
#include "Editor/PropertyEditor/Public/PropertyEditorDelegates.h"
#include "IBBMWorldPresetEditor.h"
#include "PackageHelperFunctions.h"
#include "Slate/SceneViewport.h"
#include "Tickable.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"
#include "Widgets/SViewport.h"

class IDetailsView;
class SDockableTab;
class UBBMWorldPreset;


class FBBMWorldPresetEditor : public IBBMWorldPresetEditor
{
public:

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	/**
	 * Edits the specified asset object
	 *
	 * @param	Mode					Asset editing mode for this editor (standalone or world-centric)
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	InCustomAsset			The Custom Asset to Edit
	 */
	void InitBBMWorldPresetEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UBBMWorldPreset* InWorldPreset);

	/** Destructor */
	virtual ~FBBMWorldPresetEditor();

	/** Begin IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	//virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual bool IsPrimaryEditor() const override { return true; }
	/** End IToolkit interface */

	/** Begin ICustomAssetEditor initerface */
	virtual UBBMWorldPreset* GetWorldPreset() override;
	virtual void SetWorldPreset(UBBMWorldPreset* InWorldPreset) override;
	/** End ICustomAssetEditor initerface */

	virtual void OnGenerateWorldClicked();

	virtual void SaveAsset_Execute() override;

	UWorld* ViewportWorld;

private:
	/** Create the properties tab and its content */
	TSharedRef<SDockTab> SpawnPropertiesTab(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnViewportTab(const FSpawnTabArgs& Args);


	/** Dockable tab for properties */
	TSharedPtr<SDockableTab> PropertiesTab;
	TSharedPtr<SDockableTab> ViewportTab;
	TSharedPtr<SDockTab> ViewportTabRef;

	/** Details view */
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<SBBMEditorViewport> Viewport;
	TSharedPtr<FBBMViewportClient> ViewportClient;


	/**	The tab ids for all the tabs used */
	static const FName PropertiesTabId;
	static const FName ViewportTabId;



	/** The Custom Asset open within this editor */
	UBBMWorldPreset* WorldPreset;
};

