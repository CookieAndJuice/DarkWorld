// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DWEnemyStatComponent.h"
#include "AI/Interface/DWEnemyAIInterface.h"
#include "Character/Base/DWCharacterBase.h"
#include "DWKhaimeraEnemy.generated.h"

UCLASS()
class DARKWORLD_API ADWKhaimeraEnemy : public ADWCharacterBase, public IDWEnemyAIInterface
{
	GENERATED_BODY()

public:
	ADWKhaimeraEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual float GetDetectRange() const override { return EnemyStatComponent->GetDetectRange(); }
	virtual float GetAttackRange() const override { return 100.f; }
	virtual class UBehaviorTree* GetBehaviorTree() const override { return BehaviorTree; }
	virtual class UBlackboardData* GetBlackboardData() const override { return BlackboardData; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDWEnemyStatComponent> EnemyStatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UWidgetComponent> HpBar;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", Meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
	
	void SetDead();
	void PlayDeadMontage();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UDWEnemyStatData> EnemyStatData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UBlackboardData> BlackboardData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UBehaviorTree> BehaviorTree;
};
