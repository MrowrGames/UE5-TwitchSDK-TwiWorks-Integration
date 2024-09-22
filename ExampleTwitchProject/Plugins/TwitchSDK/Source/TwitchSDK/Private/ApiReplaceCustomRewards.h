#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiReplaceCustomRewards.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiReplaceCustomRewards, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiReplaceCustomRewards : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FTwitchSDKCustomRewardList Req;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiReplaceCustomRewards Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiReplaceCustomRewards Error;

    virtual void Activate() override;

    /**
     * Updates the set of available custom channel points rewards on a channel.
     *
     * Required scope: channel:manage:redemptions
     *
     * If you enable any custom rewards, please clear them by invoking this function with an empty rewards list when the game terminates.
     *
     * Note that removing rewards that still have pending redemptions will set those redemptions to CustomRewardRedemptionState::Fulfilled.
     *
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiReplaceCustomRewards* ReplaceCustomRewards(FTwitchSDKCustomRewardList req);
};
