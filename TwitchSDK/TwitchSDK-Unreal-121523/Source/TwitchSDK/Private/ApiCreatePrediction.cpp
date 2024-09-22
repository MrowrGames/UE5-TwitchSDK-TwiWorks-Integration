#include "ApiCreatePrediction.h"

void UApiCreatePrediction::Activate() {
    TWeakObjectPtr<UApiCreatePrediction> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPredictionInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CreatePrediction error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::PredictionDefinition v;
    v.Title = R66::FromFString(Title);
    v.Outcomes = R66::FromFStringArray(Outcomes);
    v.Duration = Duration;
    try {
        FTwitchSDKModule::Get().Core->CreatePrediction(
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

UApiCreatePrediction* UApiCreatePrediction::CreatePrediction(FString title, TArray<FString> outcomes, int32 duration) {
    auto ptr = NewObject<UApiCreatePrediction>();
    ptr->Title = title;
    ptr->Outcomes = outcomes;
    ptr->Duration = duration;
    return ptr;
}
