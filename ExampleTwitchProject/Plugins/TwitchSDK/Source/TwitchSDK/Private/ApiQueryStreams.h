#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiQueryStreams.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiQueryStreams, FTwitchSDKStreamQueryResult, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiQueryStreams : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString After;
    int32 First;
    TArray<FString> GameIds;
    TArray<FString> Languages;
    TArray<FString> UserIds;
    TArray<FString> UserLogins;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiQueryStreams Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiQueryStreams Error;

    virtual void Activate() override;

    /**
     * Gets information about active streams. Streams are returned sorted by number of current viewers, in descending order.
     *
     * Submitting an empty query is valid and fetches the top streams currently on Twitch. Across multiple pages of results, there may be duplicate or missing streams, as viewers join and leave streams.
     *
     * @param after Cursor for forward pagination: tells the server where to start fetching the next set of results, in a multi-page response. The cursor value specified here is from the PaginationCursor response field of a prior query.
     * @param first Maximum number of objects to return. Maximum: 100. Default: 20.
     * @param gameIds Returns streams broadcasting a specified game ID. You can specify up to 100 IDs.
     * @param languages Stream language. You can specify up to 100 languages. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”.
     * @param userIds Returns streams broadcast by one or more specified user IDs. You can specify up to 100 IDs.
     * @param userLogins Returns streams broadcast by one or more specified user login names. You can specify up to 100 names.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiQueryStreams* QueryStreams(FString after, int32 first, TArray<FString> gameIds, TArray<FString> languages, TArray<FString> userIds, TArray<FString> userLogins);
};
