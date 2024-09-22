#pragma once

#include <cstdio>
#include <functional>
#include <cstdarg>
#include <string_view>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <utility>
#include <deque>

using namespace std::literals;

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using SteadyClock = std::chrono::steady_clock;

#include "r66.hpp"
#include "r66_json.hpp"
#include "r66_tasks.hpp"
#include "pal.hpp"
#include "r66_websockets.hpp"
#include "urlencode.hpp"

using namespace R66;
using namespace R66::PAL;


struct LoggedInApiCall {
	HttpMethod Method = HttpMethod::Get;
	string Uri;
	string ContentType;
	string RequestBody;
	int RetryCount = 0;
	bool IsReauthenticated = false;

	LoggedInApiCall() = default;
	LoggedInApiCall(string uri) : Uri(uri) {}
	LoggedInApiCall(string uri, string json, HttpMethod method = HttpMethod::Post)
		: Method(method), Uri(uri), ContentType(STR("application/json"s)), RequestBody(json) {}
};

struct AuthenticationInfoInternal {
	AuthenticationInfo Inner;
	string DeviceCode;
	int Interval = 0;
};

struct AuthorizationResult {
	string AccessToken;
	string RefreshToken;
	std::vector<string> Scopes;

	string AuthorizationHeader() const {
		return STR("Bearer ") + AccessToken;
	}
};

struct ESSubscription {
	string Type;
	string Version;
	std::string FilterConditionName;
	string SocketToken; // the websocket where this subscription is active (if any)
	string ActiveSubId; // the id twitch gave to us that we can use to delete this subscription
	uint32_t RefCount = 0; // the number of active downstream subscriptions
public:
	ESSubscription(string type, string version, std::string filter)
		: Type(type), Version(version), FilterConditionName(filter) {}

	using MapKey = std::pair<string, std::string>;
	MapKey Key() const {
		return std::make_pair(Type, FilterConditionName);
	}
};

template<class T> class PollPredictionStatus {
	FallibleTask<T> Task;
	TimePoint LastUpdate;

public:
	FallibleTask<T>& GetTask() {
		LastUpdate = SteadyClock::now();
		return Task;
	}
	using TMap = std::map<string, PollPredictionStatus>;
	static void Cleanup(TMap& map) {
		// magic number: ten polls is the maximum we keep cached to avoid memory leaks
		while (map.size() > 10)
		{
			auto oldest = std::min_element(map.begin(), map.end(), [](const typename TMap::value_type& a, const typename TMap::value_type& b) { return a.second.LastUpdate < b.second.LastUpdate; });
			oldest->second.Task.Reject(std::runtime_error("poll is gone"));
			map.erase(oldest);
		}
	}
};

struct VoidResult {};

struct TagOperation {
	string Tag;
	bool Add = false; // true=add false=delete
	ResolveFn<void> Resolve;
	RejectFn Reject;

	TagOperation(string tag, bool add, ResolveFn<void>&& resolve, RejectFn&& reject) : Tag(tag), Add(add), Resolve(resolve), Reject(reject) {}
};

using Self = std::shared_ptr<R66ApiImpl>;
class R66::R66ApiImpl : public std::enable_shared_from_this<R66ApiImpl>, public R66Api {
public:
	virtual std::shared_ptr<IR66ApiCommon> get_shared_from_this() override {
		return shared_from_this();
	}

	std::shared_ptr<PAL::PlatformAbstractionLayer> PAL;

	std::recursive_mutex Mutex;
	InfallibleTask<VoidResult> Initialized;

	string ClientId;
	FallibleTask<AuthenticationInfoInternal> AuthInfo;
	InfallibleTask<AuthorizationResult> AuthResult;
	AuthStatus MyAuthStatus;
	InfallibleTask<UserInfo> MyUserInfo;
	InfallibleTask<StreamInfo> MyStreamInfo;
	TimePoint MyStreamInfoLastUpdate;

	std::list<TagOperation> TagOperationQueue;
	bool TagsUpdateInProgress = false;

