// Fill out your copyright notice in the Description page of Project Settings.


#include "DWEnemyCombatComponent.h"

#include "DWKhaimeraEnemy.h"
#include "Character/Base/DWCharacterBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Data/DWAttackComboAnimData.h"
#include "Engine/DamageEvents.h"
#include "Helper/DWLogCategories.h"
#include "Helper/LogDebugger.h"


UDWEnemyCombatComponent::UDWEnemyCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	ConstructorHelpers::FObjectFinder<UDWAttackComboAnimData> AttackComboAnimDataRef(TEXT("/Script/DarkWorld.DWAttackComboAnimData'/Game/Dark_World/Data/Animation/DA_DWKhaimeraAttackComboAnimData.DA_DWKhaimeraAttackComboAnimData'"));
	if (AttackComboAnimDataRef.Succeeded())
	{
		AttackComboAnimData = AttackComboAnimDataRef.Object;
		if (AttackComboAnimData->AttackMontage)
		{
			AttackMontage = AttackComboAnimData->AttackMontage;
		}
		MaxComboCount = AttackComboAnimData->MaxComboCount;
	}
}

void UDWEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto& Weapon : WeaponMap)
	{
		Weapon.Value->OnComponentBeginOverlap.AddUniqueDynamic(this, &UDWEnemyCombatComponent::OnWeaponBeginOverlap);
	}
}

void UDWEnemyCombatComponent::AddWeaponInMap(UStaticMeshComponent* Weapon)
{
	if (nullptr == Weapon)
	{
		return;
	}
	WeaponMap.Add(FName(*Weapon->GetName()), Weapon);
}

void UDWEnemyCombatComponent::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void UDWEnemyCombatComponent::BeginAttack()
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	if (nullptr == AnimInstance)
	{
		return;
	}
	AnimInstance->Montage_Play(AttackMontage);
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UDWEnemyCombatComponent::EndAttack);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}

void UDWEnemyCombatComponent::EndAttack(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentComboCount = 0;
	OnAttackFinished.ExecuteIfBound();
}

void UDWEnemyCombatComponent::ChangeCanCombo()
{
	bCanCombo = true;
}

void UDWEnemyCombatComponent::CheckNextCombo()
{
	// 적 감지가 되어야만 이어지는 콤보
	// 공격 범위에 적 감지 되었는가?
	if (bNextComboCommand)
	{
		// 그럼 다음 콤보로
	}
}

void UDWEnemyCombatComponent::StartCombo(FName WeaponName)
{
	if (!WeaponMap.Contains(WeaponName))
	{
		UELOG_W(LogTemp, TEXT("Weapon %s doesn't exist in Map"), *WeaponName.ToString());
		return;
	}
	
	UCapsuleComponent* Weapon = Cast<UCapsuleComponent>(WeaponMap[WeaponName]);
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(DWCombat, Warning, TEXT("Start %s %d Combo Attack!! %s"), *Weapon->GetName(), CurrentComboCount, Weapon->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? TEXT("QueryOnly") : TEXT("NoCollision"));
	}
}

void UDWEnemyCombatComponent::EndCombo(FName WeaponName)
{
	if (!WeaponMap.Contains(WeaponName))
	{
		UELOG_W(LogTemp, TEXT("Weapon %s doesn't exist in Map"), *WeaponName.ToString());
		return;
	}
	
	UCapsuleComponent* Weapon = Cast<UCapsuleComponent>(WeaponMap[WeaponName]);
	if (Weapon)
	{
		Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(DWCombat, Warning, TEXT("Start %s %d Combo Attack!! %s"), *Weapon->GetName(), CurrentComboCount, Weapon->GetCollisionEnabled() == ECollisionEnabled::QueryOnly ? TEXT("QueryOnly") : TEXT("NoCollision"));
	}
}

void UDWEnemyCombatComponent::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(DWCombat, Warning, TEXT("%s"), *SweepResult.GetActor()->GetName());
	if (ADWPlayerCharacter* Player = Cast<ADWPlayerCharacter>(OtherActor))
	{
		UE_LOG(DWCombat, Warning, TEXT("%s Weapon Detect Player!!!"), *OverlappedComponent->GetName());
		
		float EnemyDamage = 0;
		if (ADWKhaimeraEnemy* Enemy = Cast<ADWKhaimeraEnemy>(Owner))
		{
			EnemyDamage = Enemy->GetEnemyStatComponent()->GetDamage();
		}
		
		FDamageEvent DamageEvent;
		Player->TakeDamage(EnemyDamage, DamageEvent, Owner->GetController(), Owner);
	}
}