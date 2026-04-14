// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DWPlayerCharacter.h"

#include "DWPlayerCombatComponent.h"
#include "DWPlayerLockOnComponent.h"
#include "Data/DWCharacterAppearanceData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/DWPlayerControlData.h"
#include "DWPlayerMoveComponent.h"
#include "DWPlayerStatComponent.h"
#include "Character/Base/DWCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Helper/DWCollisionName.h"

ADWPlayerCharacter::ADWPlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_DWPLAYERCAPSULE);
	
	PlayerMoveComponent = CreateDefaultSubobject<UDWPlayerMoveComponent>(TEXT("PlayerMoveComponent"));
	PlayerStatComponent = CreateDefaultSubobject<UDWPlayerStatComponent>(TEXT("PlayerStatComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UDWPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerLockOnComponent = CreateDefaultSubobject<UDWPlayerLockOnComponent>(TEXT("PlayerLockOnComponent"));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	ConstructorHelpers::FObjectFinder<UDWCharacterAppearanceData> CharacterAppearanceDataRef(TEXT("/Script/DarkWorld.DWCharacterAppearanceData'/Game/Dark_World/Data/Player/DA_DWPlayerAppearance.DA_DWPlayerAppearance'"));
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
		
		// SpringArm & Camera
		SpringArm->SetupAttachment(RootComponent);
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
		SpringArm->SetRelativeLocationAndRotation(CharacterAppearanceData->SpringArmLocation, CharacterAppearanceData->SpringArmRotation);
		SpringArm->TargetArmLength = CharacterAppearanceData->TargetArmLength;
		
		// AnimInstance
		if (CharacterAppearanceData->AnimInstance)
		{
			GetMesh()->SetAnimInstanceClass(CharacterAppearanceData->AnimInstance);
		}
	}
	
	ConstructorHelpers::FObjectFinder<UDWPlayerControlData> PlayerControlDataRef(TEXT("/Script/DarkWorld.DWPlayerControlData'/Game/Dark_World/Data/Player/DA_DWPlayerControl.DA_DWPlayerControl'"));
	if (PlayerControlDataRef.Succeeded())
	{
		PlayerControlData = PlayerControlDataRef.Object;
		
		// Input Mapping Context
		if (PlayerControlData->InputMappingContext)
		{
			IMC_Player = PlayerControlData->InputMappingContext;
		}
		
		// CharacterMovemnt
		bUseControllerRotationYaw = PlayerControlData->bUseControllerRotationYaw;
		GetCharacterMovement()->bOrientRotationToMovement = PlayerControlData->bOrientRotationToMovement;
		GetCharacterMovement()->bUseControllerDesiredRotation = PlayerControlData->bUseControllerDesiredRotation;
		GetCharacterMovement()->RotationRate = PlayerControlData->RotationRate;
		GetCharacterMovement()->MaxWalkSpeed = PlayerControlData->WalkSpeed;
		PlayerMoveComponent->SetWalkSpeed(PlayerControlData->WalkSpeed);
		PlayerMoveComponent->SetRunSpeed(PlayerControlData->RunSpeed);
		
		// SpringArm
		SpringArm->bUsePawnControlRotation = PlayerControlData->bUsePawnControlRotation;
		SpringArm->bInheritPitch = PlayerControlData->bInheritPitch;
		SpringArm->bInheritYaw = PlayerControlData->bInheritYaw;
		SpringArm->bInheritRoll = PlayerControlData->bInheritRoll;
		SpringArm->bDoCollisionTest = PlayerControlData->bDoCollisionTest;
	}
	
	LeftHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(GetMesh(), TEXT("ik_hand_l"));
	LeftHand->SetCollisionProfileName(CPROFILE_DWPLAYERWEAPON);
	RightHand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(GetMesh(), TEXT("ik_hand_r"));
	RightHand->SetCollisionProfileName(CPROFILE_DWPLAYERWEAPON);
	ConstructorHelpers::FObjectFinder<UStaticMesh> RightHandWeaponMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Dark_Knight/Dark_Knight_Male/Meshes/SM_DKM_Sword.SM_DKM_Sword'"));
	if (RightHandWeaponMeshRef.Succeeded())
	{
		RightHand->SetStaticMesh(RightHandWeaponMeshRef.Object);
	}
}

void ADWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(IMC_Player, 0);
	}
}

void ADWPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC)
	{
		PlayerMoveComponent->SetInputBinding(EIC);
		PlayerCombatComponent->SetInputBinding(EIC);
		PlayerLockOnComponent->SetInputBinding(EIC);
	}
}

bool ADWPlayerCharacter::GetIsLockOn() const
{
	return PlayerLockOnComponent->GetIsLockOn();
}

void ADWPlayerCharacter::CombatChangeCanCombo() const
{
	PlayerCombatComponent->ChangeCanCombo();
}

void ADWPlayerCharacter::CombatCheckCombo() const
{
	PlayerCombatComponent->CheckCombo();
}

void ADWPlayerCharacter::StartAttack()
{
	PlayerCombatComponent->StartAttack(RightHand);
}

void ADWPlayerCharacter::EndAttack()
{
	PlayerCombatComponent->EndAttack(RightHand);
}

float ADWPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	PlayerStatComponent->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}