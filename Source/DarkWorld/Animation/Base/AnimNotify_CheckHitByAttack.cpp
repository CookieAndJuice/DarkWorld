// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CheckHitByAttack.h"

#include "Character/Base/DWCombatInterface.h"

UAnimNotify_CheckHitByAttack::UAnimNotify_CheckHitByAttack()
{
}

void UAnimNotify_CheckHitByAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	// 공격 시작
	IDWCombatInterface* Character = Cast<IDWCombatInterface>(MeshComp->GetOwner());
	if (Character)
	{
		Character->StartAttack();
	}
}

void UAnimNotify_CheckHitByAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	// 공격 끝
	IDWCombatInterface* Character = Cast<IDWCombatInterface>(MeshComp->GetOwner());
	if (Character)
	{
		Character->EndAttack();
	}
}
