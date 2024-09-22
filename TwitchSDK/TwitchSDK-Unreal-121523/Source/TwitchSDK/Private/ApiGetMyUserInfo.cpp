#include "ApiGetMyUserInfo.h"

void UApiGetMyUserInfo::Activate() {
    TWeakObjectPtr<UApiGetMyUserInfo> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKUserInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetMyUserInfo error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    try {
        FTwitchSDKModule::Get().Core->GetMyUserInfo(
            [weak](const R66::UserInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKUserInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiGetMyUserInfo* UApiGetMyUserInfo::GetMyUserInfo() {
    auto ptr = NewObject<UApiGetMyUserInfo>();
    return ptr;
}
