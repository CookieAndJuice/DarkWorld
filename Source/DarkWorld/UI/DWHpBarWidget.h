// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DWHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDWHpBarWidget(const FObjectInitializer& ObjectInitializer);
	
	void InitHp(float NewMaxHp);
	void UpdateHpBar(float CurrentHp);
	void BindWidget(class UDWCharacterStatComponent* Stat);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(BindWidget))
	TObjectPtr<class UProgressBar> HpBar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHp;
};
