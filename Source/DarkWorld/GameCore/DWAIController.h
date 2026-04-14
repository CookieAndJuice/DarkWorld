// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DWAIController.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API ADWAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ADWAIController();
	
	void RunAI();
	void StopAI();
	
protected:
	// AI가 possess될 때 발생하는 이벤트 함수
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
