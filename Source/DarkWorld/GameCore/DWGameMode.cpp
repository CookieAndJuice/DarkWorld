// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCore/DWGameMode.h"

ADWGameMode::ADWGameMode()
{
	ConstructorHelpers::FClassFinder<APawn> defaultPawnRef(TEXT("/Game/Dark_World/Blueprints/BP_Player.BP_Player_C"));
	if (defaultPawnRef.Succeeded())
	{
		DefaultPawnClass = defaultPawnRef.Class;
	}
	
	ConstructorHelpers::FClassFinder<APlayerController> defaultPlayerControllerRef(TEXT("/Script/CoreUObject.Class'/Script/DarkWorld.DWPlayerController'"));
	if (defaultPlayerControllerRef.Succeeded())
	{
		PlayerControllerClass = defaultPlayerControllerRef.Class;
	}	
}

void ADWGameMode::BeginPlay()
{
	Super::BeginPlay();
}
