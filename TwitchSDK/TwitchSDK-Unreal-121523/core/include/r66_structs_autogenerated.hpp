#pragma once
/** \file */

namespace R66 {
    enum class LogLevel : uint8_t
    {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3,
    };
    /// <summary>
    /// The current user authentication status.
    /// </summary>
    enum class AuthStatus : uint8_t
    {
        /// <summary>
        /// The core library is logged out.
        /// </summary>
        LoggedOut = 0,
        /// <summary>
        /// The status can not be determined because an operation affecting it is currently in progress.
        /// </summary>
        Loading = 1,
        /// <summary>
        /// The authorization code flow is in progress and we are currently waiting for the user to authorize.
        /// </summary>
        WaitingForCode = 2,
        /// <summary>
        /// We have valid OAuth credentials.
        /// </summary>
        /// <remarks>
        /// Note that these credentials may still be rejected if the user revokes your Client-Id from their account.
        /// </remarks>
        LoggedIn = 3,
    };
    enum class HypeTrainContributionType : uint8_t
    {
        Bits = 0,
        Subscription = 1,
    };
    enum class CustomRewardRedemptionState : uint8_t
    {
        /// <summary>
        /// There was an error determining the state of the redemption.
        /// </summary>
        Unknown = 0,
        /// <summary>
        /// The channel points reward has been redeemed and is now waiting to be fulfilled.
        /// </summary>
        Unfulfilled = 1,
        /// <summary>
        /// The channel points reward has been fulfilled.
        /// </summary>
        Fulfilled = 2,
        /// <summary>
        /// The channel points reward has been canceled, and the channel points have been refunded.
        /// </summary>
        Canceled = 3,
    };
    /// <summary>
    /// The type of an EventSub subscription.
    /// </summary>
    enum class EventStreamKind : uint8_t
    {
        /// <summary>
        /// A notification when a specified channel receives a subscriber. This does not include resubscribes.
        /// </summary>
        Subscription = 0,
        /// <summary>
        /// A specified channel receives a follow.
        /// </summary>
        Follower = 1,
        /// <summary>
        /// A user cheers on the specified channel.
        /// </summary>
        Cheer = 2,
        /// <summary>
        /// A viewer has redeemed a custom channel points reward on the specified channel or the redemption
        /// has been updated (i.e., fulfilled or cancelled).
        /// </summary>
        CustomRewardRedemption = 3,
        /// <summary>
        /// A Hype Train makes progress on the user's channel.
        /// Requires the <c>channel:read:hype_train</c> scope.
        /// </summary>
        HypeTrain = 4,
        /// <summary>
        /// A broadcaster raids another broadcaster’s channel.
        /// </summary>
        ChannelRaid = 5,
    };
    /// <summary>
    /// Status of a Prediction.
    /// </summary>
    enum class PredictionStatus : uint8_t
    {
        /// <summary>
        /// The Prediction is active and viewers can make predictions.
        /// </summary>
        Active = 0,
        /// <summary>
        /// The Prediction has been locked and viewers can no longer make predictions.
        /// </summary>
        Locked = 1,
        /// <summary>
        /// A winning outcome has been chosen and the Channel Points have been distributed to the users who guessed the correct outcome.
        /// </summary>
        Resolved = 2,
        /// <summary>
        /// The Prediction has been canceled and the Channel Points have been refunded to participants.
        /// </summary>
        Canceled = 3,
    };
    /// <summary>
    /// Status of a poll.
    /// </summary>
    enum class PollStatus : uint8_t
    {
        /// <summary>
        /// Poll is currently in progress.
        /// </summary>
        Active = 0,
        /// <summary>
        /// Poll has reached its <see cref="PollInfo.EndedAt" /> time.
        /// </summary>
        Completed = 1,
        /// <summary>
        /// Poll has been manually terminated before its <see cref="PollInfo.EndedAt" /> time.
        /// </summary>
        Terminated = 2,
        /// <summary>
        /// Poll is no longer visible on the channel.
        /// </summary>
        Archived = 3,
        /// <summary>
        /// Poll is no longer visible to any user on Twitch.
        /// </summary>
        Moderated = 4,
        /// <summary>
        /// Something went wrong determining the state.
        /// </summary>
        Invalid = 5,
    };
    enum class HttpMethod : uint8_t
    {
        Get = 0,
        Post = 1,
        Put = 2,
        Patch = 3,
        Delete = 4,
    };
    struct AuthState;
    struct ChannelRaidEvent;
    struct CustomRewardResolveRequest;
    struct HypeTrainContribution;
    struct HypeTrainEvent;
    struct CustomRewardDefinition;
    struct CustomRewardList;
    struct BitsLeaderboardEntry;
    struct BitsLeaderboard;
    struct BitsLeaderboardRequest;
    struct ChannelFollowEvent;
    struct ChannelCheerEvent;
    struct CustomRewardEvent;
    struct EventStreamRequest;
    struct EventStreamDesc;
    struct ModifyChannelInfoRequest;
    struct PredictionDefinition;
    struct PredictionOutcome;
    struct PredictionInfo;
    struct EndPredictionRequest;
    struct StreamMarkerInfo;
    struct ClipInfo;
    struct EndPollRequest;
    struct PollChoiceInfo;
    struct PollInfo;
    struct StreamQuery;
    struct WebRequestResult;
    struct AuthenticationInfo;
    struct UserInfo;
    struct StreamInfo;
    struct StreamQueryResult;
    struct ChannelSubscribeEvent;
    struct UserSubscriptionCheckResult;
    struct PollDefinition;
    /// @class AuthState r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// The library's user authentication state.
    /// </summary>
    struct AuthState {
        /// <summary>
        /// The current authentication status.
        /// </summary>
        AuthStatus Status = AuthStatus(0);
        /// <summary>
        /// If Status is AuthStatus.LoggedIn, this is the set of available OAuth scopes.
        /// </summary>
        /// <remarks>
        /// This may be smaller than the set of scopes you passed to GetAuthenticationInfo if the user logged in with a previous version of your application.
        /// </remarks>
        std::vector<R66::string_holder> Scopes;
    };
    /// @class ChannelRaidEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A broadcaster raids another broadcaster’s channel.
    /// </summary>
    struct ChannelRaidEvent {
        /// <summary>
        /// The broadcaster ID that created the raid.
        /// </summary>
        R66::string_holder FromBroadcasterId;
        /// <summary>
        /// The broadcaster display name that created the raid.
        /// </summary>
        R66::string_holder FromBroadcasterName;
        /// <summary>
        /// The broadcaster ID that received the raid.
        /// </summary>
        R66::string_holder ToBroadcasterId;
        /// <summary>
        /// The broadcaster display name that received the raid.
        /// </summary>
        R66::string_holder ToBroadcasterName;
        /// <summary>
        /// The number of viewers in the raid.
        /// </summary>
        int64_t Viewers = 0;
    };
    /// @class CustomRewardResolveRequest r66_structs_autogenerated.hpp r66.hpp
    struct CustomRewardResolveRequest {
        /// <summary>
        /// ID of the Custom Reward Redemption to update.
        /// </summary>
        R66::string_holder RedemptionId;
        /// <summary>
        /// ID of the Custom Reward the redemptions to be updated are for.
        /// </summary>
        R66::string_holder CustomRewardId;
        R66::string_holder BroadcasterId;
        /// <summary>
        /// The new status to set redemptions to.
        /// </summary>
        /// <remarks>
        /// Can be either <see cref="CustomRewardRedemptionState.Fulfilled" /> or <see cref="CustomRewardRedemptionState.Canceled" />.
        /// Specifying <see cref="CustomRewardRedemptionState.Canceled" /> will refund the user their Channel Points.
        /// </remarks>
        CustomRewardRedemptionState Resolution = CustomRewardRedemptionState(0);
    };
    /// @class HypeTrainContribution r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a contribution to a Hype Train.
    /// </summary>
    struct HypeTrainContribution {
        /// <summary>
        /// The ID of the user who contributed to the Hype Train.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// The display name of the user who contributed to the Hype Train.
        /// </summary>
        R66::string_holder UserName;
        /// <summary>
        /// Type of contribution.
        /// </summary>
        HypeTrainContributionType Type = HypeTrainContributionType(0);
        /// <summary>
        /// The total contributed.
        /// </summary>
        int64_t Total = 0;
    };
    /// @class HypeTrainEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A Hype Train makes progress on the user's channel.
    /// Requires the <c>channel:read:hype_train</c> scope.
    /// </summary>
    struct HypeTrainEvent {
        /// <summary>
        /// The Hype Train ID.
        /// </summary>
        R66::string_holder HypeTrainId;
        /// <summary>
        /// The requested broadcaster ID.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// The requested broadcaster display name.
        /// </summary>
        R66::string_holder BroadcasterName;
        /// <summary>
        /// The current level of the Hype Train.
        /// </summary>
        int32_t Level = 0;
        /// <summary>
        /// Total points contributed to the Hype Train.
        /// </summary>
        int64_t TotalPoints = 0;
        /// <summary>
        /// The number of points contributed to the Hype Train at the current level.
        /// </summary>
        /// <remarks>
        /// Set to <c>-1</c> when the hype train is over.
        /// </remarks>
        int64_t Progress = 0;
        /// <summary>
        /// The number of points required to reach the next level.
        /// </summary>
        /// <remarks>
        /// Set to <c>-1</c> when the hype train is over.
        /// </remarks>
        int64_t Goal = 0;
        /// <summary>
        /// The time when the Hype Train started.
        /// </summary>
        R66::string_holder StartedAt;
        /// <summary>
        /// The time when the Hype Train expires. The expiration is extended when the Hype Train reaches a new level.
        /// </summary>
        /// <remarks>
        /// This is empty when the Hype Train is over.
        /// </remarks>
        R66::string_holder ExpiresAt;
        /// <summary>
        /// The time when the Hype Train ended.
        /// </summary>
        /// <remarks>
        /// Empty if the Hype Train has not ended yet.
        /// </remarks>
        R66::string_holder EndedAt;
        /// <summary>
        /// The time when the Hype Train cooldown ends so that the next Hype Train can start.
        /// </summary>
        /// <remarks>
        /// Empty if the Hype Train has not ended yet.
        /// </remarks>
        R66::string_holder CooldownEndsAt;
        /// <summary>
        /// The contributors with the most points contributed.
        /// </summary>
        /// <remarks>
        /// Contains a maximum of two objects: one for the top bits contributor and one for the top subscriptions contributor (this includes gifted subscriptions).
        /// </remarks>
        std::vector<HypeTrainContribution> TopContributions;
        /// <summary>
        /// The most recent contribution.
        /// </summary>
        /// <remarks>
        /// This is empty when the Hype Train is over.
        /// </remarks>
        HypeTrainContribution LastContribution;
    };
    /// @class CustomRewardDefinition r66_structs_autogenerated.hpp r66.hpp
    struct CustomRewardDefinition {
        /// <summary>
        /// The title of the reward.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// The cost of the reward.
        /// </summary>
        int64_t Cost = 0;
        /// <summary>
        /// Optional. The prompt for the viewer when redeeming the reward.
        /// </summary>
        R66::string_holder Prompt;
        /// <summary>
        /// Optional. Is the reward currently enabled, if <c>false</c> the reward won't show up to viewers.
        /// </summary>
        bool IsEnabled = false;
        /// <summary>
        /// Optional. Custom background color for the reward.
        /// </summary>
        /// <remarks>
        /// Format: Hex with # prefix. Example: <c>#00E5CB</c>.
        /// </remarks>
        R66::string_holder BackgroundColor;
        /// <summary>
        /// Optional. Does the user need to enter information when redeeming the reward.
        /// </summary>
        bool IsUserInputRequired = false;
        /// <summary>
        /// Optional. Whether a maximum per stream is enabled.
        /// </summary>
        bool IsMaxPerStreamEnabled = false;
        /// <summary>
        /// Optional. The maximum number per stream if enabled.
        /// </summary>
        int32_t MaxPerStream = 0;
        /// <summary>
        /// Optional. Whether a maximum per user per stream is enabled.
        /// </summary>
        bool IsMaxPerUserPerStreamEnabled = false;
        /// <summary>
        /// Optional. The maximum number per user per stream if enabled.
        /// </summary>
        int32_t MaxPerUserPerStream = 0;
        /// <summary>
        /// Optional. Whether a cooldown is enabled.
        /// </summary>
        bool IsGlobalCooldownEnabled = false;
        /// <summary>
        /// Optional. The cooldown in seconds if enabled.
        /// </summary>
        int32_t GlobalCooldownSeconds = 0;
        /// <summary>
        /// Optional. Should redemptions be set to <see cref="CustomRewardRedemptionState.Fulfilled" /> status immediately
        /// when redeemed and skip the request queue instead of the normal <see cref="CustomRewardRedemptionState.Unfulfilled" /> status.
        /// </summary>
        bool ShouldRedemptionsSkipRequestQueue = false;
    };
    /// @class CustomRewardList r66_structs_autogenerated.hpp r66.hpp
    struct CustomRewardList {
        std::vector<CustomRewardDefinition> Rewards;
    };
    /// @class BitsLeaderboardEntry r66_structs_autogenerated.hpp r66.hpp
    struct BitsLeaderboardEntry {
        /// <summary>
        /// ID of the user (viewer) in the leaderboard entry.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// Display name corresponding to <see cref="BitsLeaderboardEntry.UserId" />.
        /// </summary>
        R66::string_holder UserName;
        /// <summary>
        /// Leaderboard rank of the user.
        /// </summary>
        int64_t Rank = 0;
        /// <summary>
        /// Leaderboard score (number of Bits) of the user.
        /// </summary>
        int64_t Score = 0;
    };
    /// @class BitsLeaderboard r66_structs_autogenerated.hpp r66.hpp
    struct BitsLeaderboard {
        /// <summary>
        /// Entries on the bits leaderboard, in no particular order.
        /// </summary>
        std::vector<BitsLeaderboardEntry> Data;
        /// <summary>
        /// Start of the date range for the returned data.
        /// </summary>
        R66::string_holder StartedAt;
        /// <summary>
        /// End of the date range for the returned data.
        /// </summary>
        R66::string_holder EndedAt;
    };
    /// @class BitsLeaderboardRequest r66_structs_autogenerated.hpp r66.hpp
    struct BitsLeaderboardRequest {
        /// <summary>
        /// Number of results to be returned. Maximum: 100. Default: 10.
        /// </summary>
        int32_t Count = 0;
        /// <summary>
        /// Time period over which data is aggregated (PST time zone). This parameter interacts with <see cref="BitsLeaderboardRequest.StartedAt" />. Default: <c>all</c>.
        /// </summary>
        /// <remarks>
        /// Valid values:
        ///
        /// <list type="bullet"><item><description><c>day</c> – 00:00:00 on the day specified in <see cref="BitsLeaderboardRequest.StartedAt" />, through 00:00:00 on the following day.</description></item><item><description><c>week</c> – 00:00:00 on Monday of the week specified in <see cref="BitsLeaderboardRequest.StartedAt" />, through 00:00:00 on the following Monday.</description></item><item><description><c>month</c> – 00:00:00 on the first day of the month specified in <see cref="BitsLeaderboardRequest.StartedAt" />, through 00:00:00 on the first day of the following month.</description></item><item><description><c>year</c> – 00:00:00 on the first day of the year specified in <see cref="BitsLeaderboardRequest.StartedAt" />, through 00:00:00 on the first day of the following year.</description></item><item><description><c>all</c> – The lifetime of the broadcaster's channel. If this is specified (or used by default), <see cref="BitsLeaderboardRequest.StartedAt" /> is ignored.</description></item></list></remarks>
        R66::string_holder Period;
        /// <summary>
        /// Timestamp for the period over which the returned data is aggregated. Must be in RFC 3339 format.
        /// </summary>
        /// <remarks>
        /// If this is not provided, data is aggregated over the current period; e.g., the current day/week/month/year.
        /// This value is ignored if <see cref="BitsLeaderboardRequest.Period" /> is <c>all</c>.
        /// Currently, the HH:MM:SS part of this value is used only to identify a given day in PST and otherwise ignored.
        /// For example, if the <see cref="BitsLeaderboardRequest.StartedAt" /> value resolves to 5PM PST yesterday and <see cref="BitsLeaderboardRequest.Period" /> is <c>day</c>, data is returned for all of yesterday.
        /// </remarks>
        R66::string_holder StartedAt;
        /// <summary>
        /// ID of the user whose results are returned; i.e., the person who paid for the Bits.
        /// </summary>
        /// <remarks>
        /// As long as <see cref="BitsLeaderboardRequest.Count" /> is greater than 1, the returned data includes additional users, with Bits amounts above and below the user specified by <see cref="BitsLeaderboardRequest.UserId" />.
        /// If <see cref="BitsLeaderboardRequest.UserId" /> is not provided, the endpoint returns the Bits leaderboard data across top users (subject to the value of <see cref="BitsLeaderboardRequest.Count" />).
        /// </remarks>
        R66::string_holder UserId;
    };
    /// @class ChannelFollowEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A specified channel receives a follow.
    /// </summary>
    struct ChannelFollowEvent {
        /// <summary>
        /// The user ID for the user now following the specified channel.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// The user display name for the user now following the specified channel.
        /// </summary>
        R66::string_holder UserDisplayName;
        /// <summary>
        /// RFC3339 timestamp of when the follow occurred.
        /// </summary>
        R66::string_holder FollowedAt;
    };
    /// @class ChannelCheerEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A user cheers on the specified channel.
    /// </summary>
    struct ChannelCheerEvent {
        /// <summary>
        /// Whether the user cheered anonymously or not.
        /// </summary>
        bool IsAnonymous = false;
        /// <summary>
        /// The user ID for the user who cheered on the specified channel. This is null if <see cref="ChannelCheerEvent.IsAnonymous" /> is true.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// The user display name for the user who cheered on the specified channel. This is null if <see cref="ChannelCheerEvent.IsAnonymous" /> is true.
        /// </summary>
        R66::string_holder UserDisplayName;
        /// <summary>
        /// The message sent with the cheer.
        /// </summary>
        R66::string_holder Message;
        /// <summary>
        /// The number of bits cheered.
        /// </summary>
        int64_t Bits = 0;
    };
    /// @class CustomRewardEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A viewer has redeemed a custom channel points reward on the specified channel or the redemption
    /// has been updated (i.e., fulfilled or cancelled).
    /// </summary>
    struct CustomRewardEvent {
        /// <summary>
        /// The redemption identifier.
        /// </summary>
        R66::string_holder RedemptionId;
        /// <summary>
        /// The requested broadcaster ID.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// User ID of the user that redeemed the reward.
        /// </summary>
        R66::string_holder RedeemerId;
        /// <summary>
        /// The requested broadcaster display name.
        /// </summary>
        R66::string_holder BroadcasterName;
        /// <summary>
        /// Display name of the user that redeemed the reward.
        /// </summary>
        R66::string_holder RedeemerName;
        /// <summary>
        /// The user input provided. Empty string if not provided.
        /// </summary>
        R66::string_holder UserInput;
        /// <summary>
        /// Status of the redemption.
        /// If you resolve or cancel all redemptions from your code, you should discard all events of this type
        /// with a status other than <see cref="CustomRewardRedemptionState.Unfulfilled" />.
        /// </summary>
        CustomRewardRedemptionState Status = CustomRewardRedemptionState(0);
        /// <summary>
        /// The reward identifier.
        /// </summary>
        R66::string_holder CustomRewardId;
        /// <summary>
        /// The reward name.
        /// </summary>
        R66::string_holder CustomRewardTitle;
        /// <summary>
        /// The reward cost.
        /// </summary>
        int64_t CustomRewardCost = 0;
        /// <summary>
        /// The reward description.
        /// </summary>
        R66::string_holder CustomRewardPrompt;
        /// <summary>
        /// RFC3339 timestamp of when the reward was redeemed.
        /// </summary>
        R66::string_holder RedeemedAt;
    };
    /// @class EventStreamRequest r66_structs_autogenerated.hpp r66.hpp
    struct EventStreamRequest {
        EventStreamKind Kind = EventStreamKind(0);
    };
    /// @class EventStreamDesc r66_structs_autogenerated.hpp r66.hpp
    struct EventStreamDesc {
        EventStreamKind Kind = EventStreamKind(0);
        /// <summary>
        /// Opaque token identifying the subscription. Do not touch.
        /// </summary>
        int32_t Token = 0;
    };
    /// @class ModifyChannelInfoRequest r66_structs_autogenerated.hpp r66.hpp
    struct ModifyChannelInfoRequest {
        /// <summary>
        /// The current game ID being played on the channel. Use “0” to unset the game.
        /// </summary>
        R66::string_holder GameId;
        /// <summary>
        /// The language of the channel. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”.
        /// </summary>
        R66::string_holder Language;
        /// <summary>
        /// The title of the stream.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Stream delay in seconds. Stream delay is a Twitch Partner feature; trying to set this value for other account types will return a 400 error.
        /// </summary>
        int32_t Delay = 0;
        /// <summary>
        /// A list of channel-defined tags to apply to the channel. Tags help identify the content that the channel streams.
        /// </summary>
        /// <remarks>
        /// A channel may specify a maximum of 10 tags.
        /// Each tag is limited to a maximum of 25 characters and may not be an empty string or contain spaces or special characters.
        /// Tags are case insensitive. For readability, consider using camelCasing or PascalCasing.
        /// </remarks>
        std::vector<R66::string_holder> Tags;
        /// <summary>
        /// If <see cref="ModifyChannelInfoRequest.Tags" /> is missing or empty, channel tags will not be altered by default (if <see cref="ModifyChannelInfoRequest.ForceUpdateTags" /> is false).
        /// If <see cref="ModifyChannelInfoRequest.ForceUpdateTags" /> is <c>true</c>, passing an empty (or missing) array <see cref="ModifyChannelInfoRequest.Tags" /> causes all tags to be removed from the channel.
        /// </summary>
        bool ForceUpdateTags = false;
    };
    /// @class PredictionDefinition r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Contains the information necessary to create a new prediction.
    /// </summary>
    struct PredictionDefinition {
        /// <summary>
        /// Title for the Prediction.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Array of outcome titles for the Prediction.
        /// Array size must be 2.
        /// The first outcome object is the “blue” outcome and the second outcome object is the “pink” outcome when viewing the Prediction on Twitch.
        /// </summary>
        std::vector<R66::string_holder> Outcomes;
        /// <summary>
        /// Total duration for the Prediction (in seconds).
        /// Minimum: 1. Maximum: 1800.
        /// </summary>
        int32_t Duration = 0;
    };
    /// @class PredictionOutcome r66_structs_autogenerated.hpp r66.hpp
    struct PredictionOutcome {
        /// <summary>
        /// ID for the outcome.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// Text displayed for outcome.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Number of unique users that chose the outcome.
        /// </summary>
        int64_t Users = 0;
        /// <summary>
        /// Number of Channel Points used for the outcome.
        /// </summary>
        int64_t ChannelPoints = 0;
        /// <summary>
        /// Color for the outcome. Valid values: BLUE, PINK
        /// </summary>
        R66::string_holder Color;
    };
    /// @class PredictionInfo r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a Channel Points Predictions on a Twitch channel.
    /// </summary>
    struct PredictionInfo {
        /// <summary>
        /// ID of the Prediction.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// ID of the broadcaster.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// Title for the Prediction.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// ID of the winning outcome. If <see cref="PredictionInfo.Status" /> is <see cref="PredictionStatus.Active" />, this is set to <c>null</c>.
        /// </summary>
        R66::string_holder WinningOutcomeId;
        /// <summary>
        /// Array of possible outcomes for the Prediction.
        /// </summary>
        std::vector<PredictionOutcome> Outcomes;
        /// <summary>
        /// Status of the Prediction.
        /// </summary>
        PredictionStatus Status = PredictionStatus(0);
        /// <summary>
        /// UTC timestamp for the Prediction’s start time.
        /// </summary>
        R66::string_holder CreatedAt;
        /// <summary>
        /// UTC timestamp for when the Prediction ended. If <see cref="PredictionInfo.Status" /> is <see cref="PredictionStatus.Active" />, this is set to <c>null</c>.
        /// </summary>
        R66::string_holder EndedAt;
        /// <summary>
        /// UTC timestamp for when the Prediction was locked. If <see cref="PredictionInfo.Status" /> is <see cref="PredictionStatus.Active" />, this is set to <c>null</c>.
        /// </summary>
        R66::string_holder LockedAt;
    };
    /// @class EndPredictionRequest r66_structs_autogenerated.hpp r66.hpp
    struct EndPredictionRequest {
        /// <summary>
        /// The broadcaster running prediction events.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// ID of the Prediction.
        /// </summary>
        R66::string_holder PredictionId;
        /// <summary>
        /// The Prediction status to be set.
        /// </summary>
        /// <remarks>
        /// Valid values:
        /// <see cref="PredictionStatus.Locked" />,
        /// <see cref="PredictionStatus.Resolved" />,
        /// <see cref="PredictionStatus.Canceled" /></remarks>
        PredictionStatus Status = PredictionStatus(0);
        /// <summary>
        /// ID of the winning outcome for the Prediction, if <see cref="EndPredictionRequest.Status" /> is being set to <see cref="PredictionStatus.Resolved" />.
        /// </summary>
        R66::string_holder WinningOutcomeId;
    };
    /// @class StreamMarkerInfo r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a Stream Marker.
    /// </summary>
    struct StreamMarkerInfo {
        /// <summary>
        /// Unique ID of the marker.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// RFC3339 timestamp of the marker.
        /// </summary>
        R66::string_holder CreatedAt;
        /// <summary>
        /// Description of the marker.
        /// </summary>
        R66::string_holder Description;
        /// <summary>
        /// Relative offset (in seconds) of the marker, from the beginning of the stream.
        /// </summary>
        int64_t PositionSeconds = 0;
    };
    /// @class ClipInfo r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a Clip.
    /// </summary>
    struct ClipInfo {
        /// <summary>
        /// ID of the clip being queried.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// URL where the clip can be viewed.
        /// </summary>
        R66::string_holder Url;
        /// <summary>
        /// URL to embed the clip.
        /// </summary>
        R66::string_holder EmbedUrl;
        /// <summary>
        /// Date when the clip was created.
        /// </summary>
        R66::string_holder CreatedAt;
        /// <summary>
        /// URL of the clip thumbnail.
        /// </summary>
        R66::string_holder ThumbnailUrl;
        /// <summary>
        /// Duration of the Clip in seconds.
        /// </summary>
        int32_t Duration = 0;
    };
    /// @class EndPollRequest r66_structs_autogenerated.hpp r66.hpp
    struct EndPollRequest {
        /// <summary>
        /// ID of the broadcaster.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// ID of the poll.
        /// </summary>
        R66::string_holder PollId;
        /// <summary>
        /// If <c>true</c>, sets the poll status to <see cref="PollStatus.Terminated" /> so that results are shown.
        /// If <c>false</c>, sets the poll status to <see cref="PollStatus.Archived" /> so that the poll is canceled immediately and no results are shown.
        /// </summary>
        bool ShowResults = false;
    };
    /// @class PollChoiceInfo r66_structs_autogenerated.hpp r66.hpp
    struct PollChoiceInfo {
        /// <summary>
        /// ID for the choice.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// Text displayed for the choice.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Total number of votes received for the choice across all methods of voting.
        /// </summary>
        int64_t Votes = 0;
        /// <summary>
        /// Number of votes received via Channel Points.
        /// </summary>
        int64_t ChannelPointsVotes = 0;
    };
    /// @class PollInfo r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a Poll on a Twitch channel.
    /// </summary>
    struct PollInfo {
        /// <summary>
        /// ID of the poll.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// ID of the broadcaster.
        /// </summary>
        R66::string_holder BroadcasterId;
        /// <summary>
        /// Question displayed for the poll.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Array of the poll choices.
        /// </summary>
        std::vector<PollChoiceInfo> Choices;
        /// <summary>
        /// Indicates if Channel Points can be used for voting.
        /// </summary>
        bool ChannelPointsVotingEnabled = false;
        /// <summary>
        /// Number of Channel Points required to vote once with Channel Points.
        /// </summary>
        int32_t ChannelPointsPerVote = 0;
        /// <summary>
        /// Poll status.
        /// </summary>
        PollStatus Status = PollStatus(0);
        /// <summary>
        /// UTC timestamp for the poll’s start time.
        /// </summary>
        R66::string_holder StartedAt;
        /// <summary>
        /// UTC timestamp for the poll’s end time. Set to <c>null</c> if the poll is still <see cref="PollStatus.Active" />.
        /// </summary>
        R66::string_holder EndedAt;
    };
    /// @class StreamQuery r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a query for streams. All fields are optional.
    /// </summary>
    struct StreamQuery {
        /// <summary>
        /// Cursor for forward pagination: tells the server where to start fetching the next set of results,
        /// in a multi-page response. The cursor value specified here is from the <c>PaginationCursor</c> response
        /// field of a prior query.
        /// </summary>
        R66::string_holder After;
        /// <summary>
        /// Maximum number of objects to return. Maximum: 100. Default: 20.
        /// </summary>
        int32_t First = 0;
        /// <summary>
        /// Returns streams broadcasting a specified game ID. You can specify up to 100 IDs.
        /// </summary>
        std::vector<R66::string_holder> GameIds;
        /// <summary>
        /// Stream language. You can specify up to 100 languages. A language value must be either the ISO 639-1 two-letter code for a supported stream language or “other”.
        /// </summary>
        std::vector<R66::string_holder> Languages;
        /// <summary>
        /// Returns streams broadcast by one or more specified user IDs. You can specify up to 100 IDs.
        /// </summary>
        std::vector<R66::string_holder> UserIds;
        /// <summary>
        /// Returns streams broadcast by one or more specified user login names. You can specify up to 100 names.
        /// </summary>
        std::vector<R66::string_holder> UserLogins;
    };
    /// @class WebRequestResult r66_structs_autogenerated.hpp r66.hpp
    struct WebRequestResult {
        int32_t HttpStatus = 0;
        R66::string_holder ResponseBody;
    };
    /// @class AuthenticationInfo r66_structs_autogenerated.hpp r66.hpp
    struct AuthenticationInfo {
        R66::string_holder Uri;
        R66::string_holder UserCode;
    };
    /// @class UserInfo r66_structs_autogenerated.hpp r66.hpp
    struct UserInfo {
        /// <summary>
        /// User’s ID.
        /// </summary>
        R66::string_holder ChannelId;
        /// <summary>
        /// User’s login name.
        /// </summary>
        R66::string_holder LoginName;
        /// <summary>
        /// User’s display name.
        /// </summary>
        R66::string_holder DisplayName;
        /// <summary>
        /// User’s type: "staff", "admin", "global_mod", or "".
        /// </summary>
        R66::string_holder UserType;
        /// <summary>
        /// User’s broadcaster type: "partner", "affiliate", or "".
        /// </summary>
        R66::string_holder BroadcasterType;
        /// <summary>
        /// User’s channel description.
        /// </summary>
        R66::string_holder Description;
        /// <summary>
        /// URL of the user’s profile image.
        /// </summary>
        R66::string_holder ProfileImageUrl;
        /// <summary>
        /// URL of the user’s offline image.
        /// </summary>
        R66::string_holder OfflineImageUrl;
        /// <summary>
        /// Total number of views of the user’s channel.
        /// </summary>
        int64_t ViewCount = 0;
        /// <summary>
        /// Date when the user was created.
        /// </summary>
        R66::string_holder CreatedAt;
        /// <summary>
        /// User’s verified email address. Only available if you have the <c>user:read:email</c> scope.
        /// </summary>
        R66::string_holder Email;
    };
    /// @class StreamInfo r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes an active stream on Twitch.
    /// </summary>
    struct StreamInfo {
        /// <summary>
        /// Stream ID.
        /// </summary>
        R66::string_holder Id;
        /// <summary>
        /// ID of the user who is streaming.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// Login of the user who is streaming.
        /// </summary>
        R66::string_holder UserLogin;
        /// <summary>
        /// Display name of the user who is streaming.
        /// </summary>
        R66::string_holder UserName;
        /// <summary>
        /// ID of the game being played on the stream.
        /// </summary>
        R66::string_holder GameId;
        /// <summary>
        /// Name of the game being played.
        /// </summary>
        R66::string_holder GameName;
        /// <summary>
        /// Stream type: "live" or "" (in case of error).
        /// </summary>
        R66::string_holder Type;
        /// <summary>
        /// Stream title.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Number of viewers watching the stream at the time of the query.
        /// </summary>
        int64_t ViewerCount = 0;
        /// <summary>
        /// UTC timestamp.
        /// </summary>
        R66::string_holder StartedAt;
        /// <summary>
        /// Stream language. A language value is either the ISO 639-1 two-letter code for a supported stream language or “other”.
        /// </summary>
        R66::string_holder Language;
        /// <summary>
        /// Thumbnail URL of the stream. All image URLs have variable width and height. You can replace <c>{width}</c> and <c>{height}</c> with any values to get that size image
        /// </summary>
        R66::string_holder ThumbnailUrl;
        /// <summary>
        /// Shows tag IDs that apply to the stream.
        /// </summary>
        std::vector<R66::string_holder> Tags;
        /// <summary>
        /// Indicates if the broadcaster has specified their channel contains mature content that may be inappropriate for younger audiences.
        /// </summary>
        bool IsMature = false;
    };
    /// @class StreamQueryResult r66_structs_autogenerated.hpp r66.hpp
    struct StreamQueryResult {
        /// <summary>
        /// List of streams matching the query.
        /// </summary>
        std::vector<StreamInfo> Streams;
        /// <summary>
        /// Pagination cursor to fetch the next page of results. May be empty if there are no further results.
        /// </summary>
        R66::string_holder PaginationCursor;
    };
    /// @class ChannelSubscribeEvent r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// A notification when a specified channel receives a subscriber. This does not include resubscribes.
    /// </summary>
    struct ChannelSubscribeEvent {
        /// <summary>
        /// The user ID for the user who subscribed to the specified channel.
        /// </summary>
        R66::string_holder UserId;
        /// <summary>
        /// The user login for the user who subscribed to the specified channel.
        /// </summary>
        R66::string_holder UserLogin;
        /// <summary>
        /// The user display name for the user who subscribed to the specified channel.
        /// </summary>
        R66::string_holder UserDisplayName;
        /// <summary>
        /// The tier of the subscription. Valid values are <c>1000</c>, <c>2000</c>, and <c>3000</c>.
        /// </summary>
        R66::string_holder Tier;
        /// <summary>
        /// Whether the subscription is a gift.
        /// </summary>
        bool IsGift = false;
        /// <summary>
        /// The total number of months the user has been subscribed to the channel.
        /// This is nonzero only for resubscriptions.
        /// </summary>
        int32_t CumulativeMonths = 0;
        /// <summary>
        /// The number of consecutive months the user’s current subscription has been active.
        /// This value is <c>-1</c> if the user has opted out of sharing this information.
        /// This is nonzero only for resubscriptions.
        /// </summary>
        int32_t StreakMonths = 0;
        /// <summary>
        /// The month duration of the subscription.
        /// This is nonzero only for resubscriptions.
        /// </summary>
        int32_t DurationMonths = 0;
    };
    /// @class UserSubscriptionCheckResult r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Describes a user's subscription to a specified channel.
    /// </summary>
    struct UserSubscriptionCheckResult {
        /// <summary>
        /// Subscription tier. 1000 is tier 1, 2000 is tier 2, and 3000 is tier 3.
        /// </summary>
        R66::string_holder Tier;
        /// <summary>
        /// Login of the gifter (if <see cref="UserSubscriptionCheckResult.IsGift" /> is <c>true</c>).
        /// </summary>
        R66::string_holder GifterLogin;
        /// <summary>
        /// Display name of the gifter (if <see cref="UserSubscriptionCheckResult.IsGift" /> is <c>true</c>).
        /// </summary>
        R66::string_holder GifterName;
        /// <summary>
        /// Indicates if the subscription is a gift.
        /// </summary>
        bool IsGift = false;
        /// <summary>
        /// Indicates if the user is subscribed at all.
        /// If this is <c>false</c>, all other fields are empty.
        /// </summary>
        bool IsSubscribed = false;
    };
    /// @class PollDefinition r66_structs_autogenerated.hpp r66.hpp
    /// <summary>
    /// Contains the information necessary to create a new poll.
    /// </summary>
    struct PollDefinition {
        /// <summary>
        /// Question displayed for the poll.
        /// </summary>
        R66::string_holder Title;
        /// <summary>
        /// Array of the poll choices.
        /// </summary>
        std::vector<R66::string_holder> Choices;
        /// <summary>
        /// Total duration for the poll (in seconds).
        /// </summary>
        int64_t Duration = 0;
        /// <summary>
        /// Indicates if Bits can be used for voting.
        /// </summary>
        bool BitsVotingEnabled = false;
        /// <summary>
        /// Number of Bits required to vote once with Bits.
        /// </summary>
        int32_t BitsPerVote = 0;
        /// <summary>
        /// Indicates if Channel Points can be used for voting.
        /// </summary>
        bool ChannelPointsVotingEnabled = false;
        /// <summary>
        /// Number of Channel Points required to vote once with Channel Points.
        /// </summary>
        int32_t ChannelPointsPerVote = 0;
    };
}
