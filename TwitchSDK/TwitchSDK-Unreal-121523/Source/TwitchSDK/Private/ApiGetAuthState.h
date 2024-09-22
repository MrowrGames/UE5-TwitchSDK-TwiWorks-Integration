#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetAuthState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetAuthState, FTwitchSDKAuthState, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetAuthState : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetAuthState Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetAuthState Error;

    virtual void Activate() override;

    /**
     * Query the current authentication state.
     *
     * This call always returns immediately.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetAuthState* GetAuthState();
};
