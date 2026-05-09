// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerLookComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/Base/DWCharacterBase.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper/DWCollisionName.h"
#include "Helper/LogDebugger.h"


// Sets default values for this component's properties
UDWPlayerLookComponent::UDWPlayerLookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWLook.IA_DWLook'"));
	ConstructorHelpers::FObjectFinder<UInputAction> LockOnActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWLockOn.IA_DWLockOn'"));
	
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}
	if (LockOnActionRef.Succeeded())
	{
		LockOnAction = LockOnActionRef.Object;
	}
	
	FindTargetMaxRange = 2000.f;
	LockOnSwitchThreshold = 9.f;
	LockOnSwitchCooldown = 0.3f;
	LockOnYawBoundary = 10.f;
}

void UDWPlayerLookComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDWPlayerLookComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (LockOnSwitchCooldownRemaining > 0.f)
	{
		LockOnSwitchCooldownRemaining -= DeltaTime;
	}
	if (bIsLockOn)
	{
		// 타겟에게 플레이어 시야가 고정된다.
		UpdateLockOnRotation(DeltaTime);
	}
	if (bIsFacingForward)
	{
		FaceToForward(DeltaTime);
	}
}

void UDWPlayerLookComponent::SetInputBinding(class UEnhancedInputComponent* InputComponent)
{
	Super::SetInputBinding(InputComponent);
	
	InputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UDWPlayerLookComponent::InputLook);
	InputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &UDWPlayerLookComponent::InputLookCompleted);
	InputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &UDWPlayerLookComponent::InputLockOn);
}

void UDWPlayerLookComponent::InputLook(const FInputActionValue& Value)
{
	if (bIsFacingForward)
	{
		return;
	}

	FVector2D LookVector = Value.Get<FVector2D>();
	if (bIsLockOn)
	{
		ChangeLockOnTarget(LookVector);
	}
	else
	{
		Owner->AddControllerYawInput(LookVector.X);
		Owner->AddControllerPitchInput(LookVector.Y);
	}
}

void UDWPlayerLookComponent::InputLookCompleted()
{
	bIsDesiredLookRight = false;
	LookAccumulatedX = 0.f;
	// UESCREEN_L(FColor::Green, TEXT("LookAccumulatedX : 0!!!!!"));
}

void UDWPlayerLookComponent::InputLockOn()
{
	UESCREEN_L(FColor::Cyan, TEXT("LockOn : %s"), bIsLockOn ? TEXT("true") : TEXT("false"));
	if (!bIsLockOn)
	{
		/*
		 * 1. 플레이어 시야 내에서 가장 중심에 있는 타겟을 찾는다.
		 * 2. 타겟에게 플레이어 시야가 고정된다.
		 * 3. 플레이어 애니메이션도 전용으로 바뀐다.
		 * 4. 적에게 락온 UI 생성
		 */
	
		// 플레이어 시야 내에서 가장 중심에 있는 타겟을 찾는다.
		TArray<FOverlapResult> OverlapResults;
		FindNearTargets(FindTargetMaxRange, OverlapResults);
		bool bFindResult = FindLockOnTarget(OverlapResults);
		
		if (bFindResult)
		{
			LockOnCamera();
		}
		else
		{
			bIsFacingForward = true;
			DesiredFaceDir = Owner->GetActorForwardVector();
		}
		
		UESCREEN_L(FColor::White, TEXT("%s"), nullptr == LockOnTarget ? TEXT("No Target") : *LockOnTarget->GetName());
	}
	else
	{
		LockOffCamera();
	}
}

void UDWPlayerLookComponent::LockOnCamera()
{
	bIsLockOn = true;
	// Control 설정 변경
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	// 락온 UI 생성
	SetLockOnUI(LockOnTarget, true);
}

