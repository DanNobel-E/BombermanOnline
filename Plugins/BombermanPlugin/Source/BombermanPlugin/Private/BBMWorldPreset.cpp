// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldPreset.h"


UBBMWorldPreset::UBBMWorldPreset()
{
	PresetColorPattern = TMap<FString, FColor>();
	PresetColorPattern.Add("Floor", FColor(0,0,0,0));
	PresetColorPattern.Add("Wall", FColor::Black);
	PresetColorPattern.Add("BreakableWall", FColor::Red);
	PresetColorPattern.Add("PlayerStart", FColor::White);

	PresetActors = TMap<FString, TSubclassOf<AActor>>();
	PresetActors.Add("Floor", AStaticMeshActor::StaticClass());
	PresetActors.Add("Wall", AStaticMeshActor::StaticClass());
	PresetActors.Add("BreakableWall", AStaticMeshActor::StaticClass());
	PresetActors.Add("PlayerStart", APlayerStart::StaticClass());

	

}