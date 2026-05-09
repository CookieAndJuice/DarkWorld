// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interface/DWEnemyAIInterface.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "DWEnemyCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWEnemyCombatComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	UDWEnemyCombatComponent();

protected:
	virtual void BeginPlay() override;
	
	void AddWeaponInMap(UStaticMeshComponent* Weapon);

public:
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished);
	
	void BeginAttack();
	void EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	void ChangeCanCombo();
	void CheckNextCombo();
	
	void StartCombo(FName WeaponName);
	void EndCombo(FName WeaponName);
	
protected:
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UDWAttackComboAnimData> AttackComboAnimData;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UAnimMontage> AttackMontage;
	
	UPROPERTY(EditInstanceOnly, Category = Combat)
	TMap<FName, TObjectPtr<UPrimitiveComponent>> WeaponMap;
	
private:
	int32 MaxComboCount = 3;
	int32 CurrentComboCount = 0;
	bool bCanCombo = false;
	bool bNextComboCommand = false;
	
	FAICharacterAttackFinished OnAttackFinished;
};
