// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "DWPlayerCombatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWPlayerCombatComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	UDWPlayerCombatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetInputBinding(class UEnhancedInputComponent* InputComponent) override;
	
protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> AttackAction;
	
	void InputAttack(const FInputActionValue& Value);
	
// Attack Logic Function
protected:
	void BeginAttack();
	void EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
public:
	virtual void ChangeCanCombo();
	virtual void CheckCombo();
	
	virtual void StartCombo(UStaticMeshComponent* Weapon);
	virtual void EndCombo(UStaticMeshComponent* Weapon);
	
protected:
	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UDWAttackComboAnimData> AttackComboAnimData;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UAnimMontage> AttackMontage;
	
private:
	uint8 MaxComboCount = 3;
	uint8 CurrentComboCount = 0;
	bool bCanCombo = false;
	bool bNextComboCommand = false;
};
