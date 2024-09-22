// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TwitchSDKSettings.generated.h"

/**
 * Create your application at https://dev.twitch.tv/ and enter the OAuth credentials below.
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Twitch"))
class TWITCHSDK_API UTwitchSDKSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere, config, Category = Authentication)
	FString ClientId;

	/**
	 * Do not enable this in releases. This option instructs the plugin to connect to a local EventSubProxy instead of directly to Twitch.
	 */
	UPROPERTY(EditAnywhere, config, Category = Debugging, meta = (ConfigRestartRequired = true))
	bool UseEventSubProxy;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
