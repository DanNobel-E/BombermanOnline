// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldFactory.h"
#include "BBMWorldPreset.h"


UBBMWorldFactory::UBBMWorldFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	SupportedClass = UWorld::StaticClass();
	ColorTolerance = 10.f;
	WorldPreset = nullptr;
}

void UBBMWorldFactory::SetWorldPreset(UBBMWorldPreset* InWorldPreset)
{
	if (InWorldPreset)
	{
		WorldPreset = InWorldPreset;

	}
}

UObject* UBBMWorldFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	const bool bAddToRoot = false;
	UWorld* NewWorld = UWorld::CreateWorld(EWorldType::Inactive, false, InName, Cast<UPackage>(InParent), bAddToRoot, ERHIFeatureLevel::Num);
	NewWorld->Modify();
	GEditor->InitBuilderBrush(NewWorld);
	NewWorld->SetFlags(Flags);
	NewWorld->ThumbnailInfo = NewObject<UWorldThumbnailInfo>(NewWorld, NAME_None, RF_Transactional);

	if (!WorldPreset)
	{
		return nullptr;
	}

	int32 WorldUnit = 100;

	if (Filename != "") 
	{
		//Get Texture Data
		int32 WorldWidth;
		int32 WorldHeight;
		TArray<FColor> Pixels = GetTexturePixelsByFilename(Filename, WorldWidth, WorldHeight);

		WorldPreset->SpawnWorldEnviroment(NewWorld);
		WorldPreset->SpawnFloor(NewWorld, WorldWidth, WorldHeight);
		WorldPreset->CreateWorldFromTextureData(NewWorld, Pixels, WorldWidth, WorldHeight, WorldUnit);

		return NewWorld;

	}
	
	WorldPreset->SpawnWorldActors(NewWorld, WorldUnit);
	
	return NewWorld;
}


TArray<FColor> UBBMWorldFactory::GetTexturePixelsByFilename(const FString& Filename, int32& InWidth, int32& InHeight)
{

	TArray<FColor> ResultPixels;
	UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(Filename);

	if (!Texture)
	{
		return WorldPreset->GetTexturePixels(InWidth, InHeight);
	}

	InWidth = Texture->GetSizeX();
	InHeight = Texture->GetSizeY();

	int32 Size = InWidth * InHeight;
	ResultPixels.SetNumZeroed(Size);


	FByteBulkData Data = Texture->PlatformData->Mips[0].BulkData;
	FColor* Pixels = static_cast<FColor*>(Data.Lock(LOCK_READ_ONLY));

	for (int32 Index = 0; Index < ResultPixels.Num(); Index++)
	{
		FColor* Color = Pixels + Index;
		ResultPixels[Index] = *Color;
	}

	Data.Unlock();
	
	return ResultPixels;

}




