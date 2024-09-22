#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetUserInfoById.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetUserInfoById, FTwitchSDKUserInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetUserInfoById : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Id;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetUserInfoById Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetUserInfoById Error;

    virtual void Activate() override;

    /**
     * Gets information about a specific Twitch user account.
     *
     * Returns an empty object if no such user could be found.
     *
     * @param id The account ID to query for.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetUserInfoById* GetUserInfoById(FString id);
};
