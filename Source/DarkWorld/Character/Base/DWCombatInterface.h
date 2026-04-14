// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDWCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKWORLD_API IDWCombatInterface
{
	GENERATED_BODY()

public:
	virtual void CombatChangeCanCombo() const = 0;
	virtual void CombatCheckCombo() const = 0;
	
	virtual void StartAttack() = 0;
	virtual void EndAttack() = 0;
};
