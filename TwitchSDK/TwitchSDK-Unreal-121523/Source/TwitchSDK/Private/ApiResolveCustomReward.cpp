#include "ApiResolveCustomReward.h"

void UApiResolveCustomReward::Activate() {
    TWeakObjectPtr<UApiResolveCustomReward> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("ResolveCustomReward error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::CustomRewardResolveRequest v;
    v.RedemptionId = R66::FromFString(RedemptionId);
    v.CustomRewardId = R66::FromFString(CustomRewardId);
    v.BroadcasterId = R66::FromFString(BroadcasterId);
    v.Resolution = R66::CustomRewardRedemptionState(Resolution);
    try {
        FTwitchSDKModule::Get().Core->ResolveCustomReward(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiResolveCustomReward* UApiResolveCustomReward::ResolveCustomReward(FString redemptionId, FString customRewardId, FString broadcasterId, FTwitchSDKCustomRewardRedemptionState resolution) {
    auto ptr = NewObject<UApiResolveCustomReward>();
    ptr->RedemptionId = redemptionId;
    ptr->CustomRewardId = customRewardId;
    ptr->BroadcasterId = broadcasterId;
    ptr->Resolution = resolution;
    return ptr;
}
