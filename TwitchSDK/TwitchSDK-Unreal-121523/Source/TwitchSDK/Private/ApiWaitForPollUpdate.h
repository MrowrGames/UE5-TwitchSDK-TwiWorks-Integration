#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForPollUpdate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForPollUpdate, FTwitchSDKPollInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForPollUpdate : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Poll;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForPollUpdate Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForPollUpdate Error;

    virtual void Activate() override;

    /**
     * Wait for updates to a specified poll. Updates occur when users submit votes, and finally, when the poll ends.
     *
     * Avoid calling this function on polls after they have ended, as they will no longer recieve updates at that point.
     *
     * @param poll Id of the poll to wait for.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForPollUpdate* WaitForPollUpdate(FString poll);
};
