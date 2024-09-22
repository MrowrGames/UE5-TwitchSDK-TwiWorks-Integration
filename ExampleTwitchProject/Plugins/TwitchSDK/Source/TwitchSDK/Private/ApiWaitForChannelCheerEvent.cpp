#include "ApiWaitForChannelCheerEvent.h"

void UApiWaitForChannelCheerEvent::Activate() {
    TWeakObjectPtr<UApiWaitForChannelCheerEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKChannelCheerEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForChannelCheerEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForChannelCheerEvent(
            v,
            [weak](const R66::ChannelCheerEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKChannelCheerEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForChannelCheerEvent* UApiWaitForChannelCheerEvent::WaitForChannelCheerEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForChannelCheerEvent>();
    ptr->Desc = desc;
    return ptr;
}
