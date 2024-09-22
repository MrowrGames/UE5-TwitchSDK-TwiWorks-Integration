#include "ApiGetAuthState.h"

void UApiGetAuthState::Activate() {
    TWeakObjectPtr<UApiGetAuthState> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKAuthState(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetAuthState error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    try {
        FTwitchSDKModule::Get().Core->GetAuthState(
            [weak](const R66::AuthState& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKAuthState v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiGetAuthState* UApiGetAuthState::GetAuthState() {
    auto ptr = NewObject<UApiGetAuthState>();
    return ptr;
}
