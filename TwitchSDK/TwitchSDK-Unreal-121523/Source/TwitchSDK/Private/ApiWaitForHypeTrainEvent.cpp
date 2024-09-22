#include "ApiWaitForHypeTrainEvent.h"

void UApiWaitForHypeTrainEvent::Activate() {
    TWeakObjectPtr<UApiWaitForHypeTrainEvent> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKHypeTrainEvent(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForHypeTrainEvent error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->WaitForHypeTrainEvent(
            v,
            [weak](const R66::HypeTrainEvent& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKHypeTrainEvent v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForHypeTrainEvent* UApiWaitForHypeTrainEvent::WaitForHypeTrainEvent(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiWaitForHypeTrainEvent>();
    ptr->Desc = desc;
    return ptr;
}
