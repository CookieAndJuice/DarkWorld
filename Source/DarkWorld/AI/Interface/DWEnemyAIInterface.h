// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWEnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDWEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);

/**
 * 
 */
class DARKWORLD_API IDWEnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetDetectRange() const = 0;
	virtual float GetAttackRange() const = 0;
	
	virtual class UBehaviorTree* GetBehaviorTree() const = 0;
	virtual class UBlackboardData* GetBlackboardData() const = 0;
	
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;
};