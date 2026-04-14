// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWPlayerControlData.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWPlayerControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	// Pawn 섹션에서 캐릭터의 경우 Control Rotation의 Yaw 값을 주로 체크함.
	// 이 Yaw 값만 체크할 지 안 할지 결정하기 위한 변수를 추가함.	
	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;

	// Movement에서 사용할 변수들.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float RunSpeed;

	// 앞으로 사용할 입력 매핑 컨텍스트를 담을 멤버 변수.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	// 스프링 암에 관련된 여러 속성들을 지정할 수 있는 변수들.
	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
};
