#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiEndPoll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiEndPoll, FTwitchSDKPollInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiEndPoll : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString BroadcasterId;
    FString PollId;
    bool ShowResults;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiEndPoll Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiEndPoll Error;

    virtual void Activate() override;

    /**
     * End a poll prematurely.
     *
     * x
     *
     * @param broadcasterId ID of the broadcaster.
     * @param pollId ID of the poll.
     * @param showResults If true, sets the poll status to PollStatus::Terminated so that results are shown. If false, sets the poll status to PollStatus::Archived so that the poll is canceled immediately and no results are shown.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiEndPoll* EndPoll(FString broadcasterId, FString pollId, bool showResults);
};
