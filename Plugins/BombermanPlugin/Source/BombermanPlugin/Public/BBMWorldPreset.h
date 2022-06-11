// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/PlayerStart.h"
#include "UObject/NoExportTypes.h"
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
};
