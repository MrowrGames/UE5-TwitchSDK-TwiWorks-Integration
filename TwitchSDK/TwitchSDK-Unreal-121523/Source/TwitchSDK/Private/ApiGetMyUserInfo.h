#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetMyUserInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetMyUserInfo, FTwitchSDKUserInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetMyUserInfo : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()


public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetMyUserInfo Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetMyUserInfo Error;

    virtual void Activate() override;

    /**
     * Gets information about the currently logged-in user's account.
     *
     * If the user is not logged in, this also waits for a successful login.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetMyUserInfo* GetMyUserInfo();
};
