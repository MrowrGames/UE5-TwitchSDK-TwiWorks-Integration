#include "ApiLogOut.h"

void UApiLogOut::Activate() {
    TWeakObjectPtr<UApiLogOut> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("LogOut error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    try {
        FTwitchSDKModule::Get().Core->LogOut(
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiLogOut* UApiLogOut::LogOut() {
    auto ptr = NewObject<UApiLogOut>();
    return ptr;
}
