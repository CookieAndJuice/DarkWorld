// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DWLockableTargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDWLockableTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKWORLD_API IDWLockableTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetLockLocation() const = 0;
};
