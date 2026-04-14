// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DWCharacterBase.generated.h"

UCLASS()
class DARKWORLD_API ADWCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADWCharacterBase();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UDWCharacterAppearanceData> CharacterAppearanceData;
};
