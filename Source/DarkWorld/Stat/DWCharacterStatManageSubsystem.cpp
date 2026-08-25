// Fill out your copyright notice in the Description page of Project Settings.


#include "DWCharacterStatManageSubsystem.h"

#include "DWStatRelationDataAsset.h"
#include "GameCore/DWDeveloperSettings.h"
#include "Engine/CurveTable.h"


void UDWCharacterStatManageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	const UDWDeveloperSettings* Settings = GetDefault<UDWDeveloperSettings>();

	if (!IsValid(Settings))
	{
		return;
	}
	
	StatCurveTable = Settings->CharacterStatCurveTable.LoadSynchronous();
	RelationDataAsset = Settings->RelationData.LoadSynchronous();
}

float UDWCharacterStatManageSubsystem::GetCommonStatFromDefaultStat(EDWDefaultStatType DefaultType,
	EDWCommonStatType CommonType, int32 InLevel) const
{
	if (!IsValid(RelationDataAsset) || !IsValid(StatCurveTable))
	{
		return 0.f;
	}
	
	const FDWStatRelation* Relation = RelationDataAsset->Relations.Find(DefaultType);
	if (!Relation)
	{
		return 0.f;
	}
	
	for (const FDWStatKey& StatKey : Relation->CommonStatKeyMap)
	{
		if (CommonType != StatKey.CommonStatType) continue;
		
		const FRealCurve* Curve = StatCurveTable->FindCurve(StatKey.CurveName, TEXT("GetCommonStatFromDefaultStat"));
		if (!Curve)
		{
			return 0.f;
		}
		
		return Curve->Eval(InLevel);
	}
	
	return 0.f;
}

bool UDWCharacterStatManageSubsystem::GetCommonStatsFromDefaultStat(
	EDWDefaultStatType DefaultType, int32 InLevel, TMap<EDWCommonStatType, float>& OutCommonStats) const
{
	OutCommonStats.Reset();
	if (!IsValid(RelationDataAsset) || !IsValid(StatCurveTable))
	{
		return false;
	}
	
	const FDWStatRelation* Relation = RelationDataAsset->Relations.Find(DefaultType);
	if (!Relation)
	{
		return false;
	}
	
	for (const FDWStatKey& StatKey : Relation->CommonStatKeyMap)
	{
		const FRealCurve* Curve = StatCurveTable->FindCurve(StatKey.CurveName, TEXT("GetCommonStatsFromDefaultStat"));
		if (!Curve)
		{
			continue;
		}
		
		OutCommonStats.Add(StatKey.CommonStatType, Curve->Eval(InLevel));
	}
	
	return !OutCommonStats.IsEmpty();
}
