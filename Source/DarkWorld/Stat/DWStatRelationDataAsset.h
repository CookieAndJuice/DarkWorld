// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWStatType.h"
#include "DWStatRelationDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWStatRelationDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDWDefaultStatType, FDWStatRelation> Relations;
};
