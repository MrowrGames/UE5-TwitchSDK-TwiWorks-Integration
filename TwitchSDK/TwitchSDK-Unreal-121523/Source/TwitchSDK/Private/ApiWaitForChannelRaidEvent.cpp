#include "ApiWaitForChannelRaidEvent.h"

void UApiWaitForChannelRaidEvent::Activate() {
    TWeakObjectPtr<UApiWaitForChannelRaidEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKChannelRaidEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForChannelRaidEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForChannelRaidEvent(
            v,
            [weak](const R66::ChannelRaidEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKChannelRaidEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForChannelRaidEvent* UApiWaitForChannelRaidEvent::WaitForChannelRaidEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForChannelRaidEvent>();
    ptr->Desc = desc;
    return ptr;
}
