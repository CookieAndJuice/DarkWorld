// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "DWPlayerLockOnComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWPlayerLockOnComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	UDWPlayerLockOnComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetInputBinding(class UEnhancedInputComponent* InputComponent) override;
	bool GetIsLockOn() const { return bIsLockOn; }
	
protected:
	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> LockOnAction;
	
	void InputLockOn();
	void FindNearTargets(float ViewRange, TArray<FOverlapResult>& OutEnemyResults);
	bool FindLockOnTarget(const TArray<FOverlapResult>& InEnemyResults);
	void UpdateLockOnRotation(float DeltaSeconds);
	
private:
	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;
	
	bool bIsLockOn = false;
};
