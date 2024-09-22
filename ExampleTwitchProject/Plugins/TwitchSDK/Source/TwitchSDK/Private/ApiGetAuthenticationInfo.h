#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiGetAuthenticationInfo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiGetAuthenticationInfo, FTwitchSDKAuthenticationInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiGetAuthenticationInfo : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Scopes;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetAuthenticationInfo Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiGetAuthenticationInfo Error;

    virtual void Activate() override;

    /**
     * Start the authorization code flow. If a user is currently logged in, this does nothing. After a successfull call to GetAuthenticationInfo, subsequent calls will return the same AuthenticationInfo until the login is either completed or timed out.
     *
     * Credentials are stored persistently, so after loggin in successfully and restarting your application, the library remains in the LoggedIn state.
     *
     * @param scopes The set of OAuth scopes you would like to request.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiGetAuthenticationInfo* GetAuthenticationInfo(FString scopes);
};
