// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "Character/Base/DWCombatInterface.h"
#include "DWEnemyCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWEnemyCombatComponent : public UDWCharacterBaseComponent, public IDWCombatInterface
{
	GENERATED_BODY()

public:
	UDWEnemyCombatComponent();

protected:
	virtual void BeginPlay() override;

public:
	void BeginAttack();
	void EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	virtual void CombatChangeCanCombo() const override;
	virtual void CombatCheckCombo() const override;
	
	virtual void StartCombo() override;
	virtual void EndCombo() override;
	
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UDWAttackComboAnimData> AttackComboAnimData;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UAnimMontage> AttackMontage;
	
private:
	int32 MaxComboCount = 3;
	int32 ComboCount = 0;
};
