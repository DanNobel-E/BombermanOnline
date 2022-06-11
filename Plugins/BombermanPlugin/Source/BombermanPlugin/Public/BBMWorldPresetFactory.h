// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BBMWorldPreset.h"
#include "CoreMinimal.h"
#include "AssetRegistryModule.h"
#include "Factories/Factory.h"
#include "BBMWorldPresetFactory.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMANPLUGIN_API UBBMWorldPresetFactory : public UFactory
{
	GENERATED_BODY()

public:

	UBBMWorldPresetFactory(const FObjectInitializer& ObjectInitializer);

	UObject* UBBMWorldPresetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;


};
