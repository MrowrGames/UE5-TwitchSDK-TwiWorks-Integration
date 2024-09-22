// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TwitchSDKBPLibrary.generated.h"



UCLASS()
class UTwitchSDKBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Build OAuth Scopes", AdvancedDisplay=1, AutoCreateRefTerm = "CustomScopes"), Category = "Twitch")
	static FString BuildOAuthScopes(TArray<FTwitchSDKOAuthScope> Scopes, TArray<FString> CustomScopes);
};
