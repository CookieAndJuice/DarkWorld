// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/DWAIController.h"

#include "AI/Interface/DWEnemyAIInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ADWAIController::ADWAIController()
{
	
}

void ADWAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool Result = RunBehaviorTree(BTAsset);
		ensure(Result);
	}
}

void ADWAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ADWAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	IDWEnemyAIInterface* AIPawn = Cast<IDWEnemyAIInterface>(InPawn);
	if (AIPawn)
	{
		BBAsset = AIPawn->GetBlackboardData();
		BTAsset = AIPawn->GetBehaviorTree();
		RunAI();
	}
}