void UDWPlayerLookComponent::LockOffCamera()
{
	// 락온 UI 해제
	SetLockOnUI(LockOnTarget, false);
	// 락온 해제
	bIsLockOn = false;
	LockOnTarget = nullptr;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UDWPlayerLookComponent::SetLockOnUI(AActor* Target, bool bVisible)
{
	// 락온 UI Visible / Invisible
	IDWLockOnInterface* LockOnTargetInterface = Cast<IDWLockOnInterface>(Target);
	if (LockOnTargetInterface != nullptr)
	{
		LockOnTargetInterface->SetLockOnUIVisibility(bVisible);
	}
}

void UDWPlayerLookComponent::ChangeLockOnTarget(FVector2D InLookVector)
{
	// 1. 정방향 입력값 누적
	// 2. 역방향 입력될 시 초기화 & 재누적
	// 3. 임계값 초과 시 락온 대상 변경
	if (bIsDesiredLookRight != (InLookVector.X > 0.f))
	{
		LookAccumulatedX = InLookVector.X;
		bIsDesiredLookRight = InLookVector.X > 0.f;
	}
	else
	{
		LookAccumulatedX += InLookVector.X;
	}
	
	// 임계값 미달
	if (FMath::Abs(LookAccumulatedX) < LockOnSwitchThreshold)
	{
		// UESCREEN_L(FColor::Red, TEXT("LookAccumulatedX: %f"), LookAccumulatedX);
		return;
	}

	// 임계값 초과 -> 방향 결정 후 타겟 탐색
	LookAccumulatedX = 0.f;

	// 타겟 전환 쿨다운 중이면 전환 무시 (빠른 연속 전환 시 카메라가 좌우로 흔들리는 것 방지)
	if (LockOnSwitchCooldownRemaining > 0.f)
		return;
	LockOnSwitchCooldownRemaining = LockOnSwitchCooldown;
	
	// 플레이어 근처에 있는 타겟을 찾는다.
	TArray<FOverlapResult> OverlapResults;
	FindNearTargets(FindTargetMaxRange, OverlapResults);
	
	// 적합한 값을 저장할 변수들
	float BestDist = FLT_MAX;
	AActor* BestTarget = nullptr;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	// 락온 중인 타겟의 Screen Position
	FVector2D LockOnScreenPos;
	IsInCameraViewport(PC, LockOnTarget, LockOnScreenPos, ViewportSize);
	for (auto& OverlapResult : OverlapResults)
	{
		// 같은 방향 중 현재 타겟에 가장 가까운 액터 정하기
		AActor* Target = OverlapResult.GetActor();
		
		// 현재 순회 중인 대상이 현재 LockOn 대상이면 넘기기
		if (LockOnTarget == Target) continue;
	
		FVector2D ScreenPos;
		if (!IsInCameraViewport(PC, Target, ScreenPos, ViewportSize))
			continue;

		// 양수 - 오른쪽, 음수 - 왼쪽
		bool bIsAtRight = (ScreenPos.X - LockOnScreenPos.X > 0);
		float ScreenDist = FVector2D::Distance(ScreenPos, LockOnScreenPos);
		if ((bIsAtRight == bIsDesiredLookRight) && ScreenDist < BestDist)
		{
			BestDist = ScreenDist;
			BestTarget = Target;
		}
	}
	
	if (nullptr == BestTarget)
		return;
	
	// 락온 UI 해제
	SetLockOnUI(LockOnTarget, false);
	
	LockOnTarget = BestTarget;
	
	// 락온 UI 생성
	SetLockOnUI(LockOnTarget, true);
}

void UDWPlayerLookComponent::UpdateLockOnRotation(float DeltaSeconds)
{
	if (nullptr == LockOnTarget) return;
	
	FVector DesiredLookDir = LockOnTarget->GetActorLocation() - Owner->GetActorLocation();
	FRotator DesiredLookRot = DesiredLookDir.Rotation();
	FRotator ControlRot = Owner->GetControlRotation();

	// 허용 범위: 카메라 기준 ±LockOnYawBoundary 도 이내에 있으면 카메라 정지
	float YawDiff = FRotator::NormalizeAxis(DesiredLookRot.Yaw - ControlRot.Yaw);
	if (FMath::Abs(YawDiff) <= LockOnYawBoundary)
		return;

	// 경계를 벗어난 경우 : 중앙이 아니라 경계선으로 보간
	// 타겟이 경계선 위에 딱 걸치도록 카메라를 회전시킨다.
	float BoundaryYaw = DesiredLookRot.Yaw - FMath::Sign(YawDiff) * LockOnYawBoundary;
	FRotator BoundaryRot = FRotator(DesiredLookRot.Pitch, BoundaryYaw, DesiredLookRot.Roll);

	FRotator InterpRot = FMath::RInterpTo(ControlRot, BoundaryRot, DeltaSeconds, 10.f);
	Owner->GetController()->SetControlRotation(InterpRot);
}

void UDWPlayerLookComponent::FaceToForward(float DeltaSeconds)
{
	// 플레이어 방향으로 카메라 회전
	FRotator TargetRot = DesiredFaceDir.Rotation();
	
	FRotator InterpRot = FMath::RInterpTo(Owner->GetControlRotation(), TargetRot, DeltaSeconds, 10.f);
	Owner->GetController()->SetControlRotation(InterpRot);
	
	FRotator DiffRot = FRotator(
	FRotator::NormalizeAxis(TargetRot.Pitch - InterpRot.Pitch),
	FRotator::NormalizeAxis(TargetRot.Yaw - InterpRot.Yaw),
	FRotator::NormalizeAxis(TargetRot.Roll - InterpRot.Roll)
	);
	
	// 도중에 마우스를 돌리면 0에 가까워지지 않아서 문제 발생.
	// 따라서 bIsFacingForward 도중에는 마우스 입력 불가 처리.
	if (DiffRot.IsNearlyZero(1.f))
	{
		Owner->GetController()->SetControlRotation(TargetRot);
		bIsFacingForward = false;
	}
}

void UDWPlayerLookComponent::FindNearTargets(float ViewRange, TArray<FOverlapResult>& OutEnemyResults)
{
	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return;
	}
	
	// 락온 가능한 녀석들 가져올 수 있게 인터페이스를 만들든, CollisionProfile을 만들든 하자.
	// LockOn 전용 CollisionProfile 만들고 OverlapMultiByProfile() 함수 이용하여 성공.
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(LockOnDetect), false, Owner);
	World->OverlapMultiByProfile(
		OutEnemyResults,
		Owner->GetActorLocation(),
		FQuat::Identity,
		FName(CPROFILE_DWLOCKON),
		FCollisionShape::MakeSphere(ViewRange),
		CollisionQueryParams
		);
}

