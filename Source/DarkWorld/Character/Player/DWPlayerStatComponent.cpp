// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerStatComponent.h"

#include "Stat/DWCharacterStatManager.h"


// Sets default values for this component's properties
UDWPlayerStatComponent::UDWPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHp = 300.f;
	CurrentHp = MaxHp;
	Defense = 0.f;
	Stamina = 100.f;
	Damage = 125.f;
	EquipmentWeight = 85.f;
}

void UDWPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Player Data에서 레벨 갖고 올 것
	
	// Player Data로 바꿀 것
	MaxHp += UDWCharacterStatManager::Get().GetPlayerVitality(VitalityLevel);
	CurrentHp = MaxHp;
	Defense += UDWCharacterStatManager::Get().GetPlayerVitality(VitalityLevel) * 0.5f;
	Stamina += UDWCharacterStatManager::Get().GetPlayerVigor(VigorLevel);
	EquipmentWeight += UDWCharacterStatManager::Get().GetPlayerCapacity(CapacityLevel);
	Damage += UDWCharacterStatManager::Get().GetPlayerMotivity(MotivityLevel) + UDWCharacterStatManager::Get().GetPlayerTechnique(TechniqueLevel);
}
