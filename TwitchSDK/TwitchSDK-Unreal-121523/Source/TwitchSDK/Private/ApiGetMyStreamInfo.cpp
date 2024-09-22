#include "ApiGetMyStreamInfo.h"

void UApiGetMyStreamInfo::Activate() {
    TWeakObjectPtr<UApiGetMyStreamInfo> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKStreamInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetMyStreamInfo error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    try {
        FTwitchSDKModule::Get().Core->GetMyStreamInfo(
            [weak](const R66::StreamInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKStreamInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiGetMyStreamInfo* UApiGetMyStreamInfo::GetMyStreamInfo() {
    auto ptr = NewObject<UApiGetMyStreamInfo>();
    return ptr;
}