	bool EventSubUseProxy;
	string EventSubToken;
	std::map<ESSubscription::MapKey, ESSubscription> EventSubSubscriptions;
	bool EventSubSubscriptionUpdateInProgress = false;
	std::shared_ptr<PAL::Websocket> EventSubSocket;
	int32_t EventSubReadTimeout = 10;
	std::deque<std::pair<TimePoint, string>> EventSubExistingMessageIds;
	std::map<string, PollPredictionStatus<PollInfo>> PendingPollInfoUpdates;
	std::map<string, PollPredictionStatus<PredictionInfo>> PendingPredictionInfoUpdates;
	MulticastEventQueue<ChannelSubscribeEvent> ChannelSubscribeListeners;
	MulticastEventQueue<ChannelFollowEvent> ChannelFollowListeners;
	MulticastEventQueue<ChannelCheerEvent> ChannelCheerListeners;
	MulticastEventQueue<CustomRewardEvent> CustomRewardListeners;
	MulticastEventQueue<HypeTrainEvent> HypeTrainListeners;
	MulticastEventQueue<ChannelRaidEvent> ChannelRaidListeners;

	FallibleTask<std::shared_ptr<std::vector<std::pair<string, CustomRewardDefinition>>>> CachedCustomRewardList;
	FallibleTask<VoidResult> CustomRewardsUpdateInProgress;
	std::vector<CustomRewardDefinition> CustomRewardsTarget;

	static void InitDone(std::shared_ptr<R66ApiImpl> self);

	void Init();

	// Note: constructing an R66Api that does not live inside a shared_ptr is UB
	// (due to our use of enable_shared_from_this)
	R66ApiImpl(
		std::shared_ptr<PAL::PlatformAbstractionLayer> pal,
		string_view clientId,
		bool useEventSubProxy
	);

	virtual void PrepareShutdown(ResolveFn<void> resolve, RejectFn reject) override;

	virtual void GetAuthState(ResolveFn<const AuthState&> resolve, RejectFn reject) override;
	virtual void GetAuthenticationInfo(string_view scopes, ResolveFn<const AuthenticationInfo&> resolve, RejectFn reject) override;
	virtual void LogOut(ResolveFn<void> resolve, RejectFn reject) override;

	virtual void GetMyUserInfo(ResolveFn<const UserInfo&> resolve, RejectFn reject) override;
	virtual void GetUserInfoById(string_view id, ResolveFn<const UserInfo&> resolve, RejectFn reject) override;
	virtual void GetUserInfoByLoginName(string_view name, ResolveFn<const UserInfo&> resolve, RejectFn reject) override;
	virtual void GetMyStreamInfo(ResolveFn<const StreamInfo&> resolve, RejectFn reject) override;
	virtual void QueryStreams(const StreamQuery& q, ResolveFn<const StreamQueryResult&> resolve, RejectFn reject) override;

	virtual void CheckUserSubscription(string_view broadcasterId, ResolveFn<const UserSubscriptionCheckResult&> resolve, RejectFn reject) override;

	virtual void CreatePoll(const PollDefinition& def, ResolveFn<const PollInfo&> resolve, RejectFn reject) override;
	virtual void WaitForPollUpdate(string_view pollId, ResolveFn<const PollInfo&> resolve, RejectFn reject) override;
	virtual void EndPoll(const EndPollRequest& req, ResolveFn<const PollInfo&> resolve, RejectFn reject) override;
	virtual void UnsubscribeFromPoll(string_view id, ResolveFn<void> resolve, RejectFn reject) override;

	virtual void CreatePrediction(const PredictionDefinition& def, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) override;
	virtual void WaitForPredictionUpdate(string_view pollId, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) override;
	virtual void EndPrediction(const EndPredictionRequest& req, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) override;
	virtual void UnsubscribeFromPrediction(string_view id, ResolveFn<void> resolve, RejectFn reject) override;

	virtual void CreateClip(bool hasDelay, ResolveFn<const ClipInfo&> resolve, RejectFn reject) override;
	virtual void CreateStreamMarker(string_view description, ResolveFn<const StreamMarkerInfo&> resolve, RejectFn reject) override;

	virtual void ModifyChannelInformation(const ModifyChannelInfoRequest& req, ResolveFn<void> resolve, RejectFn reject) override;

	virtual void GetBitsLeaderboard(const BitsLeaderboardRequest& req, ResolveFn<const BitsLeaderboard&> resolve, RejectFn reject) override;

	virtual void ReplaceCustomRewards(const CustomRewardList& req, ResolveFn<void> resolve, RejectFn reject) override;
	virtual void ResolveCustomReward(const CustomRewardResolveRequest& req, ResolveFn<void> resolve, RejectFn reject) override;

