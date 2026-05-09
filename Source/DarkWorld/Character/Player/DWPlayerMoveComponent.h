// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/Base/DWCharacterBaseComponent.h"
#include "DWPlayerMoveComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKWORLD_API UDWPlayerMoveComponent : public UDWCharacterBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDWPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetInputBinding(class UEnhancedInputComponent* InputComponent) override;
	
	void SetWalkSpeed(float InWalkSpeed) { WalkSpeed = InWalkSpeed; }
	void SetRunSpeed(float InRunSpeed) { RunSpeed = InRunSpeed; } 
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<class UInputAction> RunAction;
	
protected:
	void InputMove(const FInputActionValue& Value);
	void InputJump(const FInputActionValue& Value);
	void InputStartRun();
	void InputEndRun();
	void InputSwitchRun();
	
	void Move(float DeltaTime);
	
private:
	UPROPERTY()
	TObjectPtr<class ADWPlayerCharacter> Player;
	
	float WalkSpeed = 275.f;
	float RunSpeed = 400.f;
	bool bIsRunning = false;
	FVector DesiredDirection = FVector::ZeroVector;
};
