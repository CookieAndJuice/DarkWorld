// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyStatComponent.h"

#include "Stat/DWEnemyStatData.h"


// Sets default values for this component's properties
UDWEnemyStatComponent::UDWEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MaxHp = 300.f;
	CurrentHp = MaxHp;
	Defense = 0.f;
	Stamina = 100.f;
	Damage = 125.f;
	Groggy = 20.f;
	DetectRange = 400.f;
}

void UDWEnemyStatComponent::InitStat(class UDWEnemyStatData* StatData)
{
	MaxHp = StatData->MaxHp;
	CurrentHp = MaxHp;
	Defense = StatData->Defense;
	Stamina = StatData->Stamina;
	Damage = StatData->Damage;
	Groggy = StatData->Groggy;
	DetectRange = StatData->DetectRange;
}

