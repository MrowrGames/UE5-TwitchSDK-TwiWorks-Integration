#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiLogOut.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiLogOut, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiLogOut : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiLogOut Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiLogOut Error;

    virtual void Activate() override;

    /**
     * Log out.
     *
     * This call always returns immediately.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiLogOut* LogOut();
};
