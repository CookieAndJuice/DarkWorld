// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DWPlayerDefaultStatData.h"
#include "DWCharacterStatManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDWCharacterStatManager, Error, All);

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWCharacterStatManager : public UObject
{
	GENERATED_BODY()
	
public:
	UDWCharacterStatManager();
	
	static UDWCharacterStatManager& Get();
	
public:
	FORCEINLINE float GetPlayerVitality(int32 InLevel) const { return PlayerDefaultStatTable.IsValidIndex(InLevel) ? PlayerDefaultStatTable[InLevel].Vitality : FDWPlayerDefaultStatData().Vitality; }
	FORCEINLINE float GetPlayerVigor(int32 InLevel) const { return PlayerDefaultStatTable.IsValidIndex(InLevel) ? PlayerDefaultStatTable[InLevel].Vigor : FDWPlayerDefaultStatData().Vigor; }
	FORCEINLINE float GetPlayerCapacity(int32 InLevel) const { return PlayerDefaultStatTable.IsValidIndex(InLevel) ? PlayerDefaultStatTable[InLevel].Capacity : FDWPlayerDefaultStatData().Capacity; }
	FORCEINLINE float GetPlayerMotivity(int32 InLevel) const { return PlayerDefaultStatTable.IsValidIndex(InLevel) ? PlayerDefaultStatTable[InLevel].Motivity : FDWPlayerDefaultStatData().Motivity; }
	FORCEINLINE float GetPlayerTechnique(int32 InLevel) const { return PlayerDefaultStatTable.IsValidIndex(InLevel) ? PlayerDefaultStatTable[InLevel].Technique : FDWPlayerDefaultStatData().Technique; }
	
private:
	TArray<FDWPlayerDefaultStatData> PlayerDefaultStatTable;
};
