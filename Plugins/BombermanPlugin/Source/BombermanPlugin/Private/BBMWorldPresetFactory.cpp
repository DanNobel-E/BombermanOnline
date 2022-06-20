// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldPresetFactory.h"

UBBMWorldPresetFactory::UBBMWorldPresetFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = false;
	SupportedClass = UBBMWorldPreset::StaticClass();
	
}


UObject* UBBMWorldPresetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	
	UBBMWorldPreset* WorldPreset = NewObject<UBBMWorldPreset>(GetTransientPackage(), InName, EObjectFlags::RF_Standalone | EObjectFlags::RF_Public);
	
	// Load necessary modules
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Generate a unique asset name
	FString BasePath = TEXT("/Game/") + InName.ToString();
	FString Name, PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(BasePath, TEXT(""), PackageName, Name);
	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
	FString LongPackageName = FPackageName::FilenameToLongPackageName(Name);
	if (FPackageName::DoesPackageExist(PackageName))
	{
		UE_LOG(LogTemp,Warning,TEXT("Can't create preset"));
		return nullptr;
	}

	// Create object and package
	UPackage* Package = CreatePackage(nullptr, *PackageName);

	FObjectDuplicationParameters Params= FObjectDuplicationParameters(WorldPreset,Package);
	UObject* ObjectAsset = StaticDuplicateObjectEx(Params);
	ObjectAsset->Modify();
	ObjectAsset->Rename(*InName.ToString());
	SavePackageHelper(Package, PackageName);
	
	// Inform asset registry
	AssetRegistry.AssetCreated(ObjectAsset);
	ObjectAsset->MarkPackageDirty();
	ObjectAsset->PostEditChange();
	ObjectAsset->AddToRoot();

	return ObjectAsset;


}