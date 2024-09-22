#include "ApiWaitForChannelFollowEvent.h"

void UApiWaitForChannelFollowEvent::Activate() {
    TWeakObjectPtr<UApiWaitForChannelFollowEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKChannelFollowEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForChannelFollowEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForChannelFollowEvent(
            v,
            [weak](const R66::ChannelFollowEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKChannelFollowEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForChannelFollowEvent* UApiWaitForChannelFollowEvent::WaitForChannelFollowEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForChannelFollowEvent>();
    ptr->Desc = desc;
    return ptr;
}
