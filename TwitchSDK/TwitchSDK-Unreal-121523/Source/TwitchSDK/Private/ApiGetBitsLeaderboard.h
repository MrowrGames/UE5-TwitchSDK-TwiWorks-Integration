#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetBitsLeaderboard.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetBitsLeaderboard, FTwitchSDKBitsLeaderboard, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetBitsLeaderboard : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    int32 Count;
    FString Period;
    FString StartedAt;
    FString UserId;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetBitsLeaderboard Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetBitsLeaderboard Error;

    virtual void Activate() override;

    /**
     * Gets a ranked list of Bits leaderboard information for an authorized broadcaster.
     *
     * Required scope: bits:read
     *
     * @param count Number of results to be returned. Maximum: 100. Default: 10.
     * @param period Time period over which data is aggregated (PST time zone). This parameter interacts with BitsLeaderboardRequest::StartedAt. Default: all.
     * @param startedAt Timestamp for the period over which the returned data is aggregated. Must be in RFC 3339 format.
     * @param userId ID of the user whose results are returned; i.e., the person who paid for the Bits.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetBitsLeaderboard* GetBitsLeaderboard(int32 count, FString period, FString startedAt, FString userId);
};
