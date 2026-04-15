// Fill out your copyright notice in the Description page of Project Settings.


#include "DWKhaimeraAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "Character/Enemy/DWKhaimeraEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

UDWKhaimeraAnimInstance::UDWKhaimeraAnimInstance()
{
}

void UDWKhaimeraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ACharacter* Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		Enemy = Cast<ADWKhaimeraEnemy>(Owner);
	}
	
	if (nullptr == Movement || nullptr == Enemy) return;
	
	Velocity = Movement->Velocity;
	GroundSpeed = Velocity.Size2D();
	YawDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Enemy->GetActorRotation());
	bIsIdle = GroundSpeed < MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling && (Velocity.Z > JumpThreshold);
	// bIsLockOn = Enemy->GetIsLockOn();
	bIsLockOn = false;
}
