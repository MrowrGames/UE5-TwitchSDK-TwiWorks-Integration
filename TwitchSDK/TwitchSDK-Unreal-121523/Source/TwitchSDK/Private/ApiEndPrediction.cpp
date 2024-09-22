#include "ApiEndPrediction.h"

void UApiEndPrediction::Activate() {
    TWeakObjectPtr<UApiEndPrediction> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPredictionInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("EndPrediction error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::EndPredictionRequest v;
    v.BroadcasterId = R66::FromFString(BroadcasterId);
    v.PredictionId = R66::FromFString(PredictionId);
    v.Status = R66::PredictionStatus(Status);
    v.WinningOutcomeId = R66::FromFString(WinningOutcomeId);
    try {
        FTwitchSDKModule::Get().Core->EndPrediction(
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

UApiEndPrediction* UApiEndPrediction::EndPrediction(FString broadcasterId, FString predictionId, FTwitchSDKPredictionStatus status, FString winningOutcomeId) {
    auto ptr = NewObject<UApiEndPrediction>();
    ptr->BroadcasterId = broadcasterId;
    ptr->PredictionId = predictionId;
    ptr->Status = status;
    ptr->WinningOutcomeId = winningOutcomeId;
    return ptr;
}
