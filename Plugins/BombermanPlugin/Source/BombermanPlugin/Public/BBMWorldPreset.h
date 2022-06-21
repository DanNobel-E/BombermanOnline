// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/NoExportTypes.h"

#include "Atmosphere/AtmosphericFog.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SphereReflectionCapture.h"
#include "Factories/Factory.h"
#include "Factories/WorldFactory.h"
#include "ImageUtils.h"
#include "ThumbnailRendering/WorldThumbnailInfo.h"


#include "BBMWorldPreset.generated.h"



/**
 * 
 */
UCLASS(Blueprintable)
class BOMBERMANPLUGIN_API UBBMWorldPreset : public UObject
{
	GENERATED_BODY()
public:

	UBBMWorldPreset();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float ColorTolerance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TMap<FString, FColor> PresetColorPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TMap<FString, TSubclassOf<AActor>> PresetActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters")
		TSubclassOf<APawn> Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World")
		UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
		TSubclassOf<AActor> SkySphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light")
	FTransform MainLightTransform;


	UFUNCTION(BlueprintCallable, Category = "Utils")
	UWorld* SpawnWorldActors(UWorld* InWorld, const int32 WorldUnit = 100);

	TArray<FColor> GetTexturePixels(int32& InWidth, int32& InHeight);
	UWorld* CreateWorldFromTextureData(UWorld* InWorld, const TArray<FColor> Pixels, const int32 InWidth, const int32 InHeight, const int32 WorldUnit);
	bool IsColorInToleranceRange(const FColor& InColorA, const FColor& InColorB, const float Tolerance);
	void SpawnWorldEnviroment(UWorld* InWorld);
	void SpawnFloor(UWorld* InWorld, const int32 InWidth, const int32 InHeight);
	bool SpawnPreset(UWorld* InWorld, const FString& PresetKey, const FColor& InColor, const FTransform& InTransform);
};
