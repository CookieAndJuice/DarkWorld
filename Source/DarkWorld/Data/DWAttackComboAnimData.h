// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWAttackComboAnimData.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWAttackComboAnimData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDWAttackComboAnimData();
	
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;
	
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;
	
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UAnimMontage> AttackMontage;
};
