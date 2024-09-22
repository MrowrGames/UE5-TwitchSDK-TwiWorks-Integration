#include "ApiPrepareShutdown.h"

void UApiPrepareShutdown::Activate() {
    TWeakObjectPtr<UApiPrepareShutdown> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("PrepareShutdown error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    try {
        FTwitchSDKModule::Get().Core->PrepareShutdown(
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiPrepareShutdown* UApiPrepareShutdown::PrepareShutdown() {
    auto ptr = NewObject<UApiPrepareShutdown>();
    return ptr;
}
