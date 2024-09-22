#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForHypeTrainEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForHypeTrainEvent, FTwitchSDKHypeTrainEvent, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForHypeTrainEvent : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForHypeTrainEvent Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForHypeTrainEvent Error;

    virtual void Activate() override;

    /**
     * A Hype Train makes progress on the user's channel. Requires the channel:read:hype_train scope.
     *
     * You may only call this with a subscription for the correct event type.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForHypeTrainEvent* WaitForHypeTrainEvent(FTwitchSDKEventStreamDesc desc);
};
