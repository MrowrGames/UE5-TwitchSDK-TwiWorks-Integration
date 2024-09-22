// Fill out your copyright notice in the Description page of Project Settings.


#include "TwitchSDKSettings.h"
#include "TwitchSDK.h"

using namespace R66;

UTwitchSDKSettings::UTwitchSDKSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
	CategoryName = TEXT("Plugins");
}

#if WITH_EDITOR
void UTwitchSDKSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	FTwitchSDKModule::Get().Core->UpdateOAuthCredentials(FromFString(ClientId));
	UDeveloperSettings::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
