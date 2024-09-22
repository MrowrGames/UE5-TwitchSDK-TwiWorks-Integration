#include "ApiWaitForChannelSubscribeEvent.h"

void UApiWaitForChannelSubscribeEvent::Activate() {
    TWeakObjectPtr<UApiWaitForChannelSubscribeEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKChannelSubscribeEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForChannelSubscribeEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForChannelSubscribeEvent(
            v,
            [weak](const R66::ChannelSubscribeEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKChannelSubscribeEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForChannelSubscribeEvent* UApiWaitForChannelSubscribeEvent::WaitForChannelSubscribeEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForChannelSubscribeEvent>();
    ptr->Desc = desc;
    return ptr;
}