bool UDWPlayerLookComponent::FindLockOnTarget(const TArray<FOverlapResult>& InEnemyResults)
{
	// 적합한 값을 저장할 변수들
	float BestDist = FLT_MAX;
	AActor* BestTarget = nullptr;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	FVector2D ScreenCenter(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
		
	for (auto& OverlapResult : InEnemyResults)
	{
		// 카메라 가운데에 가장 가까운 액터 정하기
		AActor* Target = OverlapResult.GetActor();
		FVector2D ScreenPos;
		if (!IsInCameraViewport(PC, Target, ScreenPos, ViewportSize))
			continue;

		float ScreenDist = FVector2D::Distance(ScreenPos, ScreenCenter);
		if (ScreenDist < BestDist)
		{
			BestDist = ScreenDist;
			BestTarget = Target;
		}
	}
	LockOnTarget = BestTarget;
	
	return (nullptr != LockOnTarget);
}

bool UDWPlayerLookComponent::IsInCameraViewport(APlayerController* PC, AActor* Target, FVector2D& ScreenPos, const FVector2D& ViewportSize)
{
	// 카메라 앞에 위치하고 있는가?
	bool bIsInScreen = PC->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos);
	if (!bIsInScreen)
	{
		return false;
	}
		
	// 화면을 약간 벗어나도 인식된다. 카메라 내에 담을 수 있게 하자.
	// Viewport 내에 들어오는가?
	if (ScreenPos.X < 0 || ScreenPos.X > ViewportSize.X
	|| ScreenPos.Y < 0 || ScreenPos.Y > ViewportSize.Y)
	{
		return false;
	}
	
	return true;
}
