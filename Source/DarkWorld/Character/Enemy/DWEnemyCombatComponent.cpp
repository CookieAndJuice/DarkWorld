// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyCombatComponent.h"

#include "Data/DWAttackComboAnimData.h"


UDWEnemyCombatComponent::UDWEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	ConstructorHelpers::FObjectFinder<UDWAttackComboAnimData> AttackComboAnimDataRef(TEXT("/Script/DarkWorld.DWAttackComboAnimData'/Game/Dark_World/Data/Animation/DA_DWKhaimeraAttackComboAnimData.DA_DWKhaimeraAttackComboAnimData'"));
	if (AttackComboAnimDataRef.Succeeded())
	{
		AttackComboAnimData = AttackComboAnimDataRef.Object;
		if (AttackComboAnimData->AttackMontage)
		{
			AttackMontage = AttackComboAnimData->AttackMontage;
		}
		MaxComboCount = AttackComboAnimData->MaxComboCount;
	}
}

void UDWEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDWEnemyCombatComponent::BeginAttack()
{
	/*
	 * 1. 애니메이션 몽타주 시작
	 * 2. 콤보 체크 (공격 범위 내 타겟 존재하는지 체크)
	 * 3. 공격 끝
	 */
	
}

void UDWEnemyCombatComponent::EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
}

void UDWEnemyCombatComponent::CombatChangeCanCombo() const
{
}

void UDWEnemyCombatComponent::CombatCheckCombo() const
{
}

void UDWEnemyCombatComponent::StartCombo()
{
}

void UDWEnemyCombatComponent::EndCombo()
{
}
