#include "ApiSubscribeToEventStream.h"

void UApiSubscribeToEventStream::Activate() {
    TWeakObjectPtr<UApiSubscribeToEventStream> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKEventStreamDesc(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("SubscribeToEventStream error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::EventStreamRequest v;
    v.Kind = R66::EventStreamKind(Kind);
    try {
        FTwitchSDKModule::Get().Core->SubscribeToEventStream(
            v,
            [weak](const R66::EventStreamDesc& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKEventStreamDesc v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiSubscribeToEventStream* UApiSubscribeToEventStream::SubscribeToEventStream(FTwitchSDKEventStreamKind kind) {
    auto ptr = NewObject<UApiSubscribeToEventStream>();
    ptr->Kind = kind;
    return ptr;
}
