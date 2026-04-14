// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/DWCharacterBase.h"

#include "DWCharacterStatComponent.h"

ADWCharacterBase::ADWCharacterBase()
{
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

