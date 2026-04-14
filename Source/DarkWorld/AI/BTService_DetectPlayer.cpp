// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_DetectPlayer.h"

#include "AIController.h"
#include "Interface/DWEnemyAIInterface.h"
#include "DrawDebugHelpers.h"
#include "DWBBKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Helper/DWCollisionName.h"
#include "Engine/OverlapResult.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("DetectPlayer");
	Interval = .5f;
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
		
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}
		
	IDWEnemyAIInterface* AIOwner = Cast<IDWEnemyAIInterface>(ControllingPawn);
	if (nullptr == AIOwner)
	{
		return;
	}
	float DetectRange = AIOwner->GetDetectRange();
	
	// SphereTrace 감지
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_DWPLAYER,
		FCollisionShape::MakeSphere(DetectRange),
		CollisionQueryParams);
	
	// Blackboard의 Target에 추가
	if (bResult)
	{
		for (const auto& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);
				DrawDebugSphere(World, Center, DetectRange, 16, FColor::Green, false, 0.5f);
				return;
			}
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectRange, 16, FColor::Red, false, 0.5f);
}
