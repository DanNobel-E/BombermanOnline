// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldPresetFactory.h"

UBBMWorldPresetFactory::UBBMWorldPresetFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditorImport = true;
	bEditAfterNew = true;
	SupportedClass = UBBMWorldPreset::StaticClass();
	
}


UObject* UBBMWorldPresetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{

	UBBMWorldPreset* WorldPreset = NewObject<UBBMWorldPreset>(UBBMWorldPreset::StaticClass(), InName, Flags);

	FAssetRegistryModule::AssetCreated(WorldPreset);

	return WorldPreset;


}