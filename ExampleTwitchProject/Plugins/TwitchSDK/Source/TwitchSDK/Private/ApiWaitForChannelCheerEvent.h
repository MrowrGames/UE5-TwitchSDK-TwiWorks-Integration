#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForChannelCheerEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForChannelCheerEvent, FTwitchSDKChannelCheerEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForChannelCheerEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelCheerEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForChannelCheerEvent Error;

    virtual void Activate() override;

    /**
     * A user cheers on the specified channel.
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForChannelCheerEvent* WaitForChannelCheerEvent(FTwitchSDKEventStreamDesc desc);
};
