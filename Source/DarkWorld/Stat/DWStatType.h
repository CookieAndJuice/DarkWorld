#pragma once

#include "CoreMinimal.h"
#include "DWStatType.generated.h"

UENUM(BlueprintType)
enum class EDWDefaultStatType : uint8
{
	Vitality,
	Vigor,
	Capacity,
	Motivity,
	Technique
};

UENUM(BlueprintType)
enum class EDWCommonStatType : uint8
{
	HP,
	Defense,
	Stamina,
	EquipmentWeight,
	Damage
};

USTRUCT(BlueprintType)
struct FDWStatKey
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EDWCommonStatType CommonStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName CurveName;
};

USTRUCT(BlueprintType)
struct FDWStatRelation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDWStatKey> CommonStatKeyMap;
};