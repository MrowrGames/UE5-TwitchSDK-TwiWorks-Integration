#include "ApiModifyChannelInformation.h"

void UApiModifyChannelInformation::Activate() {
    TWeakObjectPtr<UApiModifyChannelInformation> weak(this);
    auto exception_handler = [weak](const std::exception& e) {
        if (weak.IsValid() && weak->Error.IsBound())
            weak->Error.Broadcast(FString(e.what()));
        else
            UE_LOG(LogTwitchSDK, Error, TEXT("ModifyChannelInformation error: %s"), UTF8_TO_TCHAR(e.what()));
    };
    R66::ModifyChannelInfoRequest v;
    v.GameId = R66::FromFString(GameId);
    v.Language = R66::FromFString(Language);
    v.Title = R66::FromFString(Title);
    v.Delay = Delay;
    v.Tags = R66::FromFStringArray(Tags);
    v.ForceUpdateTags = ForceUpdateTags;
    try {
        FTwitchSDKModule::Get().Core->ModifyChannelInformation(
            v,
            [weak]() { if (weak.IsValid()) weak->Done.Broadcast(FString()); },
            exception_handler
        );
    } catch (const std::exception& e) {
        exception_handler(e);
    }
}

UApiModifyChannelInformation* UApiModifyChannelInformation::ModifyChannelInformation(FString gameId, FString language, FString title, int32 delay, TArray<FString> tags, bool forceUpdateTags) {
    auto ptr = NewObject<UApiModifyChannelInformation>();
    ptr->GameId = gameId;
    ptr->Language = language;
    ptr->Title = title;
    ptr->Delay = delay;
    ptr->Tags = tags;
    ptr->ForceUpdateTags = forceUpdateTags;
    return ptr;
}
