#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForChannelFollowEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForChannelFollowEvent, FTwitchSDKChannelFollowEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForChannelFollowEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelFollowEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelFollowEvent Error;

    virtual void Activate() override;

    /**
     * A specified channel receives a follow.
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForChannelFollowEvent* WaitForChannelFollowEvent(FTwitchSDKEventStreamDesc desc);
};
