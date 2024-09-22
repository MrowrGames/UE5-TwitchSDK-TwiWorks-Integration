#include "ApiGetUserInfoById.h"

void UApiGetUserInfoById::Activate() {
    TWeakObjectPtr<UApiGetUserInfoById> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKUserInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetUserInfoById error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Id);
    try {
        FTwitchSDKModule::Get().Core->GetUserInfoById(
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

UApiGetUserInfoById* UApiGetUserInfoById::GetUserInfoById(FString id) {
    auto ptr = NewObject<UApiGetUserInfoById>();
    ptr->Id = id;
    return ptr;
}
