#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCreateStreamMarker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiCreateStreamMarker, FTwitchSDKStreamMarkerInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCreateStreamMarker : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    FString Description;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreateStreamMarker Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreateStreamMarker Error;

    virtual void Activate() override;

    /**
     * Creates a marker in the stream of a user.
     *
     * Required scope: channel:manage:broadcast
     *
     * A marker is an arbitrary point in a stream that the broadcaster wants to mark; e.g., to easily return to later. The marker is created at the current timestamp in the live broadcast when the request is processed. Markers can be created by the stream owner or editors.
     *
     * Markers cannot be created in some cases (an error will occur):
     * - If the specified user’s stream is not live.
     * - If VOD (past broadcast) storage is not enabled for the stream.
     * - For premieres (live, first-viewing events that combine uploaded videos with live chat).
     * - For reruns (subsequent (not live) streaming of any past broadcast, including past premieres).
     *
     *
     * @param description Optional. Description of or comments on the marker. Max length is 140 characters.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCreateStreamMarker* CreateStreamMarker(FString description);
};
