// Fill out your copyright notice in the Description page of Project Settings.


#include "DWCharacterStatComponent.h"

#include "DWCharacterBase.h"
#include "Helper/DWLogCategories.h"


// Sets default values for this component's properties
UDWCharacterStatComponent::UDWCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDWCharacterStatComponent::ApplyDamage(float InDamage)
{
	float PrevHp = CurrentHp;
	float ActualDamage = FMath::Clamp<float>(InDamage - Defense, 0.f, InDamage);
	
	SetHp(PrevHp - ActualDamage);
	UE_LOG(DWCombat, Warning, TEXT("%s Take %f Damage!!!"), *Owner->GetName(), InDamage);
}

void UDWCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);
	
	OnHpChanged.Broadcast(CurrentHp);
	
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		bIsDead = true;
		UE_LOG(DWCombat, Warning, TEXT("%s Died!!!"), *Owner->GetName());
	}
}
