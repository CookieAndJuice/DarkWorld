// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWCharacterCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDWCharacterCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKWORLD_API IDWCharacterCombatInterface
{
	GENERATED_BODY()

public:
	// for character, not component. for Dependency Inversion Principle(DIP)
	virtual void ChangeCanCombo() const = 0;
	virtual void CheckNextCombo() const = 0;
	
	virtual void StartCombo(FName WeaponName) const = 0;
	virtual void EndCombo(FName WeaponName) const = 0;
};
