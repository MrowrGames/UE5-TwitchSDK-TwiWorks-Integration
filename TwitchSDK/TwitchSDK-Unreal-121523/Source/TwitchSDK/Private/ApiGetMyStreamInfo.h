#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetMyStreamInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetMyStreamInfo, FTwitchSDKStreamInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetMyStreamInfo : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetMyStreamInfo Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetMyStreamInfo Error;

    virtual void Activate() override;

    /**
     * Gets information about the currently logged-in user's stream.
     *
     * If the user is not logged in, this also waits for a successful login. Returns null if the user is currently not live.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetMyStreamInfo* GetMyStreamInfo();
};
