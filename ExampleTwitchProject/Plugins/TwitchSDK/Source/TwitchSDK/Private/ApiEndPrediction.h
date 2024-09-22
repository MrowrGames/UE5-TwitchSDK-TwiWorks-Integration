#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiEndPrediction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiEndPrediction, FTwitchSDKPredictionInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiEndPrediction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString BroadcasterId;
    FString PredictionId;
    FTwitchSDKPredictionStatus Status;
    FString WinningOutcomeId;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiEndPrediction Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiEndPrediction Error;

    virtual void Activate() override;

    /**
     * Lock, resolve, or cancel a Channel Points Prediction.
     *
     * Required scope: channel:manage:predictions
     *
     * @param broadcasterId The broadcaster running prediction events.
     * @param predictionId ID of the Prediction.
     * @param status The Prediction status to be set.
     * @param winningOutcomeId ID of the winning outcome for the Prediction, if EndPredictionRequest::Status is being set to PredictionStatus::Resolved.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiEndPrediction* EndPrediction(FString broadcasterId, FString predictionId, FTwitchSDKPredictionStatus status, FString winningOutcomeId);
};
