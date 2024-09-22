#include "ApiQueryStreams.h"

void UApiQueryStreams::Activate() {
    TWeakObjectPtr<UApiQueryStreams> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FTwitchSDKStreamQueryResult(), FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("QueryStreams error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::StreamQuery v;
    v.After = R66::FromFString(After);
    v.First = First;
    v.GameIds = R66::FromFStringArray(GameIds);
    v.Languages = R66::FromFStringArray(Languages);
    v.UserIds = R66::FromFStringArray(UserIds);
    v.UserLogins = R66::FromFStringArray(UserLogins);
    try {
        FTwitchSDKModule::Get().Core->QueryStreams(
            v,
            [weak](const R66::StreamQueryResult& r) {
                if (!weak.IsValid()) return;
                FTwitchSDKStreamQueryResult v(r);
                weak->Done.Broadcast(v, FString());
            },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiQueryStreams* UApiQueryStreams::QueryStreams(FString after, int32 first, TArray<FString> gameIds, TArray<FString> languages, TArray<FString> userIds, TArray<FString> userLogins) {
    auto ptr = NewObject<UApiQueryStreams>();
    ptr->After = after;
    ptr->First = first;
    ptr->GameIds = gameIds;
    ptr->Languages = languages;
    ptr->UserIds = userIds;
    ptr->UserLogins = userLogins;
    return ptr;
}
