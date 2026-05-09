// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/DWCharacterBase.h"

#include "DWCharacterStatComponent.h"
#include "Components/WidgetComponent.h"

ADWCharacterBase::ADWCharacterBase()
{
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetVisibility(false);
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetDrawSize(FVector2D(50, 50));
	
	ConstructorHelpers::FClassFinder<UUserWidget> LockOnUIRef(TEXT("/Game/Dark_World/UI/Character/WBP_DWLockOn.WBP_DWLockOn_C"));
	if (LockOnUIRef.Succeeded())
	{
		LockOnUI = LockOnUIRef.Class;
		LockOnWidgetComponent->SetWidgetClass(LockOnUI);
	}
}

void ADWCharacterBase::SetLockOnUIVisibility(bool bIsVisible)
{
	LockOnWidgetComponent->SetVisibility(bIsVisible);
}

