#include "ApiReplaceCustomRewards.h"

void UApiReplaceCustomRewards::Activate() {
    TWeakObjectPtr<UApiReplaceCustomRewards> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("ReplaceCustomRewards error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Req;
    try {
        FTwitchSDKModule::Get().Core->ReplaceCustomRewards(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiReplaceCustomRewards* UApiReplaceCustomRewards::ReplaceCustomRewards(FTwitchSDKCustomRewardList req) {
    auto ptr = NewObject<UApiReplaceCustomRewards>();
    ptr->Req = req;
    return ptr;
}
