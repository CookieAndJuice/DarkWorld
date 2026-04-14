// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore//DWPlayerController.h"

void ADWPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly inputMode;
	SetInputMode(inputMode);
}
