#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetAuthTokens.generated.h"

// For TwitchSDK --> TwiWorks

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetAuthTokens, FString, AccessToken, FString, RefreshToken);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetAuthTokens : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAsyncDoneApiGetAuthTokens Done;
	UPROPERTY(BlueprintAssignable)
	FAsyncDoneApiGetAuthTokens Error;

	virtual void Activate() override;

	/**
	 * Get the user's access and refresh tokens.
	 *
	 * This call always returns immediately.
	 *
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
	static UApiGetAuthTokens* GetAuthTokens();
};
