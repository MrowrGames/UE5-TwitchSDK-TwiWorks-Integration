#include "ApiUnsubscribeFromPrediction.h"

void UApiUnsubscribeFromPrediction::Activate() {
    TWeakObjectPtr<UApiUnsubscribeFromPrediction> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("UnsubscribeFromPrediction error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Id);
    try {
        FTwitchSDKModule::Get().Core->UnsubscribeFromPrediction(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiUnsubscribeFromPrediction* UApiUnsubscribeFromPrediction::UnsubscribeFromPrediction(FString id) {
    auto ptr = NewObject<UApiUnsubscribeFromPrediction>();
    ptr->Id = id;
    return ptr;
}
