#pragma once

#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"

/*
 *
 */
class FAssetTypeActions_BBMWorldPreset : public FAssetTypeActions_Base
{
public:
	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface
};