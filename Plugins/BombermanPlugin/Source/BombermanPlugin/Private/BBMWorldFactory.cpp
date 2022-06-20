// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldFactory.h"
#include "BBMWorldPreset.h"


UBBMWorldFactory::UBBMWorldFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	SupportedClass = UWorld::StaticClass();
	ColorTolerance = 10.f;
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
	WorldPreset->SpawnWorldActors(NewWorld, WorldUnit);


	return NewWorld;
}


TArray<FColor> UBBMWorldFactory::GetTexturePixels(const FString& Filename, int32& InWidth, int32& InHeight)
{

	TArray<FColor> ResultPixels;
	bool ValidPath = true;
	UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(Filename);

	if (!Texture)
	{
		ValidPath = false;
		Texture = Texture;
		if (!Texture)
		{
			return ResultPixels;

		}
	}


	InWidth = Texture->GetSizeX();
	InHeight = Texture->GetSizeY();

	int32 Size = InWidth * InHeight;
	ResultPixels.SetNumZeroed(Size);


	if (ValidPath)
	{
		FByteBulkData Data = Texture->PlatformData->Mips[0].BulkData;
		FColor* Pixels = static_cast<FColor*>(Data.Lock(LOCK_READ_ONLY));

		for (int32 Index = 0; Index < ResultPixels.Num(); Index++)
		{
			FColor* Color = Pixels + Index;
			ResultPixels[Index] = *Color;
		}

		Data.Unlock();
	}
	else
	{
		uint8* Data = Texture->Source.LockMip(0);

		int32 ByteOffset = 0;

		for (int32 Index = 0; Index < ResultPixels.Num(); Index++)
		{
			uint8* B = Data + ByteOffset + 0;
			uint8* G = Data + ByteOffset + 1;
			uint8* R = Data + ByteOffset + 2;
			uint8* A = Data + ByteOffset + 3;

			FColor Color = FColor(*R, *G, *B, *A);
			ResultPixels[Index] = Color;
			ByteOffset += 4;

		}

		Texture->Source.UnlockMip(0);
	}




	return ResultPixels;

}




