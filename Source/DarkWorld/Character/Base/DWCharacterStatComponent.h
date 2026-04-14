// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWCharacterBaseComponent.h"
#include "DWCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWCharacterStatComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	UDWCharacterStatComponent();

public:
	float GetCurrentHp() const { return CurrentHp; }
	float GetMaxHp() const { return MaxHp; }
	float GetDamage() const { return Damage; }
	void ApplyDamage(float Damage);
	bool IsDead() const { return bIsDead; }
	
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	
protected:
	void SetHp(float NewHp);
	
protected:
	// Status	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Status")
	float CurrentHp;
	UPROPERTY(VisibleInstanceOnly, Category = "Status")
	float MaxHp;
	UPROPERTY(VisibleInstanceOnly, Category = "Status")
	float Stamina;
	UPROPERTY(VisibleInstanceOnly, Category = "Status")
	float Defense;
	UPROPERTY(VisibleInstanceOnly, Category = "Status")
	float Damage;
	
	// Character State
	bool bIsDead = false;
};
