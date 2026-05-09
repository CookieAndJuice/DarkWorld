// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterBase.h"
#include "Character/Base/DWCharacterCombatInterface.h"
#include "DWPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API ADWPlayerCharacter : public ADWCharacterBase, public IDWCharacterCombatInterface
{
	GENERATED_BODY()
	
public:
	ADWPlayerCharacter();
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	class UDWPlayerStatComponent* GetPlayerStatComponent() const { return PlayerStatComponent; }
	class UCameraComponent* GetCameraComponent() const { return Camera; }
	bool GetIsLockOn() const;
	UStaticMeshComponent* GetRightHand() { return RightHand; }
	
	// For Anim Notify
	virtual void ChangeCanCombo() const override;
	virtual void CheckNextCombo() const override;
	
	virtual void StartCombo(FName WeaponName) const override;
	virtual void EndCombo(FName WeaponName) const override;
	
	// Take Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	// Default Mouse Control
	void UseDefaultMouseControl();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UDWPlayerControlData> PlayerControlData;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDWPlayerMoveComponent> PlayerMoveComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDWPlayerLookComponent> PlayerLookComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDWPlayerStatComponent> PlayerStatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDWPlayerCombatComponent> PlayerCombatComponent;
	
protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputMappingContext> IMC_Player;
	
protected:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UCameraComponent> Camera;
	
protected:
	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> LeftHand;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> RightHand;
};
