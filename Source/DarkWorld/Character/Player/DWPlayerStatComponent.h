// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "StatData/DWPlayerDefaultStatData.h"
#include "DWPlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWPlayerStatComponent : public UDWCharacterStatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDWPlayerStatComponent();
	
protected:
	virtual void InitializeComponent() override;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Status")
	float Weight;
	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
// 	FDWCharacterStatData ModifierStat;
};
