#include "ApiWaitForPredictionUpdate.h"

void UApiWaitForPredictionUpdate::Activate() {
    TWeakObjectPtr<UApiWaitForPredictionUpdate> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPredictionInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("WaitForPredictionUpdate error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    auto v = R66::FromFString(Prediction);
    try {
        FTwitchSDKModule::Get().Core->WaitForPredictionUpdate(
            v,
            [weak](const R66::PredictionInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKPredictionInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiWaitForPredictionUpdate* UApiWaitForPredictionUpdate::WaitForPredictionUpdate(FString prediction) {
    auto ptr = NewObject<UApiWaitForPredictionUpdate>();
    ptr->Prediction = prediction;
    return ptr;
}
