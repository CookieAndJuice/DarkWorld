// Fill out your copyright notice in the Description page of Project Settings.


#include "DWCharacterBaseComponent.h"

#include "DWCharacterBase.h"


// Sets default values for this component's properties
UDWCharacterBaseComponent::UDWCharacterBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}


// Called when the game starts
void UDWCharacterBaseComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UDWCharacterBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	Owner = Cast<ADWCharacterBase>(GetOwner());
}

