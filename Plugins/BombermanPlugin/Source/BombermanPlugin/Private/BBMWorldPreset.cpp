// Fill out your copyright notice in the Description page of Project Settings.


#include "BBMWorldPreset.h"


UBBMWorldPreset::UBBMWorldPreset()
{
	ColorTolerance = 10.f;

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


UWorld* UBBMWorldPreset::SpawnWorldActors(UWorld* InWorld, const int32 WorldUnit)
{
	FTransform DefaultTransform;

	//Spawn Light Stuffs
	ADirectionalLight* Light = InWorld->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), MainLightTransform);
	Light->SetBrightness(3);
	ASphereReflectionCapture* Reflection = InWorld->SpawnActor<ASphereReflectionCapture>(ASphereReflectionCapture::StaticClass(), DefaultTransform);


	//Spawn Sky Sphere
	InWorld->SpawnActor<AActor>(SkySphere, DefaultTransform);

	//Get Texture Data
	int32 WorldWidth;
	int32 WorldHeight;

	TArray<FColor> Pixels = GetTexturePixels(WorldWidth, WorldHeight);

	//Spawn Floor
	FTransform Transform;
	Transform.SetLocation(FVector(0, 0, 0));
	Transform.SetScale3D(FVector(WorldWidth, WorldHeight, 1));
	InWorld->SpawnActor<AStaticMeshActor>(PresetActors["Floor"], Transform);

	//Create World
	CreateWorldFromTextureData(InWorld, Pixels, WorldWidth, WorldHeight, WorldUnit);

	return InWorld;
}


TArray<FColor> UBBMWorldPreset::GetTexturePixels(int32& InWidth, int32& InHeight)
{

	TArray<FColor> ResultPixels;

	
	if (!Texture)
	{
		
		return ResultPixels;

	}
	

	InWidth = Texture->GetSizeX();
	InHeight = Texture->GetSizeY();

	int32 Size = InWidth * InHeight;
	ResultPixels.SetNumZeroed(Size);


	
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
	


	return ResultPixels;

}


void UBBMWorldPreset::CreateWorldFromTextureData(UWorld* InWorld, const TArray<FColor> Pixels, const int32 InWidth, const int32 InHeight, const int32 WorldUnit)
{

	int32 XOffset = -InWidth * 0.5f * WorldUnit;
	int32 YOffset = -InHeight * 0.5f * WorldUnit;


	for (int32 Index = 0; Index < Pixels.Num(); Index++)
	{
		FColor Color = Pixels[Index];
		if (Color.A != 0)
		{
			FTransform Transform;
			int32 HalfWidth = Transform.GetScale3D().X * 0.5f * WorldUnit;
			int32 HalfHeight = Transform.GetScale3D().Y * 0.5f * WorldUnit;
			Transform.SetLocation(FVector(XOffset + HalfWidth, YOffset + HalfHeight, WorldUnit));

			for (TPair<FString, FColor> Preset : PresetColorPattern)
			{
				if (SpawnPreset(InWorld, Preset.Key, Color, Transform))
				{
					break;
				}
			}


		}

		XOffset += WorldUnit;

		if (XOffset >= InWidth * 0.5f * WorldUnit)
		{
			XOffset = -InWidth * 0.5f * WorldUnit;
			YOffset += WorldUnit;
		}


	}

}

bool UBBMWorldPreset::IsColorInToleranceRange(const FColor& InColorA, const FColor& InColorB, const float Tolerance)
{
	bool result = false;



	uint8 RA = InColorA.R;
	uint8 GA = InColorA.G;
	uint8 BA = InColorA.B;
	uint8 AA = InColorA.A;



	uint8 RB = InColorB.R;
	uint8 GB = InColorB.G;
	uint8 BB = InColorB.B;
	uint8 AB = InColorB.A;

	if (RA< RB - Tolerance || RA> RB + Tolerance)
	{
		return result;
	}

	if (GA< GB - Tolerance || GA> GB + Tolerance)
	{
		return result;
	}

	if (BA< BB - Tolerance || BA> BB + Tolerance)
	{
		return result;
	}

	if (AA< AB - Tolerance || AA> AB + Tolerance)
	{
		return result;
	}

	result = true;
	return result;

}

bool UBBMWorldPreset::SpawnPreset(UWorld* InWorld, const FString& Key, const FColor& InColor, const FTransform& InTransform)
{
	bool result = false;

	if (IsColorInToleranceRange(InColor, PresetColorPattern[Key], ColorTolerance))
	{

		InWorld->SpawnActor<AActor>(PresetActors[Key], InTransform);
		result = true;
	}

	return result;
}
