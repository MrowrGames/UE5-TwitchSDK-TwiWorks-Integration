#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiSubscribeToEventStream.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiSubscribeToEventStream, FTwitchSDKEventStreamDesc, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiSubscribeToEventStream : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamKind Kind;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiSubscribeToEventStream Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiSubscribeToEventStream Error;

    virtual void Activate() override;

    /**
     * Subscribe to a Twitch EventSub stream.
     *
     * You must periodically receive events from your event stream. You must call CloseEventStream once you are done.
     *
     * To avoid leaking memory indefinitely in this case, there is an internal limit of 512 buffered events per event stream subscription, as well as an internal limit of 64 subscription per event type in total. When the internal buffer of a subscription is full, incoming events will be discarded silently. When the subscriptions limit is reached, an arbitrary subscription of the same type will be closed, and a warning message will be logged.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiSubscribeToEventStream* SubscribeToEventStream(FTwitchSDKEventStreamKind kind);
};
