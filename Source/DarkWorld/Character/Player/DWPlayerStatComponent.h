// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "Stat/DWPlayerDefaultStatData.h"
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
	
	/** Player Default Stat Level */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status|Level")
	int32 VitalityLevel = 11;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status|Level")
	int32 VigorLevel = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status|Level")
	int32 CapacityLevel = 11;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status|Level")
	int32 MotivityLevel = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Status|Level")
	int32 TechniqueLevel = 5;
	
// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
// 	FDWCharacterStatData ModifierStat;
};
