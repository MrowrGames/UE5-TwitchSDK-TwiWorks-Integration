#include "ApiGetAuthenticationInfo.h"

void UApiGetAuthenticationInfo::Activate() {
    TWeakObjectPtr<UApiGetAuthenticationInfo> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKAuthenticationInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetAuthenticationInfo error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Scopes);
    try {
        FTwitchSDKModule::Get().Core->GetAuthenticationInfo(
            v,
            [weak](const R66::AuthenticationInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKAuthenticationInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiGetAuthenticationInfo* UApiGetAuthenticationInfo::GetAuthenticationInfo(FString scopes) {
    auto ptr = NewObject<UApiGetAuthenticationInfo>();
    ptr->Scopes = scopes;
    return ptr;
}
