// Fill out your copyright notice in the Description page of Project Settings.


#include "DWCharacterDetails.h"

#include "BlueprintEditor.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Character/Base/DWCharacterBase.h"
#include "Character/Player/DWPlayerCharacter.h"
#include "Helper/LogDebugger.h"
#include "Kismet2/BlueprintEditorUtils.h"


void FDWCharacterDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	
	IDetailCategoryBuilder& CustomCategory = DetailBuilder.EditCategory("Data");
	
	CustomCategory.AddProperty(DetailBuilder.GetProperty(TEXT("CharacterAppearanceData"), ADWCharacterBase::StaticClass()));
	CustomCategory.AddProperty(DetailBuilder.GetProperty(TEXT("PlayerControlData"), ADWPlayerCharacter::StaticClass()));
	
	// 'Refresh Preset' Button
	FText ButtonText = FText::FromString(TEXT("Refresh Preset"));
	CustomCategory.AddCustomRow(ButtonText).WholeRowContent().HAlign(HAlign_Left)[
		SNew(SButton).Text(ButtonText).OnClicked(this, &FDWCharacterDetails::OnRefreshPresetClicked)
	];
}

TSharedRef<IDetailCustomization> FDWCharacterDetails::MakeInstance()
{
	return MakeShareable(new FDWCharacterDetails);
}

FReply FDWCharacterDetails::OnRefreshPresetClicked()
{
	UELOG_W(LogTemp, TEXT("Refresh Preset Clicked!!!"));
	
	const FScopedTransaction Transaction(NSLOCTEXT("DWCharacterDetails", "Refresh Preset", "Refresh Character Preset"));
	
	for (const TWeakObjectPtr<UObject>& obj : CustomizedObjects)
	{
		ADWCharacterBase* Character = Cast<ADWCharacterBase>(obj.Get());
		if (!IsValid(Character))
		{
			continue;
		}
		
		// 현재 편집 대상의 클래스를 만든 BP 에셋
		UBlueprint* Blueprint = Cast<UBlueprint>(Character->GetClass()->ClassGeneratedBy);
		if (!IsValid(Blueprint))
		{
			continue;
		}
		
		// 해당 BP의 CDO
		ADWCharacterBase* CDO = Cast<ADWCharacterBase>(Blueprint->GeneratedClass->GetDefaultObject<ADWCharacterBase>());
		if (!IsValid(CDO))
		{
			continue;
		}
		
		Blueprint->Modify();
		CDO->Modify();
		
		// 외형 값이 들어 있는 컴포넌트도 Undo 대상으로 기록
		TInlineComponentArray<UActorComponent*> Components;
		CDO->GetComponents(Components);
		
		for (UActorComponent* Component : Components)
		{
			Component->Modify();
		}
		
		// CDO의 Capsule, Mesh 등에 DataAsset 값 적용
		CDO->OnRefreshPreset();
		
		// BP 기본값이 변경되었음을 에디터에 알림
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
		
		if (GEditor)
		{
			IAssetEditorInstance* AssetEditor = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->FindEditorForAsset(Blueprint, false);
			if (AssetEditor)
			{
				FAssetEditorToolkit* Toolkit = static_cast<FAssetEditorToolkit*>(AssetEditor);
				if (Toolkit->IsBlueprintEditor())
				{
					FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(Toolkit);
					
					BlueprintEditor->UpdatePreviewActor(Blueprint, true);
				}
			}
		}
		UELOG_W(LogTemp, TEXT("CDO Mesh Name: %s"), *CDO->GetMesh()->GetName());
	}
	
	return FReply::Handled();
}
