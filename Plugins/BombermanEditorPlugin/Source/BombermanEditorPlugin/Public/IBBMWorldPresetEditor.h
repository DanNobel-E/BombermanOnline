#pragma once


#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UBBMWorldPreset;

class IBBMWorldPresetEditor : public FAssetEditorToolkit
{
public:
	/** Retrieves the current custom asset. */
	virtual UBBMWorldPreset* GetWorldPreset() = 0;

	/** Set the current custom asset. */
	virtual void SetWorldPreset(UBBMWorldPreset* InWorldPreset) = 0;
};

