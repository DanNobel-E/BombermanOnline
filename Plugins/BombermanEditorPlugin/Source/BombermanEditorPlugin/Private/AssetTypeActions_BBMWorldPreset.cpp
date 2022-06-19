#include "AssetTypeActions_BBMWorldPreset.h"
#include "BBMWorldPreset.h"
#include "BombermanEditorPlugin.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_BBMWorldPreset"

FText FAssetTypeActions_BBMWorldPreset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions_BBMWorldPreset", "AssetTypeActions_BBMWorldPreset", "BBMWorldPreset");
}

FColor FAssetTypeActions_BBMWorldPreset::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FAssetTypeActions_BBMWorldPreset::GetSupportedClass() const
{
	return UBBMWorldPreset::StaticClass();
}

uint32 FAssetTypeActions_BBMWorldPreset::GetCategories()
{
	return EAssetTypeCategories::Basic;
	
}

void FAssetTypeActions_BBMWorldPreset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UBBMWorldPreset* MyWorldPreset = Cast<UBBMWorldPreset>(*ObjIt);
		if (MyWorldPreset != NULL)
		{
			IBBMWorldPresetEditorModule* CustomAssetEditorModule = &FModuleManager::LoadModuleChecked<IBBMWorldPresetEditorModule>("BombermanEditorPlugin");
			CustomAssetEditorModule->CreateBBMWorldPresetEditor(Mode, EditWithinLevelEditor, MyWorldPreset);
		}
	}
}

#undef LOCTEXT_NAMESPACE