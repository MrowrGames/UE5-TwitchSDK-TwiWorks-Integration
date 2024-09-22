#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiUnsubscribeFromPrediction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiUnsubscribeFromPrediction, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiUnsubscribeFromPrediction : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Id;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiUnsubscribeFromPrediction Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiUnsubscribeFromPrediction Error;

    virtual void Activate() override;

    /**
     * Unsubscribe from updates for a specific prediction.
     *
     * You must not invoke this function multiple times for the same prediction. Unsubscribing from predictions is optional. Failing to do so results in unnecessary traffic, but will not cause any malfunction or memory leak.
     *
     * @param id Id of the prediction to unsubscribe from.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiUnsubscribeFromPrediction* UnsubscribeFromPrediction(FString id);
};
