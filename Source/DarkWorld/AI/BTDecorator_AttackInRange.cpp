// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"

#include "AIController.h"
#include "DWBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/DWEnemyAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}
	
	IDWEnemyAIInterface* AIPawn = Cast<IDWEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}
	
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}
	
	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackRange = AIPawn->GetAttackRange();
	bool bResult = (DistanceToTarget <= AttackRange);
	
	return bResult;
}
