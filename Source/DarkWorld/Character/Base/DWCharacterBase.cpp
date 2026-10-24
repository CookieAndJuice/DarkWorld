// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/DWCharacterBase.h"

#include "DWCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/DWCharacterAppearanceData.h"
#include "GameFramework/SpringArmComponent.h"
#include "Helper/LogDebugger.h"

ADWCharacterBase::ADWCharacterBase()
{
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetVisibility(false);
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetDrawSize(FVector2D(50, 50));
}

void ADWCharacterBase::SetLockOnUIVisibility(bool bIsVisible)
{
	LockOnWidgetComponent->SetVisibility(bIsVisible);
}

#if WITH_EDITOR
void ADWCharacterBase::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	// UELOG_W(LogTemp, TEXT("Property Changed!: %s"), *PropertyName.ToString());
	UELOG_W(LogTemp,
		TEXT("Object=%s | CDO=%d | Property=%s | ChangeType=%u"),
		*GetPathName(),
		HasAnyFlags(RF_ClassDefaultObject) ? 1 : 0,
		*PropertyName.ToString(),
		static_cast<uint32>(PropertyChangedEvent.ChangeType));
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ADWCharacterBase, CharacterAppearanceData))
	{
		ApplyCharacterAppearanceData();
	}
}

void ADWCharacterBase::OnRefreshPreset()
{
	ApplyCharacterAppearanceData();
}

void ADWCharacterBase::ApplyCharacterAppearanceData()
{
	if (IsValid(CharacterAppearanceData) == false) return;
	
	// Capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(CharacterAppearanceData->CapsuleHalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(CharacterAppearanceData->CapsuleRadius);
	
	// Skeletal Mesh
	if (CharacterAppearanceData->SkeletalMesh)
	{
		GetMesh()->SetSkeletalMesh(CharacterAppearanceData->SkeletalMesh);
		GetMesh()->SetRelativeLocationAndRotation(CharacterAppearanceData->MeshLocation, CharacterAppearanceData->MeshRotation);
	}
	
	// AnimInstance
	if (CharacterAppearanceData->AnimInstance)
	{
		GetMesh()->SetAnimInstanceClass(CharacterAppearanceData->AnimInstance);
	}
}
#endif

