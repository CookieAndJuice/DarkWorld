// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "DWEnemyStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWEnemyStatComponent : public UDWCharacterStatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDWEnemyStatComponent();
	
	float GetDetectRange() const { return DetectRange; }
	
	void InitStat(class UDWEnemyStatData* StatData);
	
protected:	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Groggy;
	
	UPROPERTY(EditAnywhere, Category = "Stat")
	float DetectRange;
};
