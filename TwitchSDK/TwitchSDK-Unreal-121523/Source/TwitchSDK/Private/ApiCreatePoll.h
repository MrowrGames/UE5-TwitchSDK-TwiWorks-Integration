#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCreatePoll.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiCreatePoll, FTwitchSDKPollInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCreatePoll : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Title;
    TArray<FString> Choices;
    int64 Duration;
    bool BitsVotingEnabled;
    int32 BitsPerVote;
    bool ChannelPointsVotingEnabled;
    int32 ChannelPointsPerVote;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreatePoll Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreatePoll Error;

    virtual void Activate() override;

    /**
     * Create a poll on the user's Twitch channel.
     *
     * Required scope: channel:manage:polls
     *
     * Creating a poll automatically subscribes to updates for this poll. Avoid unnecessary traffic by invoking UnsubscribeFromPoll at some point for every poll you create.
     *
     * @param title Question displayed for the poll.
     * @param choices Array of the poll choices.
     * @param duration Total duration for the poll (in seconds).
     * @param bitsVotingEnabled Indicates if Bits can be used for voting.
     * @param bitsPerVote Number of Bits required to vote once with Bits.
     * @param channelPointsVotingEnabled Indicates if Channel Points can be used for voting.
     * @param channelPointsPerVote Number of Channel Points required to vote once with Channel Points.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCreatePoll* CreatePoll(FString title, TArray<FString> choices, int64 duration, bool bitsVotingEnabled, int32 bitsPerVote, bool channelPointsVotingEnabled, int32 channelPointsPerVote);
};
