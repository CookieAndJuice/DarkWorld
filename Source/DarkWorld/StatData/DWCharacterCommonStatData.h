// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DWCharacterCommonStatData.generated.h"

USTRUCT(BlueprintType)
struct FDWCharacterCommonStatData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Defense;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Weight;
};
