#include "ApiCheckUserSubscription.h"

void UApiCheckUserSubscription::Activate() {
    TWeakObjectPtr<UApiCheckUserSubscription> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKUserSubscriptionCheckResult(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CheckUserSubscription error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Broadcaster);
    try {
        FTwitchSDKModule::Get().Core->CheckUserSubscription(
            v,
            [weak](const R66::UserSubscriptionCheckResult& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKUserSubscriptionCheckResult v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiCheckUserSubscription* UApiCheckUserSubscription::CheckUserSubscription(FString broadcaster) {
    auto ptr = NewObject<UApiCheckUserSubscription>();
    ptr->Broadcaster = broadcaster;
    return ptr;
}
