#include "ApiWaitForPollUpdate.h"

void UApiWaitForPollUpdate::Activate() {
    TWeakObjectPtr<UApiWaitForPollUpdate> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPollInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForPollUpdate error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Poll);
    try {
        FTwitchSDKModule::Get().Core->WaitForPollUpdate(
            v,
            [weak](const R66::PollInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKPollInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForPollUpdate* UApiWaitForPollUpdate::WaitForPollUpdate(FString poll) {
    auto ptr = NewObject<UApiWaitForPollUpdate>();
    ptr->Poll = poll;
    return ptr;
}
