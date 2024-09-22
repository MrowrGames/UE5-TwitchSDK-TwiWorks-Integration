#include "ApiWaitForCustomRewardEvent.h"

void UApiWaitForCustomRewardEvent::Activate() {
    TWeakObjectPtr<UApiWaitForCustomRewardEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKCustomRewardEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForCustomRewardEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForCustomRewardEvent(
            v,
            [weak](const R66::CustomRewardEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKCustomRewardEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForCustomRewardEvent* UApiWaitForCustomRewardEvent::WaitForCustomRewardEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForCustomRewardEvent>();
    ptr->Desc = desc;
    return ptr;
}
