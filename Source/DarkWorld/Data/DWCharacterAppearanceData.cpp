// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DWCharacterAppearanceData.h"


UDWCharacterAppearanceData::UDWCharacterAppearanceData()
{
	CapsuleHalfHeight = 88;
	CapsuleRadius = 34;
	MeshLocation = FVector(0,0,-88.2);
	MeshRotation = FRotator(0,-90,0);
	
	SpringArmLocation = FVector(0, 0, 60);
	SpringArmRotation = FRotator(-15,0,0);
	TargetArmLength = 260.0f;
}
