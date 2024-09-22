
#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "TwitchSDK.h"
#include "TwitchSDKStructs.generated.h"


UENUM(BlueprintType)
enum class FTwitchSDKLogLevel : uint8
{
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
};

/** The current user authentication status. */
UENUM(BlueprintType)
enum class FTwitchSDKAuthStatus : uint8
{
    /** The core library is logged out. */
    LoggedOut = 0,
    /** The status can not be determined because an operation affecting it is currently in progress. */
    Loading = 1,
    /** The authorization code flow is in progress and we are currently waiting for the user to authorize. */
    WaitingForCode = 2,
    /** We have valid OAuth credentials. */
    LoggedIn = 3,
};

UENUM(BlueprintType)
enum class FTwitchSDKHypeTrainContributionType : uint8
{
    Bits = 0,
    Subscription = 1,
};

UENUM(BlueprintType)
enum class FTwitchSDKCustomRewardRedemptionState : uint8
{
    /** There was an error determining the state of the redemption. */
    Unknown = 0,
    /** The channel points reward has been redeemed and is now waiting to be fulfilled. */
    Unfulfilled = 1,
    /** The channel points reward has been fulfilled. */
    Fulfilled = 2,
    /** The channel points reward has been canceled, and the channel points have been refunded. */
    Canceled = 3,
};

/** The type of an EventSub subscription. */
UENUM(BlueprintType)
enum class FTwitchSDKEventStreamKind : uint8
{
    /** A notification when a specified channel receives a subscriber. This does not include resubscribes. */
    Subscription = 0,
    /** A specified channel receives a follow. */
    Follower = 1,
    /** A user cheers on the specified channel. */
    Cheer = 2,
    /** A viewer has redeemed a custom channel points reward on the specified channel or the redemption has been updated (i.e., fulfilled or cancelled). */
    CustomRewardRedemption = 3,
    /** A Hype Train makes progress on the user's channel. Requires the channel:read:hype_train scope. */
    HypeTrain = 4,
    /** A broadcaster raids another broadcaster’s channel. */
    ChannelRaid = 5,
};

/** Status of a Prediction. */
UENUM(BlueprintType)
enum class FTwitchSDKPredictionStatus : uint8
{
    /** The Prediction is active and viewers can make predictions. */
    Active = 0,
    /** The Prediction has been locked and viewers can no longer make predictions. */
    Locked = 1,
    /** A winning outcome has been chosen and the Channel Points have been distributed to the users who guessed the correct outcome. */
    Resolved = 2,
    /** The Prediction has been canceled and the Channel Points have been refunded to participants. */
    Canceled = 3,
};

/** Status of a poll. */
UENUM(BlueprintType)
enum class FTwitchSDKPollStatus : uint8
{
    /** Poll is currently in progress. */
    Active = 0,
    /** Poll has reached its PollInfo::EndedAt time. */
    Completed = 1,
    /** Poll has been manually terminated before its PollInfo::EndedAt time. */
    Terminated = 2,
    /** Poll is no longer visible on the channel. */
    Archived = 3,
    /** Poll is no longer visible to any user on Twitch. */
    Moderated = 4,
    /** Something went wrong determining the state. */
    Invalid = 5,
};

UENUM(BlueprintType)
enum class FTwitchSDKHttpMethod : uint8
{
    Get = 0,
    Post = 1,
    Put = 2,
    Patch = 3,
    Delete = 4,
};

