// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DWStatType.h"
#include "DWCharacterStatManageSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWCharacterStatManageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	float GetCommonStatFromDefaultStat(EDWDefaultStatType DefaultType, EDWCommonStatType CommonType, int32 InLevel) const;
	bool GetCommonStatsFromDefaultStat(EDWDefaultStatType DefaultType, int32 InLevel, TMap<EDWCommonStatType, float>& OutCommonStats) const;
	
protected:
	UPROPERTY()
	TObjectPtr<class UDWStatRelationDataAsset> RelationDataAsset;
	
	UPROPERTY()
	TObjectPtr<UCurveTable> StatCurveTable;
};
