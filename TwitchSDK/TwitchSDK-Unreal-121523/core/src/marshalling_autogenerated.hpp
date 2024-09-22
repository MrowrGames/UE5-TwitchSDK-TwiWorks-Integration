#pragma once
namespace R66::Marshalling {
    class ManagedAuthState;
    class ManagedChannelRaidEvent;
    class ManagedCustomRewardResolveRequest;
    class ManagedHypeTrainContribution;
    class ManagedHypeTrainEvent;
    class ManagedCustomRewardDefinition;
    class ManagedCustomRewardList;
    class ManagedBitsLeaderboardEntry;
    class ManagedBitsLeaderboard;
    class ManagedBitsLeaderboardRequest;
    class ManagedChannelFollowEvent;
    class ManagedChannelCheerEvent;
    class ManagedCustomRewardEvent;
    class ManagedEventStreamRequest;
    class ManagedEventStreamDesc;
    class ManagedModifyChannelInfoRequest;
    class ManagedPredictionDefinition;
    class ManagedPredictionOutcome;
    class ManagedPredictionInfo;
    class ManagedEndPredictionRequest;
    class ManagedStreamMarkerInfo;
    class ManagedClipInfo;
    class ManagedEndPollRequest;
    class ManagedPollChoiceInfo;
    class ManagedPollInfo;
    class ManagedStreamQuery;
    class ManagedWebRequestResult;
    class ManagedNone;
    class ManagedPlainInt;
    class ManagedPlainString;
    class ManagedPlainBool;
    class ManagedAuthenticationInfo;
    class ManagedMarshalException;
    class ManagedUserInfo;
    class ManagedStreamInfo;
    class ManagedStreamQueryResult;
    class ManagedChannelSubscribeEvent;
    class ManagedUserSubscriptionCheckResult;
    class ManagedPollDefinition;
    class ManagedWebRequestRequest;
    class ManagedSleepRequest;
    class ManagedReadFileRequest;
    class ManagedWriteFileRequest;
    class ManagedLogRequest;
    class ManagedCreateWebSocketRequest;
    class ManagedSendWebSocketMessageRequest;
    class ManagedRecvWebSocketMessageRequest;
    class ManagedCloseWebSocketRequest;
    class ManagedAuthState : public Marshallable {
    public:
        AuthStatus Status;
        StringArrayManaged Scopes;
        ManagedAuthState(
            AuthStatus status,
            const std::vector<R66::u16string_holder>& scopes)
            : Marshallable(TYPECODE)
            , Status(status)
            , Scopes(scopes) {}
        ManagedAuthState(const AuthState& x)
            : Marshallable(TYPECODE)
            , Status(x.Status)
            , Scopes(x.Scopes) {}
        operator AuthState() const {
            AuthState v;
            v.Status = Status;
            v.Scopes = Scopes;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 824783168;
    };
    class ManagedChannelRaidEvent : public Marshallable {
    public:
        StringManaged FromBroadcasterId;
        StringManaged FromBroadcasterName;
        StringManaged ToBroadcasterId;
        StringManaged ToBroadcasterName;
        int64_t Viewers;
        ManagedChannelRaidEvent(
            std::u16string_view fromBroadcasterId,
            std::u16string_view fromBroadcasterName,
            std::u16string_view toBroadcasterId,
            std::u16string_view toBroadcasterName,
            int64_t viewers)
            : Marshallable(TYPECODE)
            , FromBroadcasterId(fromBroadcasterId)
            , FromBroadcasterName(fromBroadcasterName)
            , ToBroadcasterId(toBroadcasterId)
            , ToBroadcasterName(toBroadcasterName)
            , Viewers(viewers) {}
        ManagedChannelRaidEvent(const ChannelRaidEvent& x)
            : Marshallable(TYPECODE)
            , FromBroadcasterId(x.FromBroadcasterId)
            , FromBroadcasterName(x.FromBroadcasterName)
            , ToBroadcasterId(x.ToBroadcasterId)
            , ToBroadcasterName(x.ToBroadcasterName)
            , Viewers(x.Viewers) {}
        operator ChannelRaidEvent() const {
            ChannelRaidEvent v;
            v.FromBroadcasterId = FromBroadcasterId;
            v.FromBroadcasterName = FromBroadcasterName;
            v.ToBroadcasterId = ToBroadcasterId;
            v.ToBroadcasterName = ToBroadcasterName;
            v.Viewers = Viewers;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1840358796;
    };
    class ManagedCustomRewardResolveRequest : public Marshallable {
    public:
        StringManaged RedemptionId;
        StringManaged CustomRewardId;
        StringManaged BroadcasterId;
        CustomRewardRedemptionState Resolution;
        ManagedCustomRewardResolveRequest(
            std::u16string_view redemptionId,
            std::u16string_view customRewardId,
            std::u16string_view broadcasterId,
            CustomRewardRedemptionState resolution)
            : Marshallable(TYPECODE)
            , RedemptionId(redemptionId)
            , CustomRewardId(customRewardId)
            , BroadcasterId(broadcasterId)
            , Resolution(resolution) {}
        ManagedCustomRewardResolveRequest(const CustomRewardResolveRequest& x)
            : Marshallable(TYPECODE)
            , RedemptionId(x.RedemptionId)
            , CustomRewardId(x.CustomRewardId)
            , BroadcasterId(x.BroadcasterId)
            , Resolution(x.Resolution) {}
        operator CustomRewardResolveRequest() const {
            CustomRewardResolveRequest v;
            v.RedemptionId = RedemptionId;
            v.CustomRewardId = CustomRewardId;
            v.BroadcasterId = BroadcasterId;
            v.Resolution = Resolution;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1152625179;
    };
    class ManagedHypeTrainContribution : public Marshallable {
    public:
        StringManaged UserId;
        StringManaged UserName;
        HypeTrainContributionType Type;
        int64_t Total;
        ManagedHypeTrainContribution(
            std::u16string_view userId,
            std::u16string_view userName,
            HypeTrainContributionType type,
            int64_t total)
            : Marshallable(TYPECODE)
            , UserId(userId)
            , UserName(userName)
            , Type(type)
            , Total(total) {}
        ManagedHypeTrainContribution(const HypeTrainContribution& x)
            : Marshallable(TYPECODE)
            , UserId(x.UserId)
            , UserName(x.UserName)
            , Type(x.Type)
            , Total(x.Total) {}
        operator HypeTrainContribution() const {
            HypeTrainContribution v;
            v.UserId = UserId;
            v.UserName = UserName;
            v.Type = Type;
            v.Total = Total;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1747825069;
    };
    class ManagedHypeTrainEvent : public Marshallable {
    public:
        StringManaged HypeTrainId;
        StringManaged BroadcasterId;
        StringManaged BroadcasterName;
        int32_t Level;
        int64_t TotalPoints;
        int64_t Progress;
        int64_t Goal;
        StringManaged StartedAt;
        StringManaged ExpiresAt;
        StringManaged EndedAt;
        StringManaged CooldownEndsAt;
        GenericArrayManaged<ManagedHypeTrainContribution> TopContributions;
        ManagedHypeTrainContribution LastContribution;
        ManagedHypeTrainEvent(
            std::u16string_view hypeTrainId,
            std::u16string_view broadcasterId,
            std::u16string_view broadcasterName,
            int32_t level,
            int64_t totalPoints,
            int64_t progress,
            int64_t goal,
            std::u16string_view startedAt,
            std::u16string_view expiresAt,
            std::u16string_view endedAt,
            std::u16string_view cooldownEndsAt,
            const std::vector<HypeTrainContribution>& topContributions,
            HypeTrainContribution lastContribution)
            : Marshallable(TYPECODE)
            , HypeTrainId(hypeTrainId)
            , BroadcasterId(broadcasterId)
            , BroadcasterName(broadcasterName)
            , Level(level)
            , TotalPoints(totalPoints)
            , Progress(progress)
            , Goal(goal)
            , StartedAt(startedAt)
            , ExpiresAt(expiresAt)
            , EndedAt(endedAt)
            , CooldownEndsAt(cooldownEndsAt)
            , TopContributions(topContributions)
            , LastContribution(lastContribution) {}
        ManagedHypeTrainEvent(const HypeTrainEvent& x)
            : Marshallable(TYPECODE)
            , HypeTrainId(x.HypeTrainId)
            , BroadcasterId(x.BroadcasterId)
            , BroadcasterName(x.BroadcasterName)
            , Level(x.Level)
            , TotalPoints(x.TotalPoints)
            , Progress(x.Progress)
            , Goal(x.Goal)
            , StartedAt(x.StartedAt)
            , ExpiresAt(x.ExpiresAt)
            , EndedAt(x.EndedAt)
            , CooldownEndsAt(x.CooldownEndsAt)
            , TopContributions(x.TopContributions)
            , LastContribution(x.LastContribution) {}
        operator HypeTrainEvent() const {
            HypeTrainEvent v;
            v.HypeTrainId = HypeTrainId;
            v.BroadcasterId = BroadcasterId;
            v.BroadcasterName = BroadcasterName;
            v.Level = Level;
            v.TotalPoints = TotalPoints;
            v.Progress = Progress;
            v.Goal = Goal;
            v.StartedAt = StartedAt;
            v.ExpiresAt = ExpiresAt;
            v.EndedAt = EndedAt;
            v.CooldownEndsAt = CooldownEndsAt;
            v.TopContributions = TopContributions;
            v.LastContribution = LastContribution;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1200007443;
    };
    class ManagedCustomRewardDefinition : public Marshallable {
    public:
        StringManaged Title;
        int64_t Cost;
        StringManaged Prompt;
        bool IsEnabled;
        StringManaged BackgroundColor;
        bool IsUserInputRequired;
        bool IsMaxPerStreamEnabled;
        int32_t MaxPerStream;
        bool IsMaxPerUserPerStreamEnabled;
        int32_t MaxPerUserPerStream;
        bool IsGlobalCooldownEnabled;
        int32_t GlobalCooldownSeconds;
        bool ShouldRedemptionsSkipRequestQueue;
        ManagedCustomRewardDefinition(
            std::u16string_view title,
            int64_t cost,
            std::u16string_view prompt,
            bool isEnabled,
            std::u16string_view backgroundColor,
            bool isUserInputRequired,
            bool isMaxPerStreamEnabled,
            int32_t maxPerStream,
            bool isMaxPerUserPerStreamEnabled,
            int32_t maxPerUserPerStream,
            bool isGlobalCooldownEnabled,
            int32_t globalCooldownSeconds,
            bool shouldRedemptionsSkipRequestQueue)
            : Marshallable(TYPECODE)
            , Title(title)
            , Cost(cost)
            , Prompt(prompt)
            , IsEnabled(isEnabled)
            , BackgroundColor(backgroundColor)
            , IsUserInputRequired(isUserInputRequired)
            , IsMaxPerStreamEnabled(isMaxPerStreamEnabled)
            , MaxPerStream(maxPerStream)
            , IsMaxPerUserPerStreamEnabled(isMaxPerUserPerStreamEnabled)
            , MaxPerUserPerStream(maxPerUserPerStream)
            , IsGlobalCooldownEnabled(isGlobalCooldownEnabled)
            , GlobalCooldownSeconds(globalCooldownSeconds)
            , ShouldRedemptionsSkipRequestQueue(shouldRedemptionsSkipRequestQueue) {}
        ManagedCustomRewardDefinition(const CustomRewardDefinition& x)
            : Marshallable(TYPECODE)
            , Title(x.Title)
            , Cost(x.Cost)
            , Prompt(x.Prompt)
            , IsEnabled(x.IsEnabled)
            , BackgroundColor(x.BackgroundColor)
            , IsUserInputRequired(x.IsUserInputRequired)
            , IsMaxPerStreamEnabled(x.IsMaxPerStreamEnabled)
            , MaxPerStream(x.MaxPerStream)
            , IsMaxPerUserPerStreamEnabled(x.IsMaxPerUserPerStreamEnabled)
            , MaxPerUserPerStream(x.MaxPerUserPerStream)
            , IsGlobalCooldownEnabled(x.IsGlobalCooldownEnabled)
            , GlobalCooldownSeconds(x.GlobalCooldownSeconds)
            , ShouldRedemptionsSkipRequestQueue(x.ShouldRedemptionsSkipRequestQueue) {}
        operator CustomRewardDefinition() const {
            CustomRewardDefinition v;
            v.Title = Title;
            v.Cost = Cost;
            v.Prompt = Prompt;
            v.IsEnabled = IsEnabled;
            v.BackgroundColor = BackgroundColor;
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
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1158389922;
    };
    class ManagedCustomRewardList : public Marshallable {
    public:
        GenericArrayManaged<ManagedCustomRewardDefinition> Rewards;
        ManagedCustomRewardList(
            const std::vector<CustomRewardDefinition>& rewards)
            : Marshallable(TYPECODE)
            , Rewards(rewards) {}
        ManagedCustomRewardList(const CustomRewardList& x)
            : Marshallable(TYPECODE)
            , Rewards(x.Rewards) {}
        operator CustomRewardList() const {
            CustomRewardList v;
            v.Rewards = Rewards;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -67112796;
    };
    class ManagedBitsLeaderboardEntry : public Marshallable {
    public:
        StringManaged UserId;
        StringManaged UserName;
        int64_t Rank;
        int64_t Score;
        ManagedBitsLeaderboardEntry(
            std::u16string_view userId,
            std::u16string_view userName,
            int64_t rank,
            int64_t score)
            : Marshallable(TYPECODE)
            , UserId(userId)
            , UserName(userName)
            , Rank(rank)
            , Score(score) {}
        ManagedBitsLeaderboardEntry(const BitsLeaderboardEntry& x)
            : Marshallable(TYPECODE)
            , UserId(x.UserId)
            , UserName(x.UserName)
            , Rank(x.Rank)
            , Score(x.Score) {}
        operator BitsLeaderboardEntry() const {
            BitsLeaderboardEntry v;
            v.UserId = UserId;
            v.UserName = UserName;
            v.Rank = Rank;
            v.Score = Score;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -432329023;
    };
    class ManagedBitsLeaderboard : public Marshallable {
    public:
        GenericArrayManaged<ManagedBitsLeaderboardEntry> Data;
        StringManaged StartedAt;
        StringManaged EndedAt;
        ManagedBitsLeaderboard(
            const std::vector<BitsLeaderboardEntry>& data,
            std::u16string_view startedAt,
            std::u16string_view endedAt)
            : Marshallable(TYPECODE)
            , Data(data)
            , StartedAt(startedAt)
            , EndedAt(endedAt) {}
        ManagedBitsLeaderboard(const BitsLeaderboard& x)
            : Marshallable(TYPECODE)
            , Data(x.Data)
            , StartedAt(x.StartedAt)
            , EndedAt(x.EndedAt) {}
        operator BitsLeaderboard() const {
            BitsLeaderboard v;
            v.Data = Data;
            v.StartedAt = StartedAt;
            v.EndedAt = EndedAt;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1209729142;
    };
    class ManagedBitsLeaderboardRequest : public Marshallable {
    public:
        int32_t Count;
        StringManaged Period;
        StringManaged StartedAt;
        StringManaged UserId;
        ManagedBitsLeaderboardRequest(
            int32_t count,
            std::u16string_view period,
            std::u16string_view startedAt,
            std::u16string_view userId)
            : Marshallable(TYPECODE)
            , Count(count)
            , Period(period)
            , StartedAt(startedAt)
            , UserId(userId) {}
        ManagedBitsLeaderboardRequest(const BitsLeaderboardRequest& x)
            : Marshallable(TYPECODE)
            , Count(x.Count)
            , Period(x.Period)
            , StartedAt(x.StartedAt)
            , UserId(x.UserId) {}
        operator BitsLeaderboardRequest() const {
            BitsLeaderboardRequest v;
            v.Count = Count;
            v.Period = Period;
            v.StartedAt = StartedAt;
            v.UserId = UserId;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 794647764;
    };
    class ManagedChannelFollowEvent : public Marshallable {
    public:
        StringManaged UserId;
        StringManaged UserDisplayName;
        StringManaged FollowedAt;
        ManagedChannelFollowEvent(
            std::u16string_view userId,
            std::u16string_view userDisplayName,
            std::u16string_view followedAt)
            : Marshallable(TYPECODE)
            , UserId(userId)
            , UserDisplayName(userDisplayName)
            , FollowedAt(followedAt) {}
        ManagedChannelFollowEvent(const ChannelFollowEvent& x)
            : Marshallable(TYPECODE)
            , UserId(x.UserId)
            , UserDisplayName(x.UserDisplayName)
            , FollowedAt(x.FollowedAt) {}
        operator ChannelFollowEvent() const {
            ChannelFollowEvent v;
            v.UserId = UserId;
            v.UserDisplayName = UserDisplayName;
            v.FollowedAt = FollowedAt;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -895127529;
    };
    class ManagedChannelCheerEvent : public Marshallable {
    public:
        bool IsAnonymous;
        StringManaged UserId;
        StringManaged UserDisplayName;
        StringManaged Message;
        int64_t Bits;
        ManagedChannelCheerEvent(
            bool isAnonymous,
            std::u16string_view userId,
            std::u16string_view userDisplayName,
            std::u16string_view message,
            int64_t bits)
            : Marshallable(TYPECODE)
            , IsAnonymous(isAnonymous)
            , UserId(userId)
            , UserDisplayName(userDisplayName)
            , Message(message)
            , Bits(bits) {}
        ManagedChannelCheerEvent(const ChannelCheerEvent& x)
            : Marshallable(TYPECODE)
            , IsAnonymous(x.IsAnonymous)
            , UserId(x.UserId)
            , UserDisplayName(x.UserDisplayName)
            , Message(x.Message)
            , Bits(x.Bits) {}
        operator ChannelCheerEvent() const {
            ChannelCheerEvent v;
            v.IsAnonymous = IsAnonymous;
            v.UserId = UserId;
            v.UserDisplayName = UserDisplayName;
            v.Message = Message;
            v.Bits = Bits;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1586617571;
    };
    class ManagedCustomRewardEvent : public Marshallable {
    public:
        StringManaged RedemptionId;
        StringManaged BroadcasterId;
        StringManaged RedeemerId;
        StringManaged BroadcasterName;
        StringManaged RedeemerName;
        StringManaged UserInput;
        CustomRewardRedemptionState Status;
        StringManaged CustomRewardId;
        StringManaged CustomRewardTitle;
        int64_t CustomRewardCost;
        StringManaged CustomRewardPrompt;
        StringManaged RedeemedAt;
        ManagedCustomRewardEvent(
            std::u16string_view redemptionId,
            std::u16string_view broadcasterId,
            std::u16string_view redeemerId,
            std::u16string_view broadcasterName,
            std::u16string_view redeemerName,
            std::u16string_view userInput,
            CustomRewardRedemptionState status,
            std::u16string_view customRewardId,
            std::u16string_view customRewardTitle,
            int64_t customRewardCost,
            std::u16string_view customRewardPrompt,
            std::u16string_view redeemedAt)
            : Marshallable(TYPECODE)
            , RedemptionId(redemptionId)
            , BroadcasterId(broadcasterId)
            , RedeemerId(redeemerId)
            , BroadcasterName(broadcasterName)
            , RedeemerName(redeemerName)
            , UserInput(userInput)
            , Status(status)
            , CustomRewardId(customRewardId)
            , CustomRewardTitle(customRewardTitle)
            , CustomRewardCost(customRewardCost)
            , CustomRewardPrompt(customRewardPrompt)
            , RedeemedAt(redeemedAt) {}
        ManagedCustomRewardEvent(const CustomRewardEvent& x)
            : Marshallable(TYPECODE)
            , RedemptionId(x.RedemptionId)
            , BroadcasterId(x.BroadcasterId)
            , RedeemerId(x.RedeemerId)
            , BroadcasterName(x.BroadcasterName)
            , RedeemerName(x.RedeemerName)
            , UserInput(x.UserInput)
            , Status(x.Status)
            , CustomRewardId(x.CustomRewardId)
            , CustomRewardTitle(x.CustomRewardTitle)
            , CustomRewardCost(x.CustomRewardCost)
            , CustomRewardPrompt(x.CustomRewardPrompt)
            , RedeemedAt(x.RedeemedAt) {}
        operator CustomRewardEvent() const {
            CustomRewardEvent v;
            v.RedemptionId = RedemptionId;
            v.BroadcasterId = BroadcasterId;
            v.RedeemerId = RedeemerId;
            v.BroadcasterName = BroadcasterName;
            v.RedeemerName = RedeemerName;
            v.UserInput = UserInput;
            v.Status = Status;
            v.CustomRewardId = CustomRewardId;
            v.CustomRewardTitle = CustomRewardTitle;
            v.CustomRewardCost = CustomRewardCost;
            v.CustomRewardPrompt = CustomRewardPrompt;
            v.RedeemedAt = RedeemedAt;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -800396378;
    };
    class ManagedEventStreamRequest : public Marshallable {
    public:
        EventStreamKind Kind;
        ManagedEventStreamRequest(
            EventStreamKind kind)
            : Marshallable(TYPECODE)
            , Kind(kind) {}
        ManagedEventStreamRequest(const EventStreamRequest& x)
            : Marshallable(TYPECODE)
            , Kind(x.Kind) {}
        operator EventStreamRequest() const {
            EventStreamRequest v;
            v.Kind = Kind;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 973764268;
    };
    class ManagedEventStreamDesc : public Marshallable {
    public:
        EventStreamKind Kind;
        int32_t Token;
        ManagedEventStreamDesc(
            EventStreamKind kind,
            int32_t token)
            : Marshallable(TYPECODE)
            , Kind(kind)
            , Token(token) {}
        ManagedEventStreamDesc(const EventStreamDesc& x)
            : Marshallable(TYPECODE)
            , Kind(x.Kind)
            , Token(x.Token) {}
        operator EventStreamDesc() const {
            EventStreamDesc v;
            v.Kind = Kind;
            v.Token = Token;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 853853815;
    };
    class ManagedModifyChannelInfoRequest : public Marshallable {
    public:
        StringManaged GameId;
        StringManaged Language;
        StringManaged Title;
        int32_t Delay;
        StringArrayManaged Tags;
        bool ForceUpdateTags;
        ManagedModifyChannelInfoRequest(
            std::u16string_view gameId,
            std::u16string_view language,
            std::u16string_view title,
            int32_t delay,
            const std::vector<R66::u16string_holder>& tags,
            bool forceUpdateTags)
            : Marshallable(TYPECODE)
            , GameId(gameId)
            , Language(language)
            , Title(title)
            , Delay(delay)
            , Tags(tags)
            , ForceUpdateTags(forceUpdateTags) {}
        ManagedModifyChannelInfoRequest(const ModifyChannelInfoRequest& x)
            : Marshallable(TYPECODE)
            , GameId(x.GameId)
            , Language(x.Language)
            , Title(x.Title)
            , Delay(x.Delay)
            , Tags(x.Tags)
            , ForceUpdateTags(x.ForceUpdateTags) {}
        operator ModifyChannelInfoRequest() const {
            ModifyChannelInfoRequest v;
            v.GameId = GameId;
            v.Language = Language;
            v.Title = Title;
            v.Delay = Delay;
            v.Tags = Tags;
            v.ForceUpdateTags = ForceUpdateTags;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1414041357;
    };
    class ManagedPredictionDefinition : public Marshallable {
    public:
        StringManaged Title;
        StringArrayManaged Outcomes;
        int32_t Duration;
        ManagedPredictionDefinition(
            std::u16string_view title,
            const std::vector<R66::u16string_holder>& outcomes,
            int32_t duration)
            : Marshallable(TYPECODE)
            , Title(title)
            , Outcomes(outcomes)
            , Duration(duration) {}
        ManagedPredictionDefinition(const PredictionDefinition& x)
            : Marshallable(TYPECODE)
            , Title(x.Title)
            , Outcomes(x.Outcomes)
            , Duration(x.Duration) {}
        operator PredictionDefinition() const {
            PredictionDefinition v;
            v.Title = Title;
            v.Outcomes = Outcomes;
            v.Duration = Duration;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -354228052;
    };
    class ManagedPredictionOutcome : public Marshallable {
    public:
        StringManaged Id;
        StringManaged Title;
        int64_t Users;
        int64_t ChannelPoints;
        StringManaged Color;
        ManagedPredictionOutcome(
            std::u16string_view id,
            std::u16string_view title,
            int64_t users,
            int64_t channelPoints,
            std::u16string_view color)
            : Marshallable(TYPECODE)
            , Id(id)
            , Title(title)
            , Users(users)
            , ChannelPoints(channelPoints)
            , Color(color) {}
        ManagedPredictionOutcome(const PredictionOutcome& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , Title(x.Title)
            , Users(x.Users)
            , ChannelPoints(x.ChannelPoints)
            , Color(x.Color) {}
        operator PredictionOutcome() const {
            PredictionOutcome v;
            v.Id = Id;
            v.Title = Title;
            v.Users = Users;
            v.ChannelPoints = ChannelPoints;
            v.Color = Color;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 2038549629;
    };
    class ManagedPredictionInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged BroadcasterId;
        StringManaged Title;
        StringManaged WinningOutcomeId;
        GenericArrayManaged<ManagedPredictionOutcome> Outcomes;
        PredictionStatus Status;
        StringManaged CreatedAt;
        StringManaged EndedAt;
        StringManaged LockedAt;
        ManagedPredictionInfo(
            std::u16string_view id,
            std::u16string_view broadcasterId,
            std::u16string_view title,
            std::u16string_view winningOutcomeId,
            const std::vector<PredictionOutcome>& outcomes,
            PredictionStatus status,
            std::u16string_view createdAt,
            std::u16string_view endedAt,
            std::u16string_view lockedAt)
            : Marshallable(TYPECODE)
            , Id(id)
            , BroadcasterId(broadcasterId)
            , Title(title)
            , WinningOutcomeId(winningOutcomeId)
            , Outcomes(outcomes)
            , Status(status)
            , CreatedAt(createdAt)
            , EndedAt(endedAt)
            , LockedAt(lockedAt) {}
        ManagedPredictionInfo(const PredictionInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , BroadcasterId(x.BroadcasterId)
            , Title(x.Title)
            , WinningOutcomeId(x.WinningOutcomeId)
            , Outcomes(x.Outcomes)
            , Status(x.Status)
            , CreatedAt(x.CreatedAt)
            , EndedAt(x.EndedAt)
            , LockedAt(x.LockedAt) {}
        operator PredictionInfo() const {
            PredictionInfo v;
            v.Id = Id;
            v.BroadcasterId = BroadcasterId;
            v.Title = Title;
            v.WinningOutcomeId = WinningOutcomeId;
            v.Outcomes = Outcomes;
            v.Status = Status;
            v.CreatedAt = CreatedAt;
            v.EndedAt = EndedAt;
            v.LockedAt = LockedAt;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1418330344;
    };
    class ManagedEndPredictionRequest : public Marshallable {
    public:
        StringManaged BroadcasterId;
        StringManaged PredictionId;
        PredictionStatus Status;
        StringManaged WinningOutcomeId;
        ManagedEndPredictionRequest(
            std::u16string_view broadcasterId,
            std::u16string_view predictionId,
            PredictionStatus status,
            std::u16string_view winningOutcomeId)
            : Marshallable(TYPECODE)
            , BroadcasterId(broadcasterId)
            , PredictionId(predictionId)
            , Status(status)
            , WinningOutcomeId(winningOutcomeId) {}
        ManagedEndPredictionRequest(const EndPredictionRequest& x)
            : Marshallable(TYPECODE)
            , BroadcasterId(x.BroadcasterId)
            , PredictionId(x.PredictionId)
            , Status(x.Status)
            , WinningOutcomeId(x.WinningOutcomeId) {}
        operator EndPredictionRequest() const {
            EndPredictionRequest v;
            v.BroadcasterId = BroadcasterId;
            v.PredictionId = PredictionId;
            v.Status = Status;
            v.WinningOutcomeId = WinningOutcomeId;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -359250458;
    };
    class ManagedStreamMarkerInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged CreatedAt;
        StringManaged Description;
        int64_t PositionSeconds;
        ManagedStreamMarkerInfo(
            std::u16string_view id,
            std::u16string_view createdAt,
            std::u16string_view description,
            int64_t positionSeconds)
            : Marshallable(TYPECODE)
            , Id(id)
            , CreatedAt(createdAt)
            , Description(description)
            , PositionSeconds(positionSeconds) {}
        ManagedStreamMarkerInfo(const StreamMarkerInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , CreatedAt(x.CreatedAt)
            , Description(x.Description)
            , PositionSeconds(x.PositionSeconds) {}
        operator StreamMarkerInfo() const {
            StreamMarkerInfo v;
            v.Id = Id;
            v.CreatedAt = CreatedAt;
            v.Description = Description;
            v.PositionSeconds = PositionSeconds;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 25655671;
    };
    class ManagedClipInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged Url;
        StringManaged EmbedUrl;
        StringManaged CreatedAt;
        StringManaged ThumbnailUrl;
        int32_t Duration;
        ManagedClipInfo(
            std::u16string_view id,
            std::u16string_view url,
            std::u16string_view embedUrl,
            std::u16string_view createdAt,
            std::u16string_view thumbnailUrl,
            int32_t duration)
            : Marshallable(TYPECODE)
            , Id(id)
            , Url(url)
            , EmbedUrl(embedUrl)
            , CreatedAt(createdAt)
            , ThumbnailUrl(thumbnailUrl)
            , Duration(duration) {}
        ManagedClipInfo(const ClipInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , Url(x.Url)
            , EmbedUrl(x.EmbedUrl)
            , CreatedAt(x.CreatedAt)
            , ThumbnailUrl(x.ThumbnailUrl)
            , Duration(x.Duration) {}
        operator ClipInfo() const {
            ClipInfo v;
            v.Id = Id;
            v.Url = Url;
            v.EmbedUrl = EmbedUrl;
            v.CreatedAt = CreatedAt;
            v.ThumbnailUrl = ThumbnailUrl;
            v.Duration = Duration;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 374164541;
    };
    class ManagedEndPollRequest : public Marshallable {
    public:
        StringManaged BroadcasterId;
        StringManaged PollId;
        bool ShowResults;
        ManagedEndPollRequest(
            std::u16string_view broadcasterId,
            std::u16string_view pollId,
            bool showResults)
            : Marshallable(TYPECODE)
            , BroadcasterId(broadcasterId)
            , PollId(pollId)
            , ShowResults(showResults) {}
        ManagedEndPollRequest(const EndPollRequest& x)
            : Marshallable(TYPECODE)
            , BroadcasterId(x.BroadcasterId)
            , PollId(x.PollId)
            , ShowResults(x.ShowResults) {}
        operator EndPollRequest() const {
            EndPollRequest v;
            v.BroadcasterId = BroadcasterId;
            v.PollId = PollId;
            v.ShowResults = ShowResults;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1225210291;
    };
    class ManagedPollChoiceInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged Title;
        int64_t Votes;
        int64_t ChannelPointsVotes;
        ManagedPollChoiceInfo(
            std::u16string_view id,
            std::u16string_view title,
            int64_t votes,
            int64_t channelPointsVotes)
            : Marshallable(TYPECODE)
            , Id(id)
            , Title(title)
            , Votes(votes)
            , ChannelPointsVotes(channelPointsVotes) {}
        ManagedPollChoiceInfo(const PollChoiceInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , Title(x.Title)
            , Votes(x.Votes)
            , ChannelPointsVotes(x.ChannelPointsVotes) {}
        operator PollChoiceInfo() const {
            PollChoiceInfo v;
            v.Id = Id;
            v.Title = Title;
            v.Votes = Votes;
            v.ChannelPointsVotes = ChannelPointsVotes;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1335005312;
    };
    class ManagedPollInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged BroadcasterId;
        StringManaged Title;
        GenericArrayManaged<ManagedPollChoiceInfo> Choices;
        bool ChannelPointsVotingEnabled;
        int32_t ChannelPointsPerVote;
        PollStatus Status;
        StringManaged StartedAt;
        StringManaged EndedAt;
        ManagedPollInfo(
            std::u16string_view id,
            std::u16string_view broadcasterId,
            std::u16string_view title,
            const std::vector<PollChoiceInfo>& choices,
            bool channelPointsVotingEnabled,
            int32_t channelPointsPerVote,
            PollStatus status,
            std::u16string_view startedAt,
            std::u16string_view endedAt)
            : Marshallable(TYPECODE)
            , Id(id)
            , BroadcasterId(broadcasterId)
            , Title(title)
            , Choices(choices)
            , ChannelPointsVotingEnabled(channelPointsVotingEnabled)
            , ChannelPointsPerVote(channelPointsPerVote)
            , Status(status)
            , StartedAt(startedAt)
            , EndedAt(endedAt) {}
        ManagedPollInfo(const PollInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , BroadcasterId(x.BroadcasterId)
            , Title(x.Title)
            , Choices(x.Choices)
            , ChannelPointsVotingEnabled(x.ChannelPointsVotingEnabled)
            , ChannelPointsPerVote(x.ChannelPointsPerVote)
            , Status(x.Status)
            , StartedAt(x.StartedAt)
            , EndedAt(x.EndedAt) {}
        operator PollInfo() const {
            PollInfo v;
            v.Id = Id;
            v.BroadcasterId = BroadcasterId;
            v.Title = Title;
            v.Choices = Choices;
            v.ChannelPointsVotingEnabled = ChannelPointsVotingEnabled;
            v.ChannelPointsPerVote = ChannelPointsPerVote;
            v.Status = Status;
            v.StartedAt = StartedAt;
            v.EndedAt = EndedAt;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 41019527;
    };
    class ManagedStreamQuery : public Marshallable {
    public:
        StringManaged After;
        int32_t First;
        StringArrayManaged GameIds;
        StringArrayManaged Languages;
        StringArrayManaged UserIds;
        StringArrayManaged UserLogins;
        ManagedStreamQuery(
            std::u16string_view after,
            int32_t first,
            const std::vector<R66::u16string_holder>& gameIds,
            const std::vector<R66::u16string_holder>& languages,
            const std::vector<R66::u16string_holder>& userIds,
            const std::vector<R66::u16string_holder>& userLogins)
            : Marshallable(TYPECODE)
            , After(after)
            , First(first)
            , GameIds(gameIds)
            , Languages(languages)
            , UserIds(userIds)
            , UserLogins(userLogins) {}
        ManagedStreamQuery(const StreamQuery& x)
            : Marshallable(TYPECODE)
            , After(x.After)
            , First(x.First)
            , GameIds(x.GameIds)
            , Languages(x.Languages)
            , UserIds(x.UserIds)
            , UserLogins(x.UserLogins) {}
        operator StreamQuery() const {
            StreamQuery v;
            v.After = After;
            v.First = First;
            v.GameIds = GameIds;
            v.Languages = Languages;
            v.UserIds = UserIds;
            v.UserLogins = UserLogins;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1548732846;
    };
    class ManagedWebRequestResult : public Marshallable {
    public:
        int32_t HttpStatus;
        StringManaged ResponseBody;
        ManagedWebRequestResult(
            int32_t httpStatus,
            std::u16string_view responseBody)
            : Marshallable(TYPECODE)
            , HttpStatus(httpStatus)
            , ResponseBody(responseBody) {}
        ManagedWebRequestResult(const WebRequestResult& x)
            : Marshallable(TYPECODE)
            , HttpStatus(x.HttpStatus)
            , ResponseBody(x.ResponseBody) {}
        operator WebRequestResult() const {
            WebRequestResult v;
            v.HttpStatus = HttpStatus;
            v.ResponseBody = ResponseBody;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 706061000;
    };
    class ManagedNone : public Marshallable {
    public:
        ManagedNone()
            : Marshallable(TYPECODE) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1503227594;
    };
    class ManagedPlainInt : public Marshallable {
    public:
        int32_t Data;
        ManagedPlainInt(
            int32_t data)
            : Marshallable(TYPECODE)
            , Data(data) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1549817923;
    };
    class ManagedPlainString : public Marshallable {
    public:
        StringManaged Data;
        ManagedPlainString(
            std::u16string_view data)
            : Marshallable(TYPECODE)
            , Data(data) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 988587891;
    };
    class ManagedPlainBool : public Marshallable {
    public:
        bool Data;
        ManagedPlainBool(
            bool data)
            : Marshallable(TYPECODE)
            , Data(data) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 288933883;
    };
    class ManagedAuthenticationInfo : public Marshallable {
    public:
        StringManaged Uri;
        StringManaged UserCode;
        ManagedAuthenticationInfo(
            std::u16string_view uri,
            std::u16string_view userCode)
            : Marshallable(TYPECODE)
            , Uri(uri)
            , UserCode(userCode) {}
        ManagedAuthenticationInfo(const AuthenticationInfo& x)
            : Marshallable(TYPECODE)
            , Uri(x.Uri)
            , UserCode(x.UserCode) {}
        operator AuthenticationInfo() const {
            AuthenticationInfo v;
            v.Uri = Uri;
            v.UserCode = UserCode;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -256913275;
    };
    class ManagedMarshalException : public Marshallable {
    public:
        StringManaged What;
        ManagedMarshalException(
            std::u16string_view what)
            : Marshallable(TYPECODE)
            , What(what) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1875581513;
    };
    class ManagedUserInfo : public Marshallable {
    public:
        StringManaged ChannelId;
        StringManaged LoginName;
        StringManaged DisplayName;
        StringManaged UserType;
        StringManaged BroadcasterType;
        StringManaged Description;
        StringManaged ProfileImageUrl;
        StringManaged OfflineImageUrl;
        int64_t ViewCount;
        StringManaged CreatedAt;
        StringManaged Email;
        ManagedUserInfo(
            std::u16string_view channelId,
            std::u16string_view loginName,
            std::u16string_view displayName,
            std::u16string_view userType,
            std::u16string_view broadcasterType,
            std::u16string_view description,
            std::u16string_view profileImageUrl,
            std::u16string_view offlineImageUrl,
            int64_t viewCount,
            std::u16string_view createdAt,
            std::u16string_view email)
            : Marshallable(TYPECODE)
            , ChannelId(channelId)
            , LoginName(loginName)
            , DisplayName(displayName)
            , UserType(userType)
            , BroadcasterType(broadcasterType)
            , Description(description)
            , ProfileImageUrl(profileImageUrl)
            , OfflineImageUrl(offlineImageUrl)
            , ViewCount(viewCount)
            , CreatedAt(createdAt)
            , Email(email) {}
        ManagedUserInfo(const UserInfo& x)
            : Marshallable(TYPECODE)
            , ChannelId(x.ChannelId)
            , LoginName(x.LoginName)
            , DisplayName(x.DisplayName)
            , UserType(x.UserType)
            , BroadcasterType(x.BroadcasterType)
            , Description(x.Description)
            , ProfileImageUrl(x.ProfileImageUrl)
            , OfflineImageUrl(x.OfflineImageUrl)
            , ViewCount(x.ViewCount)
            , CreatedAt(x.CreatedAt)
            , Email(x.Email) {}
        operator UserInfo() const {
            UserInfo v;
            v.ChannelId = ChannelId;
            v.LoginName = LoginName;
            v.DisplayName = DisplayName;
            v.UserType = UserType;
            v.BroadcasterType = BroadcasterType;
            v.Description = Description;
            v.ProfileImageUrl = ProfileImageUrl;
            v.OfflineImageUrl = OfflineImageUrl;
            v.ViewCount = ViewCount;
            v.CreatedAt = CreatedAt;
            v.Email = Email;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1969979977;
    };
    class ManagedStreamInfo : public Marshallable {
    public:
        StringManaged Id;
        StringManaged UserId;
        StringManaged UserLogin;
        StringManaged UserName;
        StringManaged GameId;
        StringManaged GameName;
        StringManaged Type;
        StringManaged Title;
        int64_t ViewerCount;
        StringManaged StartedAt;
        StringManaged Language;
        StringManaged ThumbnailUrl;
        StringArrayManaged Tags;
        bool IsMature;
        ManagedStreamInfo(
            std::u16string_view id,
            std::u16string_view userId,
            std::u16string_view userLogin,
            std::u16string_view userName,
            std::u16string_view gameId,
            std::u16string_view gameName,
            std::u16string_view type,
            std::u16string_view title,
            int64_t viewerCount,
            std::u16string_view startedAt,
            std::u16string_view language,
            std::u16string_view thumbnailUrl,
            const std::vector<R66::u16string_holder>& tags,
            bool isMature)
            : Marshallable(TYPECODE)
            , Id(id)
            , UserId(userId)
            , UserLogin(userLogin)
            , UserName(userName)
            , GameId(gameId)
            , GameName(gameName)
            , Type(type)
            , Title(title)
            , ViewerCount(viewerCount)
            , StartedAt(startedAt)
            , Language(language)
            , ThumbnailUrl(thumbnailUrl)
            , Tags(tags)
            , IsMature(isMature) {}
        ManagedStreamInfo(const StreamInfo& x)
            : Marshallable(TYPECODE)
            , Id(x.Id)
            , UserId(x.UserId)
            , UserLogin(x.UserLogin)
            , UserName(x.UserName)
            , GameId(x.GameId)
            , GameName(x.GameName)
            , Type(x.Type)
            , Title(x.Title)
            , ViewerCount(x.ViewerCount)
            , StartedAt(x.StartedAt)
            , Language(x.Language)
            , ThumbnailUrl(x.ThumbnailUrl)
            , Tags(x.Tags)
            , IsMature(x.IsMature) {}
        operator StreamInfo() const {
            StreamInfo v;
            v.Id = Id;
            v.UserId = UserId;
            v.UserLogin = UserLogin;
            v.UserName = UserName;
            v.GameId = GameId;
            v.GameName = GameName;
            v.Type = Type;
            v.Title = Title;
            v.ViewerCount = ViewerCount;
            v.StartedAt = StartedAt;
            v.Language = Language;
            v.ThumbnailUrl = ThumbnailUrl;
            v.Tags = Tags;
            v.IsMature = IsMature;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -903042935;
    };
    class ManagedStreamQueryResult : public Marshallable {
    public:
        GenericArrayManaged<ManagedStreamInfo> Streams;
        StringManaged PaginationCursor;
        ManagedStreamQueryResult(
            const std::vector<StreamInfo>& streams,
            std::u16string_view paginationCursor)
            : Marshallable(TYPECODE)
            , Streams(streams)
            , PaginationCursor(paginationCursor) {}
        ManagedStreamQueryResult(const StreamQueryResult& x)
            : Marshallable(TYPECODE)
            , Streams(x.Streams)
            , PaginationCursor(x.PaginationCursor) {}
        operator StreamQueryResult() const {
            StreamQueryResult v;
            v.Streams = Streams;
            v.PaginationCursor = PaginationCursor;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1775991607;
    };
    class ManagedChannelSubscribeEvent : public Marshallable {
    public:
        StringManaged UserId;
        StringManaged UserLogin;
        StringManaged UserDisplayName;
        StringManaged Tier;
        bool IsGift;
        int32_t CumulativeMonths;
        int32_t StreakMonths;
        int32_t DurationMonths;
        ManagedChannelSubscribeEvent(
            std::u16string_view userId,
            std::u16string_view userLogin,
            std::u16string_view userDisplayName,
            std::u16string_view tier,
            bool isGift,
            int32_t cumulativeMonths,
            int32_t streakMonths,
            int32_t durationMonths)
            : Marshallable(TYPECODE)
            , UserId(userId)
            , UserLogin(userLogin)
            , UserDisplayName(userDisplayName)
            , Tier(tier)
            , IsGift(isGift)
            , CumulativeMonths(cumulativeMonths)
            , StreakMonths(streakMonths)
            , DurationMonths(durationMonths) {}
        ManagedChannelSubscribeEvent(const ChannelSubscribeEvent& x)
            : Marshallable(TYPECODE)
            , UserId(x.UserId)
            , UserLogin(x.UserLogin)
            , UserDisplayName(x.UserDisplayName)
            , Tier(x.Tier)
            , IsGift(x.IsGift)
            , CumulativeMonths(x.CumulativeMonths)
            , StreakMonths(x.StreakMonths)
            , DurationMonths(x.DurationMonths) {}
        operator ChannelSubscribeEvent() const {
            ChannelSubscribeEvent v;
            v.UserId = UserId;
            v.UserLogin = UserLogin;
            v.UserDisplayName = UserDisplayName;
            v.Tier = Tier;
            v.IsGift = IsGift;
            v.CumulativeMonths = CumulativeMonths;
            v.StreakMonths = StreakMonths;
            v.DurationMonths = DurationMonths;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1666538230;
    };
    class ManagedUserSubscriptionCheckResult : public Marshallable {
    public:
        StringManaged Tier;
        StringManaged GifterLogin;
        StringManaged GifterName;
        bool IsGift;
        bool IsSubscribed;
        ManagedUserSubscriptionCheckResult(
            std::u16string_view tier,
            std::u16string_view gifterLogin,
            std::u16string_view gifterName,
            bool isGift,
            bool isSubscribed)
            : Marshallable(TYPECODE)
            , Tier(tier)
            , GifterLogin(gifterLogin)
            , GifterName(gifterName)
            , IsGift(isGift)
            , IsSubscribed(isSubscribed) {}
        ManagedUserSubscriptionCheckResult(const UserSubscriptionCheckResult& x)
            : Marshallable(TYPECODE)
            , Tier(x.Tier)
            , GifterLogin(x.GifterLogin)
            , GifterName(x.GifterName)
            , IsGift(x.IsGift)
            , IsSubscribed(x.IsSubscribed) {}
        operator UserSubscriptionCheckResult() const {
            UserSubscriptionCheckResult v;
            v.Tier = Tier;
            v.GifterLogin = GifterLogin;
            v.GifterName = GifterName;
            v.IsGift = IsGift;
            v.IsSubscribed = IsSubscribed;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1536148643;
    };
    class ManagedPollDefinition : public Marshallable {
    public:
        StringManaged Title;
        StringArrayManaged Choices;
        int64_t Duration;
        bool BitsVotingEnabled;
        int32_t BitsPerVote;
        bool ChannelPointsVotingEnabled;
        int32_t ChannelPointsPerVote;
        ManagedPollDefinition(
            std::u16string_view title,
            const std::vector<R66::u16string_holder>& choices,
            int64_t duration,
            bool bitsVotingEnabled,
            int32_t bitsPerVote,
            bool channelPointsVotingEnabled,
            int32_t channelPointsPerVote)
            : Marshallable(TYPECODE)
            , Title(title)
            , Choices(choices)
            , Duration(duration)
            , BitsVotingEnabled(bitsVotingEnabled)
            , BitsPerVote(bitsPerVote)
            , ChannelPointsVotingEnabled(channelPointsVotingEnabled)
            , ChannelPointsPerVote(channelPointsPerVote) {}
        ManagedPollDefinition(const PollDefinition& x)
            : Marshallable(TYPECODE)
            , Title(x.Title)
            , Choices(x.Choices)
            , Duration(x.Duration)
            , BitsVotingEnabled(x.BitsVotingEnabled)
            , BitsPerVote(x.BitsPerVote)
            , ChannelPointsVotingEnabled(x.ChannelPointsVotingEnabled)
            , ChannelPointsPerVote(x.ChannelPointsPerVote) {}
        operator PollDefinition() const {
            PollDefinition v;
            v.Title = Title;
            v.Choices = Choices;
            v.Duration = Duration;
            v.BitsVotingEnabled = BitsVotingEnabled;
            v.BitsPerVote = BitsPerVote;
            v.ChannelPointsVotingEnabled = ChannelPointsVotingEnabled;
            v.ChannelPointsPerVote = ChannelPointsPerVote;
            return v;
        }
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1429450289;
    };
    class ManagedWebRequestRequest : public MarshallableStartAsync<ManagedWebRequestResult> {
    public:
        HttpMethod Method;
        StringManaged Uri;
        StringManaged ContentType;
        StringManaged ClientId;
        StringManaged Authorization;
        StringManaged RequestBody;
        ManagedWebRequestRequest(
            HttpMethod method,
            std::u16string_view uri,
            std::u16string_view contentType,
            std::u16string_view clientId,
            std::u16string_view authorization,
            std::u16string_view requestBody,
            std::function<void(const ManagedWebRequestResult&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Method(method)
            , Uri(uri)
            , ContentType(contentType)
            , ClientId(clientId)
            , Authorization(authorization)
            , RequestBody(requestBody) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1855527540;
    };
    class ManagedSleepRequest : public MarshallableStartAsync<ManagedNone> {
    public:
        int32_t Milliseconds;
        ManagedSleepRequest(
            int32_t milliseconds,
            std::function<void(const ManagedNone&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Milliseconds(milliseconds) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -504035953;
    };
    class ManagedReadFileRequest : public MarshallableStartAsync<ManagedPlainString> {
    public:
        StringManaged Path;
        ManagedReadFileRequest(
            std::u16string_view path,
            std::function<void(const ManagedPlainString&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Path(path) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1182308723;
    };
    class ManagedWriteFileRequest : public MarshallableStartAsync<ManagedNone> {
    public:
        StringManaged Path;
        StringManaged Data;
        ManagedWriteFileRequest(
            std::u16string_view path,
            std::u16string_view data,
            std::function<void(const ManagedNone&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Path(path)
            , Data(data) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1420421173;
    };
    class ManagedLogRequest : public MarshallableStartAsync<ManagedNone> {
    public:
        LogLevel Level;
        StringManaged Message;
        ManagedLogRequest(
            LogLevel level,
            std::u16string_view message,
            std::function<void(const ManagedNone&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Level(level)
            , Message(message) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1210189489;
    };
    class ManagedCreateWebSocketRequest : public MarshallableStartAsync<ManagedPlainInt> {
    public:
        StringManaged Url;
        StringManaged Protocol;
        ManagedCreateWebSocketRequest(
            std::u16string_view url,
            std::u16string_view protocol,
            std::function<void(const ManagedPlainInt&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Url(url)
            , Protocol(protocol) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -1487905959;
    };
    class ManagedSendWebSocketMessageRequest : public MarshallableStartAsync<ManagedNone> {
    public:
        int32_t Handle;
        StringManaged Message;
        ManagedSendWebSocketMessageRequest(
            int32_t handle,
            std::u16string_view message,
            std::function<void(const ManagedNone&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Handle(handle)
            , Message(message) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -223523082;
    };
    class ManagedRecvWebSocketMessageRequest : public MarshallableStartAsync<ManagedPlainString> {
    public:
        int32_t Handle;
        int32_t TimeoutSeconds;
        ManagedRecvWebSocketMessageRequest(
            int32_t handle,
            int32_t timeoutSeconds,
            std::function<void(const ManagedPlainString&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Handle(handle)
            , TimeoutSeconds(timeoutSeconds) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = 1869071618;
    };
    class ManagedCloseWebSocketRequest : public MarshallableStartAsync<ManagedNone> {
    public:
        int32_t Handle;
        ManagedCloseWebSocketRequest(
            int32_t handle,
            std::function<void(const ManagedNone&)> cb,
            std::function<void(const std::exception&)> rej)
            : MarshallableStartAsync(TYPECODE, cb, rej)
            , Handle(handle) {}
        bool IsValid() const { return TypeCode == TYPECODE; }
        static const int32_t TYPECODE = -2145718554;
    };
}
