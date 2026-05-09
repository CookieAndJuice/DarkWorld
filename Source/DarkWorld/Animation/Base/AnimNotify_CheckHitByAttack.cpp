// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CheckHitByAttack.h"

#include "Character/Base/DWCharacterCombatInterface.h"

UAnimNotify_CheckHitByAttack::UAnimNotify_CheckHitByAttack()
{
	WeaponName = TEXT("Default");
}

void UAnimNotify_CheckHitByAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	// 공격 시작
	IDWCharacterCombatInterface* Character = Cast<IDWCharacterCombatInterface>(MeshComp->GetOwner());
	if (Character)
	{
		Character->StartCombo(WeaponName);
	}
}

void UAnimNotify_CheckHitByAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	// 공격 끝
	IDWCharacterCombatInterface* Character = Cast<IDWCharacterCombatInterface>(MeshComp->GetOwner());
	if (Character)
	{
		Character->EndCombo(WeaponName);
	}
}
