#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCreatePrediction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiCreatePrediction, FTwitchSDKPredictionInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCreatePrediction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Title;
    TArray<FString> Outcomes;
    int32 Duration;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreatePrediction Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreatePrediction Error;

    virtual void Activate() override;

    /**
     * Create a Channel Points Prediction on the user's Twitch channel.
     *
     * Required scope: channel:manage:predictions
     *
     * Creating a prediction automatically subscribes to updates for this prediction. Avoid unnecessary traffic by invoking UnsubscribeFromPrediction at some point for every poll you create.
     *
     * @param title Title for the Prediction.
     * @param outcomes Array of outcome titles for the Prediction. Array size must be 2. The first outcome object is the “blue” outcome and the second outcome object is the “pink” outcome when viewing the Prediction on Twitch.
     * @param duration Total duration for the Prediction (in seconds). Minimum: 1. Maximum: 1800.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCreatePrediction* CreatePrediction(FString title, TArray<FString> outcomes, int32 duration);
};
