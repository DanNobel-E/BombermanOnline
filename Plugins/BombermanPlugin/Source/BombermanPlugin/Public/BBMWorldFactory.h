// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atmosphere/AtmosphericFog.h"
#include "CoreMinimal.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SphereReflectionCapture.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerStart.h"
#include "Factories/Factory.h"
#include "Factories/WorldFactory.h"
#include "ImageUtils.h"
#include "ThumbnailRendering/WorldThumbnailInfo.h"
#include "BBMWorldFactory.generated.h"


class UBBMWorldPreset;

UCLASS()
class BOMBERMANPLUGIN_API UBBMWorldFactory : public UFactory
{
	GENERATED_BODY()

public:

	UBBMWorldFactory(const FObjectInitializer& ObjectInitializer);
	void SetWorldPreset(UBBMWorldPreset* InWorldPreset);

	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	TArray<FColor> GetTexturePixels(const FString& Filename, int32& InWidth, int32& InHeight);
	void CreateWorldFromTextureData(UWorld* InWorld, const TArray<FColor> Pixels, const int32 InWidth, const int32 InHeight, const int32 WorldUnit);
	bool IsColorInToleranceRange(const FColor& InColorA, const FColor& InColorB, const float Tolerance);
	bool SpawnPreset(UWorld* InWorld, const FString& PresetKey, const FColor& InColor, const FTransform& InTransform);

	UBBMWorldPreset* WorldPreset;
	float ColorTolerance;
};
