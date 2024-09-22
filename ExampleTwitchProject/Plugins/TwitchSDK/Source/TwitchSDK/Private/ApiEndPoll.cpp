#include "ApiEndPoll.h"

void UApiEndPoll::Activate() {
    TWeakObjectPtr<UApiEndPoll> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPollInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("EndPoll error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::EndPollRequest v;
    v.BroadcasterId = R66::FromFString(BroadcasterId);
    v.PollId = R66::FromFString(PollId);
    v.ShowResults = ShowResults;
    try {
        FTwitchSDKModule::Get().Core->EndPoll(
            v,
            [weak](const R66::PollInfo& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKPollInfo v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiEndPoll* UApiEndPoll::EndPoll(FString broadcasterId, FString pollId, bool showResults) {
    auto ptr = NewObject<UApiEndPoll>();
    ptr->BroadcasterId = broadcasterId;
    ptr->PollId = pollId;
    ptr->ShowResults = showResults;
    return ptr;
}
