#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiResolveCustomReward.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiResolveCustomReward, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiResolveCustomReward : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString RedemptionId;
    FString CustomRewardId;
    FString BroadcasterId;
    FTwitchSDKCustomRewardRedemptionState Resolution;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiResolveCustomReward Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiResolveCustomReward Error;

    virtual void Activate() override;

    /**
     * Updates the status of Custom Reward Redemption objects on a channel that are in the CustomRewardRedemptionState::Unfulfilled status.
     *
     * Required scope: channel:manage:redemptions
     *
     * @param redemptionId ID of the Custom Reward Redemption to update.
     * @param customRewardId ID of the Custom Reward the redemptions to be updated are for.
     * @param resolution The new status to set redemptions to.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiResolveCustomReward* ResolveCustomReward(FString redemptionId, FString customRewardId, FString broadcasterId, FTwitchSDKCustomRewardRedemptionState resolution);
};
