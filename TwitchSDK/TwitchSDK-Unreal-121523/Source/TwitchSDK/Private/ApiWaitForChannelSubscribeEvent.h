#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForChannelSubscribeEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForChannelSubscribeEvent, FTwitchSDKChannelSubscribeEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForChannelSubscribeEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelSubscribeEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelSubscribeEvent Error;

    virtual void Activate() override;

    /**
     * A notification when a specified channel receives a subscriber. This does not include resubscribes.
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForChannelSubscribeEvent* WaitForChannelSubscribeEvent(FTwitchSDKEventStreamDesc desc);
};
