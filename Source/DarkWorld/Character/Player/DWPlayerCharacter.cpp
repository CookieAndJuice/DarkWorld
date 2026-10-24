// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/DWPlayerCharacter.h"

#include "DWPlayerCombatComponent.h"
#include "DWPlayerLookComponent.h"
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
#include "Helper/LogDebugger.h"

ADWPlayerCharacter::ADWPlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_DWPLAYERCAPSULE);
	
	PlayerMoveComponent = CreateDefaultSubobject<UDWPlayerMoveComponent>(TEXT("PlayerMoveComponent"));
	PlayerStatComponent = CreateDefaultSubobject<UDWPlayerStatComponent>(TEXT("PlayerStatComponent"));
	PlayerCombatComponent = CreateDefaultSubobject<UDWPlayerCombatComponent>(TEXT("PlayerCombatComponent"));
	PlayerLookComponent = CreateDefaultSubobject<UDWPlayerLookComponent>(TEXT("PlayerLookComponent"));
	
	// SpringArm & Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
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
	
	AutoPossessPlayer = EAutoReceiveInput::Type::Player0;
}

void ADWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(IMC_Player, 0);
	}
	
	PlayerCombatComponent->AddWeaponInMap(RightHand);
	PlayerCombatComponent->AddWeaponInMap(LeftHand);
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
		PlayerLookComponent->SetInputBinding(EIC);
		PlayerCombatComponent->SetInputBinding(EIC);
	}
}

bool ADWPlayerCharacter::GetIsLockOn() const
{
	return PlayerLookComponent->GetIsLockOn();
}

void ADWPlayerCharacter::ChangeCanCombo() const
{
	PlayerCombatComponent->ChangeCanCombo();
}

void ADWPlayerCharacter::CheckNextCombo() const
{
	PlayerCombatComponent->CheckNextCombo();
}

void ADWPlayerCharacter::StartCombo(FName WeaponName) const
{
	PlayerCombatComponent->StartCombo(WeaponName);
}

void ADWPlayerCharacter::EndCombo(FName WeaponName) const
{
	PlayerCombatComponent->EndCombo(WeaponName);
}

float ADWPlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	PlayerStatComponent->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

#if WITH_EDITOR
void ADWPlayerCharacter::OnRefreshPreset()
{
	Super::OnRefreshPreset();
	
	ApplyPlayerControlData();
}

void ADWPlayerCharacter::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ADWPlayerCharacter, PlayerControlData))
	{
		ApplyPlayerControlData();
	}
}

void ADWPlayerCharacter::ApplyCharacterAppearanceData()
{
	Super::ApplyCharacterAppearanceData();
	
	if (IsValid(CharacterAppearanceData) == false) return;
	
	SpringArm->SetRelativeLocationAndRotation(CharacterAppearanceData->SpringArmLocation, CharacterAppearanceData->SpringArmRotation);
	SpringArm->TargetArmLength = CharacterAppearanceData->TargetArmLength;
}

void ADWPlayerCharacter::ApplyPlayerControlData()
{
	if (IsValid(PlayerControlData) == false) return;
		
	// Input Mapping Context
	if (PlayerControlData->InputMappingContext)
	{
		IMC_Player = PlayerControlData->InputMappingContext;
	}
		
	// Init Control
	UseDefaultMouseControl();
}

void ADWPlayerCharacter::UseDefaultMouseControl()
{
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
#endif
