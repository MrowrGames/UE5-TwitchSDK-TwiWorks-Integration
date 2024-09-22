#include "ApiCreateStreamMarker.h"

void UApiCreateStreamMarker::Activate() {
    TWeakObjectPtr<UApiCreateStreamMarker> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKStreamMarkerInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CreateStreamMarker error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Description);
    try {
        FTwitchSDKModule::Get().Core->CreateStreamMarker(
            v,
            [weak](const R66::StreamMarkerInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKStreamMarkerInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiCreateStreamMarker* UApiCreateStreamMarker::CreateStreamMarker(FString description) {
    auto ptr = NewObject<UApiCreateStreamMarker>();
    ptr->Description = description;
    return ptr;
}
