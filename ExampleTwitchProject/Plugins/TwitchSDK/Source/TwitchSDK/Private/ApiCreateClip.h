#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "TwitchSDKStructs.h"

#include "ApiCreateClip.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncDoneApiCreateClip, FTwitchSDKClipInfo, Result, FString, Error);

UCLASS(meta=(HideThen=true))
class TWITCHSDK_API UApiCreateClip : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

    bool HasDelay;

public:
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreateClip Done;
    UPROPERTY(BlueprintAssignable)
    FAsyncDoneApiCreateClip Error;

    virtual void Activate() override;

    /**
     * Creates a clip programmatically. This returns both an ID and an edit URL for the new clip.
     *
     * Required scope: clips:edit
     *
     * Creating a clip can take up to 15 seconds.
     *
     * If hasDelay is false, the clip is captured from the live stream when the API is called; otherwise, a delay is added before the clip is captured (to account for the brief delay between the broadcaster’s stream and the viewer’s experience of that stream).
     *
     * @param hasDelay Whether stream delay should be added before capturing the clip.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Twitch")
    static UApiCreateClip* CreateClip(bool hasDelay);
};
