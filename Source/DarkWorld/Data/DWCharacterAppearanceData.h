// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DWCharacterAppearanceData.generated.h"

/**
 * 
 */
UCLASS()
class DARKWORLD_API UDWCharacterAppearanceData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDWCharacterAppearanceData();
	
	UPROPERTY(EditAnywhere, Category = Character)
	float CapsuleHalfHeight;
	
	UPROPERTY(EditAnywhere, Category = Character)
	float CapsuleRadius;
	
	UPROPERTY(EditAnywhere, Category = Character)
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = Character)
	TSubclassOf<UAnimInstance> AnimInstance;
	
	UPROPERTY(EditAnywhere, Category = Character)
	FVector MeshLocation;
	
	UPROPERTY(EditAnywhere, Category = Character)
	FRotator MeshRotation;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FVector SpringArmLocation;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator SpringArmRotation;
	
	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;
};
