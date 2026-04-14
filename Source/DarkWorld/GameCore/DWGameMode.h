// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DWGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API ADWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADWGameMode();
	
protected:
	virtual void BeginPlay() override;
};
