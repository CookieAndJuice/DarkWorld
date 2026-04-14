// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerMoveComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/Base/DWCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UDWPlayerMoveComponent::UDWPlayerMoveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWMove.IA_DWMove'"));
	ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWLook.IA_DWLook'"));
	ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWJump.IA_DWJump'"));
	ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWRun.IA_DWRun'"));
	
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}
	if (RunActionRef.Succeeded())
	{
		RunAction = RunActionRef.Object;
	}
}

// Called when the game starts
void UDWPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UDWPlayerMoveComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called every frame
void UDWPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UDWPlayerMoveComponent::SetInputBinding(class UEnhancedInputComponent* InputComponent)
{
	Super::SetInputBinding(InputComponent);
	
	InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UDWPlayerMoveComponent::InputMove);
	InputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UDWPlayerMoveComponent::InputLook);
	InputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UDWPlayerMoveComponent::InputJump);
	InputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &UDWPlayerMoveComponent::InputStartRun);
	InputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &UDWPlayerMoveComponent::InputEndRun);
	InputComponent->BindAction(RunAction, ETriggerEvent::Canceled, this, &UDWPlayerMoveComponent::InputEndRun);
}

void UDWPlayerMoveComponent::InputMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DesiredDirection = FVector(MovementVector, 0);
}

void UDWPlayerMoveComponent::InputLook(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	
	Owner->AddControllerYawInput(LookVector.X);
	Owner->AddControllerPitchInput(LookVector.Y);
}

void UDWPlayerMoveComponent::InputJump(const FInputActionValue& Value)
{
}

void UDWPlayerMoveComponent::InputStartRun()
{
	bIsRunning = true;
	Owner->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void UDWPlayerMoveComponent::InputEndRun()
{
	bIsRunning = false;
	Owner->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void UDWPlayerMoveComponent::InputSwitchRun()
{
	bIsRunning ? InputEndRun() : InputStartRun();
}

void UDWPlayerMoveComponent::Move(float DeltaTime)
{
	if (DesiredDirection == FVector::ZeroVector) return;
	
	// 이동 방향 계산
	FRotator YawRotation = FRotator(0, Owner->GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	DesiredDirection = ForwardDirection * DesiredDirection.X + RightDirection * DesiredDirection.Y;
	DesiredDirection.Normalize();
	
	// // 이동 방향에 대한 회전값 계산
	// FRotator TargetRotation = DesiredDirection.Rotation();
	// float InterpSpeed = GetCharacterMovement()->RotationRate.Yaw;
	// FRotator NewRotation = FMath::RInterpTo(GetControlRotation(), TargetRotation, DeltaTime, InterpSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("DeltaTime : %f, InterpSpeed : %s"), DeltaTime, *GetCharacterMovement()->RotationRate.ToString());
	//
	// // 이동 위치 계산
	// FVector NewLocation = GetActorLocation() + DesiredDirection * GetCharacterMovement()->GetMaxSpeed() * DeltaTime;
	//
	// SetActorLocationAndRotation(NewLocation, NewRotation);
		
	Owner->AddMovementInput(DesiredDirection);
	DesiredDirection = FVector::ZeroVector;
}