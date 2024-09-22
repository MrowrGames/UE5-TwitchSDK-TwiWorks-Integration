#include "ApiGetBitsLeaderboard.h"

void UApiGetBitsLeaderboard::Activate() {
    TWeakObjectPtr<UApiGetBitsLeaderboard> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKBitsLeaderboard(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("GetBitsLeaderboard error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::BitsLeaderboardRequest v;
    v.Count = Count;
    v.Period = R66::FromFString(Period);
    v.StartedAt = R66::FromFString(StartedAt);
    v.UserId = R66::FromFString(UserId);
    try {
        FTwitchSDKModule::Get().Core->GetBitsLeaderboard(
            v,
            [weak](const R66::BitsLeaderboard& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKBitsLeaderboard v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiGetBitsLeaderboard* UApiGetBitsLeaderboard::GetBitsLeaderboard(int32 count, FString period, FString startedAt, FString userId) {
    auto ptr = NewObject<UApiGetBitsLeaderboard>();
    ptr->Count = count;
    ptr->Period = period;
    ptr->StartedAt = startedAt;
    ptr->UserId = userId;
    return ptr;
}
