// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerComboCheck.h"

#include "Character/Base/DWCombatInterface.h"

UAnimNotify_PlayerComboCheck::UAnimNotify_PlayerComboCheck()
{
}

void UAnimNotify_PlayerComboCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (IDWCombatInterface* Character = Cast<IDWCombatInterface>(MeshComp->GetOwner()))
	{
		// 콤보 가능 여부 체크로
		Character->CombatChangeCanCombo();
	}
}

void UAnimNotify_PlayerComboCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (IDWCombatInterface* Character = Cast<IDWCombatInterface>(MeshComp->GetOwner()))
	{
		// 콤보 체크 함수 호출
		Character->CombatCheckCombo();
	}
}