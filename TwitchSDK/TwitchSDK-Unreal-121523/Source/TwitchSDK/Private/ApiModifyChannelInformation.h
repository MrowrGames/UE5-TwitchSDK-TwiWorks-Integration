#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiModifyChannelInformation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncDoneApiModifyChannelInformation, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiModifyChannelInformation : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString GameId;
    FString Language;
    FString Title;
    int32 Delay;
    TArray<FString> Tags;
    bool ForceUpdateTags;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiModifyChannelInformation Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiModifyChannelInformation Error;

    virtual void Activate() override;

    /**
     * Modifies channel information for the currently logged-in user.
     *
     * Required scope: channel:manage:broadcast
     *
     * @param gameId The current game ID being played on the channel. Use “0” to unset the game.
     * @param language The language of the channel. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”.
     * @param title The title of the stream.
     * @param delay Stream delay in seconds. Stream delay is a Twitch Partner feature; trying to set this value for other account types will return a 400 error.
     * @param tags A list of channel-defined tags to apply to the channel. Tags help identify the content that the channel streams.
     * @param forceUpdateTags If ModifyChannelInfoRequest::Tags is missing or empty, channel tags will not be altered by default (if ModifyChannelInfoRequest::ForceUpdateTags is false). If ModifyChannelInfoRequest::ForceUpdateTags is true, passing an empty (or missing) array ModifyChannelInfoRequest::Tags causes all tags to be removed from the channel.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiModifyChannelInformation* ModifyChannelInformation(FString gameId, FString language, FString title, int32 delay, TArray<FString> tags, bool forceUpdateTags);
};
