#include "TwitchSDK.h"
#include "UnrealPAL.h"
#include "TwitchSDKSettings.h"
#include "TwitchSDKSettingsDetails.h"
#include "TwitchSDKBPLibrary.h"
#if WITH_EDITOR
#include "PropertyEditorModule.h"
#endif

#define LOCTEXT_NAMESPACE "FTwitchAPIModule"

using namespace R66;

FTwitchSDKModule* FTwitchSDKModule::Singleton = NULL;

void FTwitchSDKModule::StartupModule()
{
	Singleton = this;

#if WITH_EDITOR
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(UTwitchSDKSettings::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FTwitchSDKSettingsDetails::MakeInstance));
#endif

	auto settings = GetDefault<UTwitchSDKSettings>();
	PAL = std::make_shared<UnrealPAL>();
	if (settings->ClientId.IsEmpty())
	{
		UE_LOG(LogTwitchSDK, Error, TEXT("No OAuth Client ID found. Please open Project Settings -> Twitch."));
	}
	Core = R66::R66Api::Initialize(PAL, FromFString(settings->ClientId), settings->UseEventSubProxy);
}

void FTwitchSDKModule::ShutdownModule()
{
	PAL->Shutdown();
	Core.reset();

	Singleton = nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTwitchSDKModule, TwitchSDK);

DEFINE_LOG_CATEGORY(LogTwitchSDK);

R66::string R66::BuildOAuthScopes(TArray<FTwitchSDKOAuthScope> scopes, TArray<FString> customScopes) {
	FString s = UTwitchSDKBPLibrary::BuildOAuthScopes(scopes, customScopes);
	return R66::string(FromFString(s));
}