/** The library's user authentication state. */
USTRUCT(BlueprintType)
struct FTwitchSDKAuthState {
    GENERATED_BODY()

public:
    FTwitchSDKAuthState() = default;
    FTwitchSDKAuthState(const R66::AuthState& x) : Status(FTwitchSDKAuthStatus(x.Status)), Scopes(R66::ToFStringArray(x.Scopes)) {}
    operator R66::AuthState () const {
        R66::AuthState v;
        v.Status = R66::AuthStatus(Status);
        v.Scopes = R66::FromFStringArray(Scopes);
        return v;
    }
    /** The current authentication status. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKAuthStatus Status;
    /** If Status is AuthStatus.LoggedIn, this is the set of available OAuth scopes. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Scopes;
};

/** A broadcaster raids another broadcaster’s channel. */
USTRUCT(BlueprintType)
struct FTwitchSDKChannelRaidEvent {
    GENERATED_BODY()

public:
    FTwitchSDKChannelRaidEvent() = default;
    FTwitchSDKChannelRaidEvent(const R66::ChannelRaidEvent& x) : FromBroadcasterId(R66::ToFString(x.FromBroadcasterId)), FromBroadcasterName(R66::ToFString(x.FromBroadcasterName)), ToBroadcasterId(R66::ToFString(x.ToBroadcasterId)), ToBroadcasterName(R66::ToFString(x.ToBroadcasterName)), Viewers(x.Viewers) {}
    operator R66::ChannelRaidEvent () const {
        R66::ChannelRaidEvent v;
        v.FromBroadcasterId = R66::FromFString(FromBroadcasterId);
        v.FromBroadcasterName = R66::FromFString(FromBroadcasterName);
        v.ToBroadcasterId = R66::FromFString(ToBroadcasterId);
        v.ToBroadcasterName = R66::FromFString(ToBroadcasterName);
        v.Viewers = Viewers;
        return v;
    }
    /** The broadcaster ID that created the raid. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString FromBroadcasterId;
    /** The broadcaster display name that created the raid. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString FromBroadcasterName;
    /** The broadcaster ID that received the raid. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ToBroadcasterId;
    /** The broadcaster display name that received the raid. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ToBroadcasterName;
    /** The number of viewers in the raid. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Viewers;
};

USTRUCT(BlueprintType)
struct FTwitchSDKCustomRewardResolveRequest {
    GENERATED_BODY()

public:
    FTwitchSDKCustomRewardResolveRequest() = default;
    FTwitchSDKCustomRewardResolveRequest(const R66::CustomRewardResolveRequest& x) : RedemptionId(R66::ToFString(x.RedemptionId)), CustomRewardId(R66::ToFString(x.CustomRewardId)), BroadcasterId(R66::ToFString(x.BroadcasterId)), Resolution(FTwitchSDKCustomRewardRedemptionState(x.Resolution)) {}
    operator R66::CustomRewardResolveRequest () const {
        R66::CustomRewardResolveRequest v;
        v.RedemptionId = R66::FromFString(RedemptionId);
        v.CustomRewardId = R66::FromFString(CustomRewardId);
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.Resolution = R66::CustomRewardRedemptionState(Resolution);
        return v;
    }
    /** ID of the Custom Reward Redemption to update. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString RedemptionId;
    /** ID of the Custom Reward the redemptions to be updated are for. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CustomRewardId;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** The new status to set redemptions to. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKCustomRewardRedemptionState Resolution;
};

/** Describes a contribution to a Hype Train. */
USTRUCT(BlueprintType)
struct FTwitchSDKHypeTrainContribution {
    GENERATED_BODY()

public:
    FTwitchSDKHypeTrainContribution() = default;
    FTwitchSDKHypeTrainContribution(const R66::HypeTrainContribution& x) : UserId(R66::ToFString(x.UserId)), UserName(R66::ToFString(x.UserName)), Type(FTwitchSDKHypeTrainContributionType(x.Type)), Total(x.Total) {}
    operator R66::HypeTrainContribution () const {
        R66::HypeTrainContribution v;
        v.UserId = R66::FromFString(UserId);
        v.UserName = R66::FromFString(UserName);
        v.Type = R66::HypeTrainContributionType(Type);
        v.Total = Total;
        return v;
    }
    /** The ID of the user who contributed to the Hype Train. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** The display name of the user who contributed to the Hype Train. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserName;
    /** Type of contribution. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKHypeTrainContributionType Type;
    /** The total contributed. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Total;
};

/** A Hype Train makes progress on the user's channel. Requires the channel:read:hype_train scope. */
USTRUCT(BlueprintType)
struct FTwitchSDKHypeTrainEvent {
    GENERATED_BODY()

public:
    FTwitchSDKHypeTrainEvent() = default;
    FTwitchSDKHypeTrainEvent(const R66::HypeTrainEvent& x) : HypeTrainId(R66::ToFString(x.HypeTrainId)), BroadcasterId(R66::ToFString(x.BroadcasterId)), BroadcasterName(R66::ToFString(x.BroadcasterName)), Level(x.Level), TotalPoints(x.TotalPoints), Progress(x.Progress), Goal(x.Goal), StartedAt(R66::ToFString(x.StartedAt)), ExpiresAt(R66::ToFString(x.ExpiresAt)), EndedAt(R66::ToFString(x.EndedAt)), CooldownEndsAt(R66::ToFString(x.CooldownEndsAt)), TopContributions(R66::ToUnrealArray<R66::HypeTrainContribution, FTwitchSDKHypeTrainContribution>(x.TopContributions)), LastContribution(x.LastContribution) {}
    operator R66::HypeTrainEvent () const {
        R66::HypeTrainEvent v;
        v.HypeTrainId = R66::FromFString(HypeTrainId);
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.BroadcasterName = R66::FromFString(BroadcasterName);
        v.Level = Level;
        v.TotalPoints = TotalPoints;
        v.Progress = Progress;
        v.Goal = Goal;
        v.StartedAt = R66::FromFString(StartedAt);
        v.ExpiresAt = R66::FromFString(ExpiresAt);
        v.EndedAt = R66::FromFString(EndedAt);
        v.CooldownEndsAt = R66::FromFString(CooldownEndsAt);
        v.TopContributions = R66::FromUnrealArray<R66::HypeTrainContribution, FTwitchSDKHypeTrainContribution>(TopContributions);
        v.LastContribution = LastContribution;
        return v;
    }
    /** The Hype Train ID. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString HypeTrainId;
    /** The requested broadcaster ID. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** The requested broadcaster display name. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterName;
    /** The current level of the Hype Train. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Level;
    /** Total points contributed to the Hype Train. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 TotalPoints;
    /** The number of points contributed to the Hype Train at the current level. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Progress;
    /** The number of points required to reach the next level. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Goal;
    /** The time when the Hype Train started. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString StartedAt;
    /** The time when the Hype Train expires. The expiration is extended when the Hype Train reaches a new level. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ExpiresAt;
    /** The time when the Hype Train ended. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString EndedAt;
    /** The time when the Hype Train cooldown ends so that the next Hype Train can start. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CooldownEndsAt;
    /** The contributors with the most points contributed. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKHypeTrainContribution> TopContributions;
    /** The most recent contribution. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKHypeTrainContribution LastContribution;
};

USTRUCT(BlueprintType)
struct FTwitchSDKCustomRewardDefinition {
    GENERATED_BODY()

public:
    FTwitchSDKCustomRewardDefinition() = default;
    FTwitchSDKCustomRewardDefinition(const R66::CustomRewardDefinition& x) : Title(R66::ToFString(x.Title)), Cost(x.Cost), Prompt(R66::ToFString(x.Prompt)), IsEnabled(x.IsEnabled), BackgroundColor(R66::ToFString(x.BackgroundColor)), IsUserInputRequired(x.IsUserInputRequired), IsMaxPerStreamEnabled(x.IsMaxPerStreamEnabled), MaxPerStream(x.MaxPerStream), IsMaxPerUserPerStreamEnabled(x.IsMaxPerUserPerStreamEnabled), MaxPerUserPerStream(x.MaxPerUserPerStream), IsGlobalCooldownEnabled(x.IsGlobalCooldownEnabled), GlobalCooldownSeconds(x.GlobalCooldownSeconds), ShouldRedemptionsSkipRequestQueue(x.ShouldRedemptionsSkipRequestQueue) {}
    operator R66::CustomRewardDefinition () const {
        R66::CustomRewardDefinition v;
        v.Title = R66::FromFString(Title);
        v.Cost = Cost;
        v.Prompt = R66::FromFString(Prompt);
        v.IsEnabled = IsEnabled;
        v.BackgroundColor = R66::FromFString(BackgroundColor);
        v.IsUserInputRequired = IsUserInputRequired;
        v.IsMaxPerStreamEnabled = IsMaxPerStreamEnabled;
        v.MaxPerStream = MaxPerStream;
        v.IsMaxPerUserPerStreamEnabled = IsMaxPerUserPerStreamEnabled;
        v.MaxPerUserPerStream = MaxPerUserPerStream;
        v.IsGlobalCooldownEnabled = IsGlobalCooldownEnabled;
        v.GlobalCooldownSeconds = GlobalCooldownSeconds;
        v.ShouldRedemptionsSkipRequestQueue = ShouldRedemptionsSkipRequestQueue;
        return v;
    }
    /** The title of the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** The cost of the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Cost;
    /** Optional. The prompt for the viewer when redeeming the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Prompt;
    /** Optional. Is the reward currently enabled, if false the reward won't show up to viewers. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsEnabled;
    /** Optional. Custom background color for the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BackgroundColor;
    /** Optional. Does the user need to enter information when redeeming the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsUserInputRequired;
    /** Optional. Whether a maximum per stream is enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsMaxPerStreamEnabled;
    /** Optional. The maximum number per stream if enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 MaxPerStream;
    /** Optional. Whether a maximum per user per stream is enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsMaxPerUserPerStreamEnabled;
    /** Optional. The maximum number per user per stream if enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 MaxPerUserPerStream;
    /** Optional. Whether a cooldown is enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsGlobalCooldownEnabled;
    /** Optional. The cooldown in seconds if enabled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 GlobalCooldownSeconds;
    /** Optional. Should redemptions be set to CustomRewardRedemptionState::Fulfilled status immediately when redeemed and skip the request queue instead of the normal CustomRewardRedemptionState::Unfulfilled status. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool ShouldRedemptionsSkipRequestQueue;
};

USTRUCT(BlueprintType)
struct FTwitchSDKCustomRewardList {
    GENERATED_BODY()

public:
    FTwitchSDKCustomRewardList() = default;
    FTwitchSDKCustomRewardList(const R66::CustomRewardList& x) : Rewards(R66::ToUnrealArray<R66::CustomRewardDefinition, FTwitchSDKCustomRewardDefinition>(x.Rewards)) {}
    operator R66::CustomRewardList () const {
        R66::CustomRewardList v;
        v.Rewards = R66::FromUnrealArray<R66::CustomRewardDefinition, FTwitchSDKCustomRewardDefinition>(Rewards);
        return v;
    }
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKCustomRewardDefinition> Rewards;
};

USTRUCT(BlueprintType)
struct FTwitchSDKBitsLeaderboardEntry {
    GENERATED_BODY()

public:
    FTwitchSDKBitsLeaderboardEntry() = default;
    FTwitchSDKBitsLeaderboardEntry(const R66::BitsLeaderboardEntry& x) : UserId(R66::ToFString(x.UserId)), UserName(R66::ToFString(x.UserName)), Rank(x.Rank), Score(x.Score) {}
    operator R66::BitsLeaderboardEntry () const {
        R66::BitsLeaderboardEntry v;
        v.UserId = R66::FromFString(UserId);
        v.UserName = R66::FromFString(UserName);
        v.Rank = Rank;
        v.Score = Score;
        return v;
    }
    /** ID of the user (viewer) in the leaderboard entry. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** Display name corresponding to BitsLeaderboardEntry::UserId. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserName;
    /** Leaderboard rank of the user. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Rank;
    /** Leaderboard score (number of Bits) of the user. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Score;
};

USTRUCT(BlueprintType)
struct FTwitchSDKBitsLeaderboard {
    GENERATED_BODY()

public:
    FTwitchSDKBitsLeaderboard() = default;
    FTwitchSDKBitsLeaderboard(const R66::BitsLeaderboard& x) : Data(R66::ToUnrealArray<R66::BitsLeaderboardEntry, FTwitchSDKBitsLeaderboardEntry>(x.Data)), StartedAt(R66::ToFString(x.StartedAt)), EndedAt(R66::ToFString(x.EndedAt)) {}
    operator R66::BitsLeaderboard () const {
        R66::BitsLeaderboard v;
        v.Data = R66::FromUnrealArray<R66::BitsLeaderboardEntry, FTwitchSDKBitsLeaderboardEntry>(Data);
        v.StartedAt = R66::FromFString(StartedAt);
        v.EndedAt = R66::FromFString(EndedAt);
        return v;
    }
    /** Entries on the bits leaderboard, in no particular order. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKBitsLeaderboardEntry> Data;
    /** Start of the date range for the returned data. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString StartedAt;
    /** End of the date range for the returned data. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString EndedAt;
};

USTRUCT(BlueprintType)
struct FTwitchSDKBitsLeaderboardRequest {
    GENERATED_BODY()

public:
    FTwitchSDKBitsLeaderboardRequest() = default;
    FTwitchSDKBitsLeaderboardRequest(const R66::BitsLeaderboardRequest& x) : Count(x.Count), Period(R66::ToFString(x.Period)), StartedAt(R66::ToFString(x.StartedAt)), UserId(R66::ToFString(x.UserId)) {}
    operator R66::BitsLeaderboardRequest () const {
        R66::BitsLeaderboardRequest v;
        v.Count = Count;
        v.Period = R66::FromFString(Period);
        v.StartedAt = R66::FromFString(StartedAt);
        v.UserId = R66::FromFString(UserId);
        return v;
    }
    /** Number of results to be returned. Maximum: 100. Default: 10. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Count;
    /** Time period over which data is aggregated (PST time zone). This parameter interacts with BitsLeaderboardRequest::StartedAt. Default: all. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Period;
    /** Timestamp for the period over which the returned data is aggregated. Must be in RFC 3339 format. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString StartedAt;
    /** ID of the user whose results are returned; i.e., the person who paid for the Bits. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
};

/** A specified channel receives a follow. */
USTRUCT(BlueprintType)
struct FTwitchSDKChannelFollowEvent {
    GENERATED_BODY()

public:
    FTwitchSDKChannelFollowEvent() = default;
    FTwitchSDKChannelFollowEvent(const R66::ChannelFollowEvent& x) : UserId(R66::ToFString(x.UserId)), UserDisplayName(R66::ToFString(x.UserDisplayName)), FollowedAt(R66::ToFString(x.FollowedAt)) {}
    operator R66::ChannelFollowEvent () const {
        R66::ChannelFollowEvent v;
        v.UserId = R66::FromFString(UserId);
        v.UserDisplayName = R66::FromFString(UserDisplayName);
        v.FollowedAt = R66::FromFString(FollowedAt);
        return v;
    }
    /** The user ID for the user now following the specified channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** The user display name for the user now following the specified channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserDisplayName;
    /** RFC3339 timestamp of when the follow occurred. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString FollowedAt;
};

/** A user cheers on the specified channel. */
USTRUCT(BlueprintType)
struct FTwitchSDKChannelCheerEvent {
    GENERATED_BODY()

public:
    FTwitchSDKChannelCheerEvent() = default;
    FTwitchSDKChannelCheerEvent(const R66::ChannelCheerEvent& x) : IsAnonymous(x.IsAnonymous), UserId(R66::ToFString(x.UserId)), UserDisplayName(R66::ToFString(x.UserDisplayName)), Message(R66::ToFString(x.Message)), Bits(x.Bits) {}
    operator R66::ChannelCheerEvent () const {
        R66::ChannelCheerEvent v;
        v.IsAnonymous = IsAnonymous;
        v.UserId = R66::FromFString(UserId);
        v.UserDisplayName = R66::FromFString(UserDisplayName);
        v.Message = R66::FromFString(Message);
        v.Bits = Bits;
        return v;
    }
    /** Whether the user cheered anonymously or not. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsAnonymous;
    /** The user ID for the user who cheered on the specified channel. This is null if ChannelCheerEvent::IsAnonymous is true. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** The user display name for the user who cheered on the specified channel. This is null if ChannelCheerEvent::IsAnonymous is true. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserDisplayName;
    /** The message sent with the cheer. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Message;
    /** The number of bits cheered. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Bits;
};

/** A viewer has redeemed a custom channel points reward on the specified channel or the redemption has been updated (i.e., fulfilled or cancelled). */
USTRUCT(BlueprintType)
struct FTwitchSDKCustomRewardEvent {
    GENERATED_BODY()

public:
    FTwitchSDKCustomRewardEvent() = default;
    FTwitchSDKCustomRewardEvent(const R66::CustomRewardEvent& x) : RedemptionId(R66::ToFString(x.RedemptionId)), BroadcasterId(R66::ToFString(x.BroadcasterId)), RedeemerId(R66::ToFString(x.RedeemerId)), BroadcasterName(R66::ToFString(x.BroadcasterName)), RedeemerName(R66::ToFString(x.RedeemerName)), UserInput(R66::ToFString(x.UserInput)), Status(FTwitchSDKCustomRewardRedemptionState(x.Status)), CustomRewardId(R66::ToFString(x.CustomRewardId)), CustomRewardTitle(R66::ToFString(x.CustomRewardTitle)), CustomRewardCost(x.CustomRewardCost), CustomRewardPrompt(R66::ToFString(x.CustomRewardPrompt)), RedeemedAt(R66::ToFString(x.RedeemedAt)) {}
    operator R66::CustomRewardEvent () const {
        R66::CustomRewardEvent v;
        v.RedemptionId = R66::FromFString(RedemptionId);
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.RedeemerId = R66::FromFString(RedeemerId);
        v.BroadcasterName = R66::FromFString(BroadcasterName);
        v.RedeemerName = R66::FromFString(RedeemerName);
        v.UserInput = R66::FromFString(UserInput);
        v.Status = R66::CustomRewardRedemptionState(Status);
        v.CustomRewardId = R66::FromFString(CustomRewardId);
        v.CustomRewardTitle = R66::FromFString(CustomRewardTitle);
        v.CustomRewardCost = CustomRewardCost;
        v.CustomRewardPrompt = R66::FromFString(CustomRewardPrompt);
        v.RedeemedAt = R66::FromFString(RedeemedAt);
        return v;
    }
    /** The redemption identifier. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString RedemptionId;
    /** The requested broadcaster ID. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** User ID of the user that redeemed the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString RedeemerId;
    /** The requested broadcaster display name. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterName;
    /** Display name of the user that redeemed the reward. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString RedeemerName;
    /** The user input provided. Empty string if not provided. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserInput;
    /** Status of the redemption. If you resolve or cancel all redemptions from your code, you should discard all events of this type with a status other than CustomRewardRedemptionState::Unfulfilled. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKCustomRewardRedemptionState Status;
    /** The reward identifier. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CustomRewardId;
    /** The reward name. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CustomRewardTitle;
    /** The reward cost. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 CustomRewardCost;
    /** The reward description. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CustomRewardPrompt;
    /** RFC3339 timestamp of when the reward was redeemed. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString RedeemedAt;
};

USTRUCT(BlueprintType)
struct FTwitchSDKEventStreamRequest {
    GENERATED_BODY()

public:
    FTwitchSDKEventStreamRequest() = default;
    FTwitchSDKEventStreamRequest(const R66::EventStreamRequest& x) : Kind(FTwitchSDKEventStreamKind(x.Kind)) {}
    operator R66::EventStreamRequest () const {
        R66::EventStreamRequest v;
        v.Kind = R66::EventStreamKind(Kind);
        return v;
    }
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKEventStreamKind Kind;
};

USTRUCT(BlueprintType)
struct FTwitchSDKEventStreamDesc {
    GENERATED_BODY()

public:
    FTwitchSDKEventStreamDesc() = default;
    FTwitchSDKEventStreamDesc(const R66::EventStreamDesc& x) : Kind(FTwitchSDKEventStreamKind(x.Kind)), Token(x.Token) {}
    operator R66::EventStreamDesc () const {
        R66::EventStreamDesc v;
        v.Kind = R66::EventStreamKind(Kind);
        v.Token = Token;
        return v;
    }
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKEventStreamKind Kind;
    /** Opaque token identifying the subscription. Do not touch. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Token;
};

USTRUCT(BlueprintType)
struct FTwitchSDKModifyChannelInfoRequest {
    GENERATED_BODY()

public:
    FTwitchSDKModifyChannelInfoRequest() = default;
    FTwitchSDKModifyChannelInfoRequest(const R66::ModifyChannelInfoRequest& x) : GameId(R66::ToFString(x.GameId)), Language(R66::ToFString(x.Language)), Title(R66::ToFString(x.Title)), Delay(x.Delay), Tags(R66::ToFStringArray(x.Tags)), ForceUpdateTags(x.ForceUpdateTags) {}
    operator R66::ModifyChannelInfoRequest () const {
        R66::ModifyChannelInfoRequest v;
        v.GameId = R66::FromFString(GameId);
        v.Language = R66::FromFString(Language);
        v.Title = R66::FromFString(Title);
        v.Delay = Delay;
        v.Tags = R66::FromFStringArray(Tags);
        v.ForceUpdateTags = ForceUpdateTags;
        return v;
    }
    /** The current game ID being played on the channel. Use “0” to unset the game. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString GameId;
    /** The language of the channel. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Language;
    /** The title of the stream. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Stream delay in seconds. Stream delay is a Twitch Partner feature; trying to set this value for other account types will return a 400 error. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Delay;
    /** A list of channel-defined tags to apply to the channel. Tags help identify the content that the channel streams. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Tags;
    /** If ModifyChannelInfoRequest::Tags is missing or empty, channel tags will not be altered by default (if ModifyChannelInfoRequest::ForceUpdateTags is false). If ModifyChannelInfoRequest::ForceUpdateTags is true, passing an empty (or missing) array ModifyChannelInfoRequest::Tags causes all tags to be removed from the channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool ForceUpdateTags;
};

/** Contains the information necessary to create a new prediction. */
USTRUCT(BlueprintType)
struct FTwitchSDKPredictionDefinition {
    GENERATED_BODY()

public:
    FTwitchSDKPredictionDefinition() = default;
    FTwitchSDKPredictionDefinition(const R66::PredictionDefinition& x) : Title(R66::ToFString(x.Title)), Outcomes(R66::ToFStringArray(x.Outcomes)), Duration(x.Duration) {}
    operator R66::PredictionDefinition () const {
        R66::PredictionDefinition v;
        v.Title = R66::FromFString(Title);
        v.Outcomes = R66::FromFStringArray(Outcomes);
        v.Duration = Duration;
        return v;
    }
    /** Title for the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Array of outcome titles for the Prediction. Array size must be 2. The first outcome object is the “blue” outcome and the second outcome object is the “pink” outcome when viewing the Prediction on Twitch. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Outcomes;
    /** Total duration for the Prediction (in seconds). Minimum: 1. Maximum: 1800. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Duration;
};

USTRUCT(BlueprintType)
struct FTwitchSDKPredictionOutcome {
    GENERATED_BODY()

public:
    FTwitchSDKPredictionOutcome() = default;
    FTwitchSDKPredictionOutcome(const R66::PredictionOutcome& x) : Id(R66::ToFString(x.Id)), Title(R66::ToFString(x.Title)), Users(x.Users), ChannelPoints(x.ChannelPoints), Color(R66::ToFString(x.Color)) {}
    operator R66::PredictionOutcome () const {
        R66::PredictionOutcome v;
        v.Id = R66::FromFString(Id);
        v.Title = R66::FromFString(Title);
        v.Users = Users;
        v.ChannelPoints = ChannelPoints;
        v.Color = R66::FromFString(Color);
        return v;
    }
    /** ID for the outcome. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** Text displayed for outcome. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Number of unique users that chose the outcome. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Users;
    /** Number of Channel Points used for the outcome. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 ChannelPoints;
    /** Color for the outcome. Valid values: BLUE, PINK */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Color;
};

/** Describes a Channel Points Predictions on a Twitch channel. */
USTRUCT(BlueprintType)
struct FTwitchSDKPredictionInfo {
    GENERATED_BODY()

public:
    FTwitchSDKPredictionInfo() = default;
    FTwitchSDKPredictionInfo(const R66::PredictionInfo& x) : Id(R66::ToFString(x.Id)), BroadcasterId(R66::ToFString(x.BroadcasterId)), Title(R66::ToFString(x.Title)), WinningOutcomeId(R66::ToFString(x.WinningOutcomeId)), Outcomes(R66::ToUnrealArray<R66::PredictionOutcome, FTwitchSDKPredictionOutcome>(x.Outcomes)), Status(FTwitchSDKPredictionStatus(x.Status)), CreatedAt(R66::ToFString(x.CreatedAt)), EndedAt(R66::ToFString(x.EndedAt)), LockedAt(R66::ToFString(x.LockedAt)) {}
    operator R66::PredictionInfo () const {
        R66::PredictionInfo v;
        v.Id = R66::FromFString(Id);
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.Title = R66::FromFString(Title);
        v.WinningOutcomeId = R66::FromFString(WinningOutcomeId);
        v.Outcomes = R66::FromUnrealArray<R66::PredictionOutcome, FTwitchSDKPredictionOutcome>(Outcomes);
        v.Status = R66::PredictionStatus(Status);
        v.CreatedAt = R66::FromFString(CreatedAt);
        v.EndedAt = R66::FromFString(EndedAt);
        v.LockedAt = R66::FromFString(LockedAt);
        return v;
    }
    /** ID of the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** ID of the broadcaster. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** Title for the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** ID of the winning outcome. If PredictionInfo::Status is PredictionStatus::Active, this is set to null. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString WinningOutcomeId;
    /** Array of possible outcomes for the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKPredictionOutcome> Outcomes;
    /** Status of the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKPredictionStatus Status;
    /** UTC timestamp for the Prediction’s start time. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CreatedAt;
    /** UTC timestamp for when the Prediction ended. If PredictionInfo::Status is PredictionStatus::Active, this is set to null. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString EndedAt;
    /** UTC timestamp for when the Prediction was locked. If PredictionInfo::Status is PredictionStatus::Active, this is set to null. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString LockedAt;
};

USTRUCT(BlueprintType)
struct FTwitchSDKEndPredictionRequest {
    GENERATED_BODY()

public:
    FTwitchSDKEndPredictionRequest() = default;
    FTwitchSDKEndPredictionRequest(const R66::EndPredictionRequest& x) : BroadcasterId(R66::ToFString(x.BroadcasterId)), PredictionId(R66::ToFString(x.PredictionId)), Status(FTwitchSDKPredictionStatus(x.Status)), WinningOutcomeId(R66::ToFString(x.WinningOutcomeId)) {}
    operator R66::EndPredictionRequest () const {
        R66::EndPredictionRequest v;
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.PredictionId = R66::FromFString(PredictionId);
        v.Status = R66::PredictionStatus(Status);
        v.WinningOutcomeId = R66::FromFString(WinningOutcomeId);
        return v;
    }
    /** The broadcaster running prediction events. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** ID of the Prediction. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString PredictionId;
    /** The Prediction status to be set. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKPredictionStatus Status;
    /** ID of the winning outcome for the Prediction, if EndPredictionRequest::Status is being set to PredictionStatus::Resolved. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString WinningOutcomeId;
};

/** Describes a Stream Marker. */
USTRUCT(BlueprintType)
struct FTwitchSDKStreamMarkerInfo {
    GENERATED_BODY()

public:
    FTwitchSDKStreamMarkerInfo() = default;
    FTwitchSDKStreamMarkerInfo(const R66::StreamMarkerInfo& x) : Id(R66::ToFString(x.Id)), CreatedAt(R66::ToFString(x.CreatedAt)), Description(R66::ToFString(x.Description)), PositionSeconds(x.PositionSeconds) {}
    operator R66::StreamMarkerInfo () const {
        R66::StreamMarkerInfo v;
        v.Id = R66::FromFString(Id);
        v.CreatedAt = R66::FromFString(CreatedAt);
        v.Description = R66::FromFString(Description);
        v.PositionSeconds = PositionSeconds;
        return v;
    }
    /** Unique ID of the marker. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** RFC3339 timestamp of the marker. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CreatedAt;
    /** Description of the marker. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Description;
    /** Relative offset (in seconds) of the marker, from the beginning of the stream. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 PositionSeconds;
};

/** Describes a Clip. */
USTRUCT(BlueprintType)
struct FTwitchSDKClipInfo {
    GENERATED_BODY()

public:
    FTwitchSDKClipInfo() = default;
    FTwitchSDKClipInfo(const R66::ClipInfo& x) : Id(R66::ToFString(x.Id)), Url(R66::ToFString(x.Url)), EmbedUrl(R66::ToFString(x.EmbedUrl)), CreatedAt(R66::ToFString(x.CreatedAt)), ThumbnailUrl(R66::ToFString(x.ThumbnailUrl)), Duration(x.Duration) {}
    operator R66::ClipInfo () const {
        R66::ClipInfo v;
        v.Id = R66::FromFString(Id);
        v.Url = R66::FromFString(Url);
        v.EmbedUrl = R66::FromFString(EmbedUrl);
        v.CreatedAt = R66::FromFString(CreatedAt);
        v.ThumbnailUrl = R66::FromFString(ThumbnailUrl);
        v.Duration = Duration;
        return v;
    }
    /** ID of the clip being queried. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** URL where the clip can be viewed. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Url;
    /** URL to embed the clip. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString EmbedUrl;
    /** Date when the clip was created. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CreatedAt;
    /** URL of the clip thumbnail. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ThumbnailUrl;
    /** Duration of the Clip in seconds. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 Duration;
};

USTRUCT(BlueprintType)
struct FTwitchSDKEndPollRequest {
    GENERATED_BODY()

public:
    FTwitchSDKEndPollRequest() = default;
    FTwitchSDKEndPollRequest(const R66::EndPollRequest& x) : BroadcasterId(R66::ToFString(x.BroadcasterId)), PollId(R66::ToFString(x.PollId)), ShowResults(x.ShowResults) {}
    operator R66::EndPollRequest () const {
        R66::EndPollRequest v;
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.PollId = R66::FromFString(PollId);
        v.ShowResults = ShowResults;
        return v;
    }
    /** ID of the broadcaster. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** ID of the poll. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString PollId;
    /** If true, sets the poll status to PollStatus::Terminated so that results are shown. If false, sets the poll status to PollStatus::Archived so that the poll is canceled immediately and no results are shown. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool ShowResults;
};

USTRUCT(BlueprintType)
struct FTwitchSDKPollChoiceInfo {
    GENERATED_BODY()

public:
    FTwitchSDKPollChoiceInfo() = default;
    FTwitchSDKPollChoiceInfo(const R66::PollChoiceInfo& x) : Id(R66::ToFString(x.Id)), Title(R66::ToFString(x.Title)), Votes(x.Votes), ChannelPointsVotes(x.ChannelPointsVotes) {}
    operator R66::PollChoiceInfo () const {
        R66::PollChoiceInfo v;
        v.Id = R66::FromFString(Id);
        v.Title = R66::FromFString(Title);
        v.Votes = Votes;
        v.ChannelPointsVotes = ChannelPointsVotes;
        return v;
    }
    /** ID for the choice. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** Text displayed for the choice. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Total number of votes received for the choice across all methods of voting. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Votes;
    /** Number of votes received via Channel Points. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 ChannelPointsVotes;
};

/** Describes a Poll on a Twitch channel. */
USTRUCT(BlueprintType)
struct FTwitchSDKPollInfo {
    GENERATED_BODY()

public:
    FTwitchSDKPollInfo() = default;
    FTwitchSDKPollInfo(const R66::PollInfo& x) : Id(R66::ToFString(x.Id)), BroadcasterId(R66::ToFString(x.BroadcasterId)), Title(R66::ToFString(x.Title)), Choices(R66::ToUnrealArray<R66::PollChoiceInfo, FTwitchSDKPollChoiceInfo>(x.Choices)), ChannelPointsVotingEnabled(x.ChannelPointsVotingEnabled), ChannelPointsPerVote(x.ChannelPointsPerVote), Status(FTwitchSDKPollStatus(x.Status)), StartedAt(R66::ToFString(x.StartedAt)), EndedAt(R66::ToFString(x.EndedAt)) {}
    operator R66::PollInfo () const {
        R66::PollInfo v;
        v.Id = R66::FromFString(Id);
        v.BroadcasterId = R66::FromFString(BroadcasterId);
        v.Title = R66::FromFString(Title);
        v.Choices = R66::FromUnrealArray<R66::PollChoiceInfo, FTwitchSDKPollChoiceInfo>(Choices);
        v.ChannelPointsVotingEnabled = ChannelPointsVotingEnabled;
        v.ChannelPointsPerVote = ChannelPointsPerVote;
        v.Status = R66::PollStatus(Status);
        v.StartedAt = R66::FromFString(StartedAt);
        v.EndedAt = R66::FromFString(EndedAt);
        return v;
    }
    /** ID of the poll. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** ID of the broadcaster. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterId;
    /** Question displayed for the poll. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Array of the poll choices. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKPollChoiceInfo> Choices;
    /** Indicates if Channel Points can be used for voting. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool ChannelPointsVotingEnabled;
    /** Number of Channel Points required to vote once with Channel Points. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 ChannelPointsPerVote;
    /** Poll status. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FTwitchSDKPollStatus Status;
    /** UTC timestamp for the poll’s start time. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString StartedAt;
    /** UTC timestamp for the poll’s end time. Set to null if the poll is still PollStatus::Active. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString EndedAt;
};

/** Describes a query for streams. All fields are optional. */
USTRUCT(BlueprintType)
struct FTwitchSDKStreamQuery {
    GENERATED_BODY()

public:
    FTwitchSDKStreamQuery() = default;
    FTwitchSDKStreamQuery(const R66::StreamQuery& x) : After(R66::ToFString(x.After)), First(x.First), GameIds(R66::ToFStringArray(x.GameIds)), Languages(R66::ToFStringArray(x.Languages)), UserIds(R66::ToFStringArray(x.UserIds)), UserLogins(R66::ToFStringArray(x.UserLogins)) {}
    operator R66::StreamQuery () const {
        R66::StreamQuery v;
        v.After = R66::FromFString(After);
        v.First = First;
        v.GameIds = R66::FromFStringArray(GameIds);
        v.Languages = R66::FromFStringArray(Languages);
        v.UserIds = R66::FromFStringArray(UserIds);
        v.UserLogins = R66::FromFStringArray(UserLogins);
        return v;
    }
    /** Cursor for forward pagination: tells the server where to start fetching the next set of results, in a multi-page response. The cursor value specified here is from the PaginationCursor response field of a prior query. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString After;
    /** Maximum number of objects to return. Maximum: 100. Default: 20. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 First;
    /** Returns streams broadcasting a specified game ID. You can specify up to 100 IDs. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> GameIds;
    /** Stream language. You can specify up to 100 languages. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Languages;
    /** Returns streams broadcast by one or more specified user IDs. You can specify up to 100 IDs. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> UserIds;
    /** Returns streams broadcast by one or more specified user login names. You can specify up to 100 names. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> UserLogins;
};

USTRUCT(BlueprintType)
struct FTwitchSDKWebRequestResult {
    GENERATED_BODY()

public:
    FTwitchSDKWebRequestResult() = default;
    FTwitchSDKWebRequestResult(const R66::WebRequestResult& x) : HttpStatus(x.HttpStatus), ResponseBody(R66::ToFString(x.ResponseBody)) {}
    operator R66::WebRequestResult () const {
        R66::WebRequestResult v;
        v.HttpStatus = HttpStatus;
        v.ResponseBody = R66::FromFString(ResponseBody);
        return v;
    }
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 HttpStatus;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ResponseBody;
};

USTRUCT(BlueprintType)
struct FTwitchSDKAuthenticationInfo {
    GENERATED_BODY()

public:
    FTwitchSDKAuthenticationInfo() = default;
    FTwitchSDKAuthenticationInfo(const R66::AuthenticationInfo& x) : Uri(R66::ToFString(x.Uri)), UserCode(R66::ToFString(x.UserCode)) {}
    operator R66::AuthenticationInfo () const {
        R66::AuthenticationInfo v;
        v.Uri = R66::FromFString(Uri);
        v.UserCode = R66::FromFString(UserCode);
        return v;
    }
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Uri;
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserCode;
};

USTRUCT(BlueprintType)
struct FTwitchSDKUserInfo {
    GENERATED_BODY()

public:
    FTwitchSDKUserInfo() = default;
    FTwitchSDKUserInfo(const R66::UserInfo& x) : ChannelId(R66::ToFString(x.ChannelId)), LoginName(R66::ToFString(x.LoginName)), DisplayName(R66::ToFString(x.DisplayName)), UserType(R66::ToFString(x.UserType)), BroadcasterType(R66::ToFString(x.BroadcasterType)), Description(R66::ToFString(x.Description)), ProfileImageUrl(R66::ToFString(x.ProfileImageUrl)), OfflineImageUrl(R66::ToFString(x.OfflineImageUrl)), ViewCount(x.ViewCount), CreatedAt(R66::ToFString(x.CreatedAt)), Email(R66::ToFString(x.Email)) {}
    operator R66::UserInfo () const {
        R66::UserInfo v;
        v.ChannelId = R66::FromFString(ChannelId);
        v.LoginName = R66::FromFString(LoginName);
        v.DisplayName = R66::FromFString(DisplayName);
        v.UserType = R66::FromFString(UserType);
        v.BroadcasterType = R66::FromFString(BroadcasterType);
        v.Description = R66::FromFString(Description);
        v.ProfileImageUrl = R66::FromFString(ProfileImageUrl);
        v.OfflineImageUrl = R66::FromFString(OfflineImageUrl);
        v.ViewCount = ViewCount;
        v.CreatedAt = R66::FromFString(CreatedAt);
        v.Email = R66::FromFString(Email);
        return v;
    }
    /** User’s ID. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ChannelId;
    /** User’s login name. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString LoginName;
    /** User’s display name. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString DisplayName;
    /** User’s type: "staff", "admin", "global_mod", or "". */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserType;
    /** User’s broadcaster type: "partner", "affiliate", or "". */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString BroadcasterType;
    /** User’s channel description. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Description;
    /** URL of the user’s profile image. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ProfileImageUrl;
    /** URL of the user’s offline image. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString OfflineImageUrl;
    /** Total number of views of the user’s channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 ViewCount;
    /** Date when the user was created. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString CreatedAt;
    /** User’s verified email address. Only available if you have the user:read:email scope. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Email;
};

/** Describes an active stream on Twitch. */
USTRUCT(BlueprintType)
struct FTwitchSDKStreamInfo {
    GENERATED_BODY()

public:
    FTwitchSDKStreamInfo() = default;
    FTwitchSDKStreamInfo(const R66::StreamInfo& x) : Id(R66::ToFString(x.Id)), UserId(R66::ToFString(x.UserId)), UserLogin(R66::ToFString(x.UserLogin)), UserName(R66::ToFString(x.UserName)), GameId(R66::ToFString(x.GameId)), GameName(R66::ToFString(x.GameName)), Type(R66::ToFString(x.Type)), Title(R66::ToFString(x.Title)), ViewerCount(x.ViewerCount), StartedAt(R66::ToFString(x.StartedAt)), Language(R66::ToFString(x.Language)), ThumbnailUrl(R66::ToFString(x.ThumbnailUrl)), Tags(R66::ToFStringArray(x.Tags)), IsMature(x.IsMature) {}
    operator R66::StreamInfo () const {
        R66::StreamInfo v;
        v.Id = R66::FromFString(Id);
        v.UserId = R66::FromFString(UserId);
        v.UserLogin = R66::FromFString(UserLogin);
        v.UserName = R66::FromFString(UserName);
        v.GameId = R66::FromFString(GameId);
        v.GameName = R66::FromFString(GameName);
        v.Type = R66::FromFString(Type);
        v.Title = R66::FromFString(Title);
        v.ViewerCount = ViewerCount;
        v.StartedAt = R66::FromFString(StartedAt);
        v.Language = R66::FromFString(Language);
        v.ThumbnailUrl = R66::FromFString(ThumbnailUrl);
        v.Tags = R66::FromFStringArray(Tags);
        v.IsMature = IsMature;
        return v;
    }
    /** Stream ID. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Id;
    /** ID of the user who is streaming. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** Login of the user who is streaming. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserLogin;
    /** Display name of the user who is streaming. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserName;
    /** ID of the game being played on the stream. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString GameId;
    /** Name of the game being played. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString GameName;
    /** Stream type: "live" or "" (in case of error). */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Type;
    /** Stream title. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Number of viewers watching the stream at the time of the query. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 ViewerCount;
    /** UTC timestamp. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString StartedAt;
    /** Stream language. A language value is either the ISO 639-1 two-letter code for a supported stream language or “other”. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Language;
    /** Thumbnail URL of the stream. All image URLs have variable width and height. You can replace {width} and {height} with any values to get that size image */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString ThumbnailUrl;
    /** Shows tag IDs that apply to the stream. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Tags;
    /** Indicates if the broadcaster has specified their channel contains mature content that may be inappropriate for younger audiences. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsMature;
};

USTRUCT(BlueprintType)
struct FTwitchSDKStreamQueryResult {
    GENERATED_BODY()

public:
    FTwitchSDKStreamQueryResult() = default;
    FTwitchSDKStreamQueryResult(const R66::StreamQueryResult& x) : Streams(R66::ToUnrealArray<R66::StreamInfo, FTwitchSDKStreamInfo>(x.Streams)), PaginationCursor(R66::ToFString(x.PaginationCursor)) {}
    operator R66::StreamQueryResult () const {
        R66::StreamQueryResult v;
        v.Streams = R66::FromUnrealArray<R66::StreamInfo, FTwitchSDKStreamInfo>(Streams);
        v.PaginationCursor = R66::FromFString(PaginationCursor);
        return v;
    }
    /** List of streams matching the query. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FTwitchSDKStreamInfo> Streams;
    /** Pagination cursor to fetch the next page of results. May be empty if there are no further results. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString PaginationCursor;
};

/** A notification when a specified channel receives a subscriber. This does not include resubscribes. */
USTRUCT(BlueprintType)
struct FTwitchSDKChannelSubscribeEvent {
    GENERATED_BODY()

public:
    FTwitchSDKChannelSubscribeEvent() = default;
    FTwitchSDKChannelSubscribeEvent(const R66::ChannelSubscribeEvent& x) : UserId(R66::ToFString(x.UserId)), UserLogin(R66::ToFString(x.UserLogin)), UserDisplayName(R66::ToFString(x.UserDisplayName)), Tier(R66::ToFString(x.Tier)), IsGift(x.IsGift), CumulativeMonths(x.CumulativeMonths), StreakMonths(x.StreakMonths), DurationMonths(x.DurationMonths) {}
    operator R66::ChannelSubscribeEvent () const {
        R66::ChannelSubscribeEvent v;
        v.UserId = R66::FromFString(UserId);
        v.UserLogin = R66::FromFString(UserLogin);
        v.UserDisplayName = R66::FromFString(UserDisplayName);
        v.Tier = R66::FromFString(Tier);
        v.IsGift = IsGift;
        v.CumulativeMonths = CumulativeMonths;
        v.StreakMonths = StreakMonths;
        v.DurationMonths = DurationMonths;
        return v;
    }
    /** The user ID for the user who subscribed to the specified channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserId;
    /** The user login for the user who subscribed to the specified channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserLogin;
    /** The user display name for the user who subscribed to the specified channel. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString UserDisplayName;
    /** The tier of the subscription. Valid values are 1000, 2000, and 3000. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Tier;
    /** Whether the subscription is a gift. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsGift;
    /** The total number of months the user has been subscribed to the channel. This is nonzero only for resubscriptions. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 CumulativeMonths;
    /** The number of consecutive months the user’s current subscription has been active. This value is -1 if the user has opted out of sharing this information. This is nonzero only for resubscriptions. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 StreakMonths;
    /** The month duration of the subscription. This is nonzero only for resubscriptions. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 DurationMonths;
};

/** Describes a user's subscription to a specified channel. */
USTRUCT(BlueprintType)
struct FTwitchSDKUserSubscriptionCheckResult {
    GENERATED_BODY()

public:
    FTwitchSDKUserSubscriptionCheckResult() = default;
    FTwitchSDKUserSubscriptionCheckResult(const R66::UserSubscriptionCheckResult& x) : Tier(R66::ToFString(x.Tier)), GifterLogin(R66::ToFString(x.GifterLogin)), GifterName(R66::ToFString(x.GifterName)), IsGift(x.IsGift), IsSubscribed(x.IsSubscribed) {}
    operator R66::UserSubscriptionCheckResult () const {
        R66::UserSubscriptionCheckResult v;
        v.Tier = R66::FromFString(Tier);
        v.GifterLogin = R66::FromFString(GifterLogin);
        v.GifterName = R66::FromFString(GifterName);
        v.IsGift = IsGift;
        v.IsSubscribed = IsSubscribed;
        return v;
    }
    /** Subscription tier. 1000 is tier 1, 2000 is tier 2, and 3000 is tier 3. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Tier;
    /** Login of the gifter (if UserSubscriptionCheckResult::IsGift is true). */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString GifterLogin;
    /** Display name of the gifter (if UserSubscriptionCheckResult::IsGift is true). */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString GifterName;
    /** Indicates if the subscription is a gift. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsGift;
    /** Indicates if the user is subscribed at all. If this is false, all other fields are empty. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool IsSubscribed;
};

/** Contains the information necessary to create a new poll. */
USTRUCT(BlueprintType)
struct FTwitchSDKPollDefinition {
    GENERATED_BODY()

public:
    FTwitchSDKPollDefinition() = default;
    FTwitchSDKPollDefinition(const R66::PollDefinition& x) : Title(R66::ToFString(x.Title)), Choices(R66::ToFStringArray(x.Choices)), Duration(x.Duration), BitsVotingEnabled(x.BitsVotingEnabled), BitsPerVote(x.BitsPerVote), ChannelPointsVotingEnabled(x.ChannelPointsVotingEnabled), ChannelPointsPerVote(x.ChannelPointsPerVote) {}
    operator R66::PollDefinition () const {
        R66::PollDefinition v;
        v.Title = R66::FromFString(Title);
        v.Choices = R66::FromFStringArray(Choices);
        v.Duration = Duration;
        v.BitsVotingEnabled = BitsVotingEnabled;
        v.BitsPerVote = BitsPerVote;
        v.ChannelPointsVotingEnabled = ChannelPointsVotingEnabled;
        v.ChannelPointsPerVote = ChannelPointsPerVote;
        return v;
    }
    /** Question displayed for the poll. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") FString Title;
    /** Array of the poll choices. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") TArray<FString> Choices;
    /** Total duration for the poll (in seconds). */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int64 Duration;
    /** Indicates if Bits can be used for voting. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool BitsVotingEnabled;
    /** Number of Bits required to vote once with Bits. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 BitsPerVote;
    /** Indicates if Channel Points can be used for voting. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") bool ChannelPointsVotingEnabled;
    /** Number of Channel Points required to vote once with Channel Points. */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Twitch") int32 ChannelPointsPerVote;
};

