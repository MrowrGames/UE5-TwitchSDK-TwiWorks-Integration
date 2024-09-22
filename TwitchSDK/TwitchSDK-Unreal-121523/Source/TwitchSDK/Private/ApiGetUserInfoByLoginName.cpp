#include "ApiGetUserInfoByLoginName.h"

void UApiGetUserInfoByLoginName::Activate() {
    TWeakObjectPtr<UApiGetUserInfoByLoginName> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKUserInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetUserInfoByLoginName error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Login);
    try {
        FTwitchSDKModule::Get().Core->GetUserInfoByLoginName(
            v,
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

UApiGetUserInfoByLoginName* UApiGetUserInfoByLoginName::GetUserInfoByLoginName(FString login) {
    auto ptr = NewObject<UApiGetUserInfoByLoginName>();
    ptr->Login = login;
    return ptr;
}
