#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetUserInfoByLoginName.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetUserInfoByLoginName, FTwitchSDKUserInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetUserInfoByLoginName : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Login;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetUserInfoByLoginName Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetUserInfoByLoginName Error;

    virtual void Activate() override;

    /**
     * Gets information about a specific Twitch user account.
     *
     * Returns an empty object if no such user could be found.
     *
     * @param login The login name to query for.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetUserInfoByLoginName* GetUserInfoByLoginName(FString login);
};
