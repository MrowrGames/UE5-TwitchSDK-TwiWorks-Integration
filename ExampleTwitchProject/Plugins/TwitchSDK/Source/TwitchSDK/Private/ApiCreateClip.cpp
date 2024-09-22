#include "ApiCreateClip.h"

void UApiCreateClip::Activate() {
    TWeakObjectPtr<UApiCreateClip> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKClipInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CreateClip error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = HasDelay;
    try {
        FTwitchSDKModule::Get().Core->CreateClip(
            v,
            [weak](const R66::ClipInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKClipInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiCreateClip* UApiCreateClip::CreateClip(bool hasDelay) {
    auto ptr = NewObject<UApiCreateClip>();
    ptr->HasDelay = hasDelay;
    return ptr;
}
