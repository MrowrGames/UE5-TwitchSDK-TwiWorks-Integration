#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCloseEventStream.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiCloseEventStream, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCloseEventStream : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKEventStreamDesc Desc;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCloseEventStream Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCloseEventStream Error;

    virtual void Activate() override;

    /**
     * Close a Twitch EventSub subscription.
     *
     * After the subscription is closed, trying to receive events from it is an error.
     *
     * @param desc An object describing the subscription.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCloseEventStream* CloseEventStream(FTwitchSDKEventStreamDesc desc);
};
