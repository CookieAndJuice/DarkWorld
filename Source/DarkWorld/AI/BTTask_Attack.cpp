// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "Interface/DWEnemyAIInterface.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	IDWEnemyAIInterface* AIOwner = Cast<IDWEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIOwner)
	{
		return EBTNodeResult::Failed;
	}
	
	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda([&]()
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	});
	AIOwner->SetAIAttackDelegate(OnAttackFinished);
	AIOwner->AttackByAI();
	
	return EBTNodeResult::InProgress;
}
