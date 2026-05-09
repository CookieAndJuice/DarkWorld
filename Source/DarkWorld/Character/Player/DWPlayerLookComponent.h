// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "InputActionValue.h"
#include "DWPlayerLookComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWPlayerLookComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	UDWPlayerLookComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetInputBinding(class UEnhancedInputComponent* InputComponent) override;
	bool GetIsLockOn() const { return bIsLockOn; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> LockOnAction;
	
protected:
	// 입력 함수
	void InputLook(const FInputActionValue& Value);
	void InputLookCompleted();
	void InputLockOn();
	
	// 락온 함수
	void LockOnCamera();
	void LockOffCamera();
	void SetLockOnUI(AActor* Target, bool bVisible);
	void ChangeLockOnTarget(FVector2D InLookVector);
	
	// TickComponent 호출 함수
	void UpdateLockOnRotation(float DeltaSeconds);
	void FaceToForward(float DeltaSeconds);
	
	// 헬퍼 함수
	void FindNearTargets(float ViewRange, TArray<FOverlapResult>& OutEnemyResults);
	bool FindLockOnTarget(const TArray<FOverlapResult>& InEnemyResults);
	bool IsInCameraViewport(APlayerController* PC, AActor* Target, FVector2D& ScreenPos, const FVector2D& ViewportSize);
	
private:
	UPROPERTY()
	TObjectPtr<AActor> LockOnTarget;
	
	// 부울 값
	bool bIsLockOn = false;
	bool bIsDesiredLookRight = false;
	bool bIsFacingForward = false;
	
	// 누적 및 유지되는 값
	float LookAccumulatedX = 0.f;
	float LockOnSwitchCooldownRemaining = 0.f;
	FVector DesiredFaceDir = FVector::ZeroVector;
	
	// 타겟 감지 최대 범위
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LockOn", meta=(AllowPrivateAccess))
	float FindTargetMaxRange;
	
	// 타겟 전환 시 마우스 이동 임계값(누적값)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LockOn", meta=(AllowPrivateAccess))
	float LockOnSwitchThreshold;

	// 타겟 전환 후 다음 전환까지 쿨타임 (초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LockOn", meta=(AllowPrivateAccess))
	float LockOnSwitchCooldown;
	
	// 회전 경계값 (도 단위) : 카메라 기준, 타겟이 이 각도를 벗어나면 카메라가 회전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LockOn", meta=(AllowPrivateAccess))
	float LockOnYawBoundary;
};
