#include "CustomCharacterModule.h"

#include "DWCharacterDetails.h"
#include "Character/Base/DWCharacterBase.h"
#include "PropertyEditorModule.h"

IMPLEMENT_GAME_MODULE(FCustomCharacterModule, DarkWorldEditor);

void FCustomCharacterModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule =
		FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	PropertyModule.RegisterCustomClassLayout(
		ADWCharacterBase::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FDWCharacterDetails::MakeInstance));
	
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FCustomCharacterModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule =
			FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		
		PropertyModule.UnregisterCustomClassLayout(
			ADWCharacterBase::StaticClass()->GetFName());
	}
}
