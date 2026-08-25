// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat/DWCharacterStatManager.h"

DEFINE_LOG_CATEGORY(LogDWCharacterStatManager);

/**
 * This Class is Deprecated
 * This Class is Deprecated
 * This Class is Deprecated
 * This Class is Deprecated
 * This Class is Deprecated
 */

UDWCharacterStatManager::UDWCharacterStatManager()
{
	ConstructorHelpers::FObjectFinder<UDataTable> CharacterStatTableRef(TEXT("/Script/Engine.DataTable'/Game/Dark_World/StatData/DT_DWPlayerDefaultStat.DT_DWPlayerDefaultStat'"));
	if (CharacterStatTableRef.Succeeded())
	{
		const UDataTable* DataTable = CharacterStatTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
		
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, PlayerDefaultStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FDWPlayerDefaultStatData*>(Value);
			});
	}
}

UDWCharacterStatManager& UDWCharacterStatManager::Get()
{
	UDWCharacterStatManager* Singleton = CastChecked<UDWCharacterStatManager>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}
	
	UE_LOG(LogDWCharacterStatManager, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UDWCharacterStatManager>();
}