	virtual void SubscribeToEventStream(const EventStreamRequest& req, ResolveFn<const EventStreamDesc&> resolve, RejectFn reject) override;
	virtual void WaitForChannelSubscribeEvent(const EventStreamDesc& desc, ResolveFn<const ChannelSubscribeEvent&> resolve, RejectFn reject) override;
	virtual void WaitForChannelFollowEvent(const EventStreamDesc& desc, ResolveFn<const ChannelFollowEvent&> resolve, RejectFn reject) override;
	virtual void WaitForChannelCheerEvent(const EventStreamDesc& desc, ResolveFn<const ChannelCheerEvent&> resolve, RejectFn reject) override;
	virtual void WaitForCustomRewardEvent(const EventStreamDesc& desc, ResolveFn<const CustomRewardEvent&> resolve, RejectFn reject) override;
	virtual void WaitForHypeTrainEvent(const EventStreamDesc& desc, ResolveFn<const HypeTrainEvent&> resolve, RejectFn reject) override;
	virtual void WaitForChannelRaidEvent(const EventStreamDesc& desc, ResolveFn<const ChannelRaidEvent&> resolve, RejectFn reject) override;
	virtual void CloseEventStream(const EventStreamDesc& desc, ResolveFn<void> resolve, RejectFn reject) override;

	virtual void RefreshOAuthToken(ResolveFn<string_view> resolve) override;
	virtual void UpdateOAuthCredentials(string_view clientId) override;


	// private calls below:
	// (public for the sake of our source files, but private regarding the library user)

	static void CallApi(Self self, std::shared_ptr<LoggedInApiCall> call, ResolveFn<const WebRequestResult&> resolve, RejectFn reject, string authHeaderOverride = string());
	static bool CallApiRetry(Self self, std::shared_ptr<LoggedInApiCall> call, ResolveFn<const WebRequestResult&> resolve, RejectFn reject, string authHeaderOverride);

	static void LoadAuthTokens(Self self);
	static void TryParseAuthTokens(Self self, const Json& j);
	static void FetchAuthenticationInfo(Self self, string scopes, ResolveFn<const AuthenticationInfo&> resolve, RejectFn reject);
	static void CheckAuthenticationState(Self self);
	static void FetchMyOwnUserInfo(Self self);
	static void FetchUserInfo(Self self, string login, string id, ResolveFn<UserInfo&&> resolve, RejectFn reject);
	static void RefreshAccessToken(Self self);
	static void Deauthenticate(Self self, bool deletePersistently);
	static void FetchMyStreamInfo(Self self);
	static void FetchClipCreationProgress(Self self, string id, int attempts, ResolveFn<const ClipInfo&> resolve, RejectFn reject);

	static void ESTriggerSubscriptionUpdate(Self self);
	static void ESAddSubscription(Self self, const ESSubscription& sub);
	static void ESDelSubscription(Self self, const ESSubscription& sub);

	static void ESConnect(Self self);
	static void ESUpdateSubscriptions(Self self, string channelId);
	static void ESReceiving(Self self);
	static void ESReceivingOnDyingSocket(Self self, TimePoint startTs, std::shared_ptr<Websocket> sws);
	static void ESReceiveNotification(const Self& self, const JsonRef& envelope, string_view messageId);
	static void ESOnConnectionDead(Self self, const std::exception& e);

	static void ESEventPollUpdate(Self self, const JsonRef& j, string_view key);
	static void ESEventPredictionUpdate(Self self, const JsonRef& j, string_view key);
	static void ESEventChannelPointsUpdate(Self self, const JsonRef& j, string_view key);

	static void CPApplyUpdates(Self self, string uid);

	static void TagsApplyUpdates(Self self);
};

template<typename... Args> void Log(const Self& self, R66::LogLevel level, std::string_view format, Args&& ...args)
{
	auto s = format_string(format, args...);
	self->PAL->Log(level, from_utf8(s), []() {}, [](const std::exception&) {});
}

void BadHttpResponse(const R66::WebRequestResult& web, const R66::RejectFn& reject);

inline string str_toupper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
				   [](unsigned char c) { return std::toupper(c); }
	);
	return from_utf8(s).ToOwned();
}

template<class J> string json_tostring(J j) {
	return j.is_null() ? string() : j.to_string();
}

extern ESSubscription PollProgress;
extern ESSubscription PollEnd;
extern ESSubscription PredictionProgress;
extern ESSubscription PredictionLock;
extern ESSubscription PredictionEnd;

extern ESSubscription StreamOnline;
extern ESSubscription StreamOffline;
extern ESSubscription UserUpdate;
extern ESSubscription ChannelUpdate;

class BadHttpResponseException : public std::exception {
	std::string text;
	int64_t status;

public:
	BadHttpResponseException(std::string text, int64_t status) : text(text), status(status) {}

	virtual const char* what() const throw() {
		return text.c_str();
	}
	int64_t status_code() const {
		return status;
	}
};
