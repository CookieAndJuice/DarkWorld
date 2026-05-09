// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/DWLockOnInterface.h"
#include "DWCharacterBase.generated.h"

UCLASS()
class DARKWORLD_API ADWCharacterBase : public ACharacter, public IDWLockOnInterface
{
	GENERATED_BODY()

public:
	ADWCharacterBase();
	
	virtual void SetLockOnUIVisibility(bool bIsVisible) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UDWCharacterAppearanceData> CharacterAppearanceData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LockOn")
	TObjectPtr<class UWidgetComponent> LockOnWidgetComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockOn")
	TSubclassOf<class UUserWidget> LockOnUI;
};
