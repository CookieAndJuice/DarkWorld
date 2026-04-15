// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Player/DWPlayerAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/DWPlayerCharacter.h"

UDWPlayerAnimInstance::UDWPlayerAnimInstance()
{
	MovingThreshold = 3.f;
}

void UDWPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ACharacter* Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		Player = Cast<ADWPlayerCharacter>(Owner);
	}
	
	if (nullptr == Movement || nullptr == Player) return;
	
	Velocity = Movement->Velocity;
	GroundSpeed = Velocity.Size2D();
	YawDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Player->GetActorRotation());
	bIsIdle = GroundSpeed < MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling && (Velocity.Z > JumpThreshold);
	bIsLockOn = Player->GetIsLockOn();
}