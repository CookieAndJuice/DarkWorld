// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerCombatComponent.h"

#include "DWPlayerCharacter.h"
#include "DWPlayerStatComponent.h"
#include "EnhancedInputComponent.h"
#include "Character/Base/DWCharacterBase.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "Character/Enemy/DWKhaimeraEnemy.h"
#include "Data/DWAttackComboAnimData.h"
#include "Engine/DamageEvents.h"
#include "Helper/DWLogCategories.h"


// Sets default values for this component's properties
UDWPlayerCombatComponent::UDWPlayerCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UDWAttackComboAnimData> AttackComboAnimDataRef(TEXT("/Script/DarkWorld.DWAttackComboAnimData'/Game/Dark_World/Data/Animation/DA_DWPlayerAttackComboAnimData.DA_DWPlayerAttackComboAnimData'"));
	ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWAttack.IA_DWAttack'"));
	
	if (AttackComboAnimDataRef.Succeeded())
	{
		AttackComboAnimData = AttackComboAnimDataRef.Object;
		
		if (AttackComboAnimData->AttackMontage)
		{
			AttackMontage = AttackComboAnimData->AttackMontage;
		}
		MaxComboCount = AttackComboAnimData->MaxComboCount;
	}
	if (AttackActionRef.Succeeded())
	{
		AttackAction = AttackActionRef.Object;
	}
}


// Called when the game starts
void UDWPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ADWPlayerCharacter* Player = Cast<ADWPlayerCharacter>(GetOwner());
	if (Player)
	{
		Player->GetRightHand()->OnComponentBeginOverlap.AddUniqueDynamic(this, &UDWPlayerCombatComponent::OnWeaponBeginOverlap);
		Player->GetRightHand()->OnComponentEndOverlap.AddUniqueDynamic(this, &UDWPlayerCombatComponent::OnWeaponEndOverlap);
	}
}

void UDWPlayerCombatComponent::SetInputBinding(class UEnhancedInputComponent* InputComponent)
{
	Super::SetInputBinding(InputComponent);
	
	InputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &UDWPlayerCombatComponent::InputAttack);
}

void UDWPlayerCombatComponent::InputAttack(const FInputActionValue& Value)
{
	if (0 == CurrentComboCount)
	{
		BeginAttack();
	}
	
	if (bCanCombo)
	{
		bNextComboCommand = true;
		// UE_LOG(LogTemp, Warning, TEXT("bNextComboCommand : %s"), bNextComboCommand ? TEXT("true") : TEXT("false"));
	}
	else
	{
		bNextComboCommand = false;
		// UE_LOG(LogTemp, Warning, TEXT("bNextComboCommand : %s"), bNextComboCommand ? TEXT("true") : TEXT("false"));
	}
}

void UDWPlayerCombatComponent::BeginAttack()
{
	CurrentComboCount = 1;
	
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage);
	// UE_LOG(LogTemp, Warning, TEXT("CurrentComboCount : %d, MaxComboCount : %d"), CurrentComboCount, MaxComboCount);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UDWPlayerCombatComponent::EndAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void UDWPlayerCombatComponent::EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentComboCount = 0;
	// UE_LOG(LogTemp, Warning, TEXT("CurrentComboCount : %d, MaxComboCount : %d"), CurrentComboCount, MaxComboCount);
}

void UDWPlayerCombatComponent::ChangeCanCombo()
{
	bCanCombo = true;
}

void UDWPlayerCombatComponent::CheckCombo()
{
	if (bNextComboCommand)
	{
		CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 1, MaxComboCount);
		
		// UE_LOG(LogTemp, Warning, TEXT("CurrentComboCount : %d, MaxComboCount : %d"), CurrentComboCount, MaxComboCount);
		if (1 <= CurrentComboCount && CurrentComboCount <= MaxComboCount)
		{
			FName NextSection = *FString::Printf(TEXT("%s%d"), *AttackComboAnimData->MontageSectionNamePrefix, CurrentComboCount);
			Owner->GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextSection, AttackMontage);
		}
		bNextComboCommand = false;
	}
	bCanCombo = false;
	// UE_LOG(LogTemp, Warning, TEXT("bCanCombo %s"), bCanCombo ? TEXT("true") : TEXT("false"));
}

void UDWPlayerCombatComponent::StartCombo(UStaticMeshComponent* Weapon)
{
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(DWCombat, Warning, TEXT("Start %s %d Combo Attack!! %s"), *Weapon->GetName(), CurrentComboCount, Weapon->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? TEXT("QueryOnly") : TEXT("NoCollision"));
	}
}

void UDWPlayerCombatComponent::EndCombo(UStaticMeshComponent* Weapon)
{
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(DWCombat, Warning, TEXT("End %s %d Combo Attack!! %s"), *Weapon->GetName(), CurrentComboCount, Weapon->GetCollisionEnabled() == ECollisionEnabled::NoCollision ? TEXT("NoCollision") : TEXT("QueryOnly"));
	}
}

void UDWPlayerCombatComponent::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(DWCombat, Warning, TEXT("%s"), *SweepResult.GetActor()->GetName());
	if (ADWKhaimeraEnemy* enemy = Cast<ADWKhaimeraEnemy>(OtherActor))
	{
		UE_LOG(DWCombat, Warning, TEXT("%s Weapon Detect Enemy!!!"), *OverlappedComponent->GetName());
		
		float PlayerDamage = 0;
		if (ADWPlayerCharacter* Player = Cast<ADWPlayerCharacter>(Owner))
		{
			PlayerDamage = Player->GetPlayerStatComponent()->GetDamage();
		}
		
		FDamageEvent DamageEvent;
		enemy->TakeDamage(PlayerDamage, DamageEvent, Owner->GetController(), Owner);
	}
}

void UDWPlayerCombatComponent::OnWeaponEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(DWCombat, Warning, TEXT("Weapon Enemy Detect End!!!"));
}