// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

/**
 * 
 */
class FDWCharacterDetails : public IDetailCustomization
{	
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& Layout) override;
	
	static TSharedRef<IDetailCustomization> MakeInstance();
	
private:
	FReply OnRefreshPresetClicked();
	
private:
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
};
