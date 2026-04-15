// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DWPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UDWPlayerAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn)
	TObjectPtr<class ADWPlayerCharacter> Player;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float YawDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsIdle : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsFalling : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsJumping : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	uint8 bIsLockOn : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovingThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float JumpThreshold;
};
