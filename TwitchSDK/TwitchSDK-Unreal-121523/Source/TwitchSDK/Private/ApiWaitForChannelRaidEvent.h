#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForChannelRaidEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForChannelRaidEvent, FTwitchSDKChannelRaidEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForChannelRaidEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelRaidEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelRaidEvent Error;

    virtual void Activate() override;

    /**
     * A broadcaster raids another broadcaster’s channel.
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForChannelRaidEvent* WaitForChannelRaidEvent(FTwitchSDKEventStreamDesc desc);
};
