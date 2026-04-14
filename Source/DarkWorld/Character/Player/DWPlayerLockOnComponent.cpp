// Fill out your copyright notice in the Description page of Project Settings.


#include "DWPlayerLockOnComponent.h"

#include "EnhancedInputComponent.h"
#include "Character/Base/DWCharacterBase.h"
#include "Engine/OverlapResult.h"
#include "Helper/DWCollisionName.h"
#include "Helper/LogDebugger.h"


// Sets default values for this component's properties
UDWPlayerLockOnComponent::UDWPlayerLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UInputAction> LockOnActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dark_World/Input/IA_DWLockOn.IA_DWLockOn'"));
	if (LockOnActionRef.Succeeded())
	{
		LockOnAction = LockOnActionRef.Object;
	}
}

void UDWPlayerLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDWPlayerLockOnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsLockOn)
	{
		// 타겟에게 플레이어 시야가 고정된다.
		UpdateLockOnRotation(DeltaTime);
	}
}

void UDWPlayerLockOnComponent::SetInputBinding(class UEnhancedInputComponent* InputComponent)
{
	Super::SetInputBinding(InputComponent);
	
	InputComponent->BindAction(LockOnAction, ETriggerEvent::Triggered, this, &UDWPlayerLockOnComponent::InputLockOn);
}

void UDWPlayerLockOnComponent::InputLockOn()
{
	UESCREEN_L(FColor::Cyan, TEXT("LockOn : %s"), bIsLockOn ? TEXT("true") : TEXT("false"));
	if (!bIsLockOn)
	{
		/*
		 * 1. 플레이어 시야 내에서 가장 중심에 있는 타겟을 찾는다.
		 * 2. 타겟에게 플레이어 시야가 고정된다.
		 * 3. 플레이어 애니메이션도 전용으로 바뀐다.
		 * 4. UI에 락온 발생
		 */
	
		// 1. 플레이어 시야 내에서 가장 중심에 있는 타겟을 찾는다.
		float MaxRange = 1000.f;
		TArray<FOverlapResult> OverlapResults;
		
		FindNearTargets(MaxRange, OverlapResults);
		bool bFindResult = FindLockOnTarget(OverlapResults);
		if (bFindResult)
		{
			bIsLockOn = true;
		}
		else
		{
			// 플레이어 방향으로 카메라 회전
		}
		
		UESCREEN_L(FColor::White, TEXT("%s"), nullptr == LockOnTarget ? TEXT("No Target") : *LockOnTarget->GetName());
	}
	else
	{
		// 락온 해제
		bIsLockOn = false;
	}
}

void UDWPlayerLockOnComponent::FindNearTargets(float ViewRange, TArray<FOverlapResult>& OutEnemyResults)
{
	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return;
	}
	
	// 락온 가능한 녀석들 전용 인터페이스든 뭐든 만들자.
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

bool UDWPlayerLockOnComponent::FindLockOnTarget(const TArray<FOverlapResult>& InEnemyResults)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	int32 ViewW, ViewH;
	PC->GetViewportSize(ViewW, ViewH);
	FVector2D ScreenCenter(ViewW * 0.5f, ViewH * 0.5f);

	float BestDist = FLT_MAX;
		
	for (auto& OverlapResult : InEnemyResults)
	{
		// 카메라 가운데에 가장 가까운 액터 정하기
		AActor* Target = OverlapResult.GetActor();
			
		FVector2D ScreenPos;
		bool bIsInScreen = PC->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos);
		UESCREEN_L(FColor::White, TEXT("Is In Screen? %s"), bIsInScreen ? TEXT("true") : TEXT("false"));
		if (!bIsInScreen)
		{
			continue; // 카메라 뒤에 있으면 false 반환
		}
		// 화면을 약간 벗어나도 인식된다. 나중에 고치자.

		float ScreenDist = FVector2D::Distance(ScreenPos, ScreenCenter);
		if (ScreenDist < BestDist)
		{
			BestDist = ScreenDist;
			LockOnTarget = Target;
		}
	}
	
	return nullptr != LockOnTarget;
}

void UDWPlayerLockOnComponent::UpdateLockOnRotation(float DeltaSeconds)
{
	// 락온하자.
	// 카메라의 방향을 타겟 쪽으로 회전한다.
	FVector DesiredLookDir = LockOnTarget->GetActorLocation() - Owner->GetActorLocation();
	FRotator InterpRot = FMath::RInterpTo(Owner->GetControlRotation(), DesiredLookDir.Rotation(), DeltaSeconds, 10.f);
	Owner->GetController()->SetControlRotation(InterpRot);
}
