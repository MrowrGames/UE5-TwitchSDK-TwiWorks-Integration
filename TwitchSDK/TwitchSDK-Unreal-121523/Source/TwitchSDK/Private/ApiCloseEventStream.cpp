#include "ApiCloseEventStream.h"

void UApiCloseEventStream::Activate() {
    TWeakObjectPtr<UApiCloseEventStream> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CloseEventStream error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = Desc;
    try {
        FTwitchSDKModule::Get().Core->CloseEventStream(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiCloseEventStream* UApiCloseEventStream::CloseEventStream(FTwitchSDKEventStreamDesc desc) {
    auto ptr = NewObject<UApiCloseEventStream>();
    ptr->Desc = desc;
    return ptr;
}
