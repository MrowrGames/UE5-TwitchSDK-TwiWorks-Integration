#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiUnsubscribeFromPoll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiUnsubscribeFromPoll, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiUnsubscribeFromPoll : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Id;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiUnsubscribeFromPoll Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiUnsubscribeFromPoll Error;

    virtual void Activate() override;

    /**
     * Unsubscribe from updates for a specific poll.
     *
     * You must not invoke this function multiple times for the same poll. Unsubscribing from polls is optional. Failing to do so results in unnecessary traffic, but will not cause any malfunction or memory leak.
     *
     * @param id Id of the poll to unsubscribe from.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiUnsubscribeFromPoll* UnsubscribeFromPoll(FString id);
};
