#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCheckUserSubscription.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiCheckUserSubscription, FTwitchSDKUserSubscriptionCheckResult, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCheckUserSubscription : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Broadcaster;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCheckUserSubscription Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCheckUserSubscription Error;

    virtual void Activate() override;

    /**
     * Checks if the currently logged-in user is subscribed to a specific channel.
     *
     * Required scope: user:read:subscriptions
     *
     * @param broadcaster ID of the broadcaster to check for.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCheckUserSubscription* CheckUserSubscription(FString broadcaster);
};
