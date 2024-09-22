#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForCustomRewardEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForCustomRewardEvent, FTwitchSDKCustomRewardEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForCustomRewardEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForCustomRewardEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForCustomRewardEvent Error;

    virtual void Activate() override;

    /**
     * A viewer has redeemed a custom channel points reward on the specified channel or the redemption has been updated (i.e., fulfilled or cancelled).
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForCustomRewardEvent* WaitForCustomRewardEvent(FTwitchSDKEventStreamDesc desc);
};
