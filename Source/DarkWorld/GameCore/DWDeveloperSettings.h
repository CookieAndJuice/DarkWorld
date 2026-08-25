// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DWDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "DWDeveloperSettings"))
class DARKWORLD_API UDWDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, config, Category = "Game | Character | Stat")
	TSoftObjectPtr<UCurveTable> CharacterStatCurveTable;
	
	UPROPERTY(EditAnywhere, config, Category = "Game | Character | Stat")
	TSoftObjectPtr<class UDWStatRelationDataAsset> RelationData;
};
