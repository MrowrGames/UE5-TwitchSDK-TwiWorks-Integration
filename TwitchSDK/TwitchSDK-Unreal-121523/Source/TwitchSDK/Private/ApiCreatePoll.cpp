#include "ApiCreatePoll.h"

void UApiCreatePoll::Activate() {
    TWeakObjectPtr<UApiCreatePoll> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKPollInfo(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("CreatePoll error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::PollDefinition v;
    v.Title = R66::FromFString(Title);
    v.Choices = R66::FromFStringArray(Choices);
    v.Duration = Duration;
    v.BitsVotingEnabled = BitsVotingEnabled;
    v.BitsPerVote = BitsPerVote;
    v.ChannelPointsVotingEnabled = ChannelPointsVotingEnabled;
    v.ChannelPointsPerVote = ChannelPointsPerVote;
    try {
        FTwitchSDKModule::Get().Core->CreatePoll(
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

UApiCreatePoll* UApiCreatePoll::CreatePoll(FString title, TArray<FString> choices, int64 duration, bool bitsVotingEnabled, int32 bitsPerVote, bool channelPointsVotingEnabled, int32 channelPointsPerVote) {
    auto ptr = NewObject<UApiCreatePoll>();
    ptr->Title = title;
    ptr->Choices = choices;
    ptr->Duration = duration;
    ptr->BitsVotingEnabled = bitsVotingEnabled;
    ptr->BitsPerVote = bitsPerVote;
    ptr->ChannelPointsVotingEnabled = channelPointsVotingEnabled;
    ptr->ChannelPointsPerVote = channelPointsPerVote;
    return ptr;
}
