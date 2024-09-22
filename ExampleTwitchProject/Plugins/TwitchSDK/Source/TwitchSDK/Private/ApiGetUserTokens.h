#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetUserTokens.generated.h"

// For TwitchSDK --> TwiWorks

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetUserTokens, FString, AccessToken, FString, RefreshToken);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetUserTokens : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAsyncDoneApiGetUserTokens Done;
	UPROPERTY(BlueprintAssignable)
	FAsyncDoneApiGetUserTokens Error;

	virtual void Activate() override;

	/**
	 * Get the user's access and refresh tokens.
	 *
	 * This call always returns immediately.
	 *
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
	static UApiGetUserTokens* GetUserTokens();
};
