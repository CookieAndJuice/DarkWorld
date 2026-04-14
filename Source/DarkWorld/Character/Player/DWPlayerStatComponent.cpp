// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerStatComponent.h"

#include "StatData/DWCharacterStatManager.h"


// Sets default values for this component's properties
UDWPlayerStatComponent::UDWPlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MaxHp = 300.f;
	CurrentHp = MaxHp;
	Defense = 0.f;
	Stamina = 100.f;
	Damage = 125.f;
	Weight = 85.f;
}

void UDWPlayerStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Player Data에서 레벨 갖고 올 것
	
	// Player Data로 바꿀 것
	MaxHp += UDWCharacterStatManager::Get().GetPlayerVitality(11);
	CurrentHp = MaxHp;
	Defense += UDWCharacterStatManager::Get().GetPlayerVitality(11) * 0.5f;
	Stamina += UDWCharacterStatManager::Get().GetPlayerVigor(5);
	Weight += UDWCharacterStatManager::Get().GetPlayerCapacity(11);
	Damage += UDWCharacterStatManager::Get().GetPlayerMotivity(5) + UDWCharacterStatManager::Get().GetPlayerTechnique(5);
}
