#include "ApiUnsubscribeFromPoll.h"

void UApiUnsubscribeFromPoll::Activate() {
    TWeakObjectPtr<UApiUnsubscribeFromPoll> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("UnsubscribeFromPoll error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Id);
    try {
        FTwitchSDKModule::Get().Core->UnsubscribeFromPoll(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiUnsubscribeFromPoll* UApiUnsubscribeFromPoll::UnsubscribeFromPoll(FString id) {
    auto ptr = NewObject<UApiUnsubscribeFromPoll>();
    ptr->Id = id;
    return ptr;
}
