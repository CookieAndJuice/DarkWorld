// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_PlayerComboCheck.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UAnimNotify_PlayerComboCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotify_PlayerComboCheck();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
