// Fill out your copyright notice in the Description page of Project Settings.


#include "DWKhaimeraEnemy.h"

#include "DWEnemyCombatComponent.h"
#include "DWEnemyStatComponent.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/DWCharacterAppearanceData.h"
#include "GameCore/DWAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper/DWCollisionName.h"
#include "UI/DWHpBarWidget.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Helper/LogDebugger.h"
#include "StatData/DWEnemyStatData.h"


// Sets default values
ADWKhaimeraEnemy::ADWKhaimeraEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	EnemyStatComponent = CreateDefaultSubobject<UDWEnemyStatComponent>(TEXT("EnemyStatComponent"));
	EnemyCombatComponent = CreateDefaultSubobject<UDWEnemyCombatComponent>(TEXT("EnemyCombatComponent"));
	
	AIControllerClass = ADWAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_DWENEMYCAPSULE);
	
	ConstructorHelpers::FObjectFinder<UDWCharacterAppearanceData> CharacterAppearanceDataRef(TEXT("/Script/DarkWorld.DWCharacterAppearanceData'/Game/Dark_World/Data/Enemy/Khaimera/DA_DWKhaimeraAppearance.DA_DWKhaimeraAppearance'"));
	if (CharacterAppearanceDataRef.Succeeded())
	{
		CharacterAppearanceData = CharacterAppearanceDataRef.Object;
		
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
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 196.f));
	ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Dark_World/UI/Character/WBP_DWHpBar.WBP_DWHpBar_C"));
	if (HpBarWidgetRef.Succeeded())
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 10.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	LeftHand = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	LeftHand->SetCollisionProfileName(CPROFILE_DWENEMYWEAPON);
	LeftHand->SetRelativeLocationAndRotation(FVector(0,68,-8), FRotator(0,0,90));
	LeftHand->SetCapsuleHalfHeight(35.f);
	LeftHand->SetCapsuleRadius(10.f);
	RightHand = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(GetMesh(), TEXT("weapon_r"));
	RightHand->SetCollisionProfileName(CPROFILE_DWENEMYWEAPON);
	RightHand->SetRelativeLocationAndRotation(FVector(0,-68,8), FRotator(0,0,-90));
	RightHand->SetCapsuleHalfHeight(35.f);
	RightHand->SetCapsuleRadius(10.f);
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Dark_World/Animation/Enemy/Khaimera/AM_KhaimeraDeath.AM_KhaimeraDeath'"));;
	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UDWEnemyStatData> EnemyStatDataRef(TEXT("/Script/DarkWorld.DWEnemyStatData'/Game/Dark_World/StatData/Enemy/DA_KhaimeraStat.DA_KhaimeraStat'"));
	if (EnemyStatDataRef.Succeeded())
	{
		EnemyStatData = EnemyStatDataRef.Object;
		GetCharacterMovement()->MaxWalkSpeed = EnemyStatData->WalkSpeed;
	}
	
	ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/Dark_World/AI/Khaimera/BB_DWKhaimera.BB_DWKhaimera'"));
	if (BlackboardDataRef.Succeeded())
	{
		BlackboardData = BlackboardDataRef.Object;
	}
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Dark_World/AI/Khaimera/BT_DWKhaimera.BT_DWKhaimera'"));
	if (BehaviorTreeRef.Succeeded())
	{
		BehaviorTree = BehaviorTreeRef.Object;
	}
}

void ADWKhaimeraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	UDWHpBarWidget* HpBarWidget = Cast<UDWHpBarWidget>(HpBar->GetWidget());
	if (HpBarWidget)
	{
		HpBarWidget->InitHp(EnemyStatComponent->GetMaxHp());
		HpBarWidget->BindWidget(EnemyStatComponent);
	}
	EnemyStatComponent->OnHpZero.AddUObject(this, &ADWKhaimeraEnemy::SetDead);
	EnemyStatComponent->InitStat(EnemyStatData);
}

void ADWKhaimeraEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ADWKhaimeraEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	EnemyStatComponent->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void ADWKhaimeraEnemy::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	EnemyCombatComponent->SetAIAttackDelegate(InOnAttackFinished);
}

void ADWKhaimeraEnemy::AttackByAI()
{
	EnemyCombatComponent->BeginAttack();
}

void ADWKhaimeraEnemy::SetDead()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayDeadMontage();
}

void ADWKhaimeraEnemy::PlayDeadMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DeadMontage, 1.f);
	
	FOnMontageEnded OnDeadEndDelegate;
	OnDeadEndDelegate.BindLambda([this, AnimInstance](UAnimMontage* Montage, bool bInterrupted)
	{
		GetMesh()->SetSimulatePhysics(true);
	});
	AnimInstance->Montage_SetEndDelegate(OnDeadEndDelegate, DeadMontage);
}
