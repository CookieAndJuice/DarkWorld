// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DWPlayerDefaultStatData.generated.h"

USTRUCT(BlueprintType)
struct FDWPlayerDefaultStatData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Vitality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Vigor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Capacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Motivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Technique;
	
	FDWPlayerDefaultStatData operator+(const FDWPlayerDefaultStatData& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);
		
		FDWPlayerDefaultStatData Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FDWPlayerDefaultStatData) / sizeof(float);
		for (int32 i = 0; i < StatNum; ++i)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		
		return Result;
	}
};