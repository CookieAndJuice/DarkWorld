// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWEnemyStatData.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWEnemyStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxHp;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Stamina;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Defense;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Damage;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Groggy;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float DetectRange;
};
