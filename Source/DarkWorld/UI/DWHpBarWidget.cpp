// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DWHpBarWidget.h"

#include "Character/Base/DWCharacterStatComponent.h"
#include "Components/ProgressBar.h"

UDWHpBarWidget::UDWHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = 1.f;
}

void UDWHpBarWidget::InitHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;
	UpdateHpBar(MaxHp);
}

void UDWHpBarWidget::UpdateHpBar(float CurrentHp)
{
	ensure(MaxHp > 0.0f);
	
	if (HpBar)
	{
		HpBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UDWHpBarWidget::BindWidget(class UDWCharacterStatComponent* Stat)
{
	Stat->OnHpChanged.AddUObject(this, &UDWHpBarWidget::UpdateHpBar);
}
