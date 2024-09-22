#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiWaitForPredictionUpdate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiWaitForPredictionUpdate, FTwitchSDKPredictionInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiWaitForPredictionUpdate : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Prediction;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForPredictionUpdate Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiWaitForPredictionUpdate Error;

    virtual void Activate() override;

    /**
     * Wait for updates to a specified prediction. Updates occur when users submit votes, and finally, when the prediction ends.
     *
     * Avoid calling this function on predictions after they have ended, as they will no longer recieve updates at that point.
     *
     * @param prediction Id of the prediction to wait for.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiWaitForPredictionUpdate* WaitForPredictionUpdate(FString prediction);
};
