#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiPrepareShutdown.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiPrepareShutdown, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiPrepareShutdown : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiPrepareShutdown Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiPrepareShutdown Error;

    virtual void Activate() override;

    /**
     * Prepare for shutdown.
     *
     * You should invoke this function and wait for it to return before exiting from your game process. This function resets state associated with your session such as available channel points rewards.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiPrepareShutdown* PrepareShutdown();
};
