#include "r66_impl.hpp"
#include <sstream> // for istringstream below

auto ESConnectionDead(Self self) {
	return [self](const std::exception& e) { R66ApiImpl::ESOnConnectionDead(self, e); };
}

#define ES_WEBSOCKET_LIVE_URL STR("wss://eventsub.wss.twitch.tv/ws"sv)
#define ES_WEBSOCKET_DEV_URL STR("ws://127.0.0.1:25708/url-unknown"sv)
#define ES_WEBSOCKET_PROTOCOL STR("protocol-unknown"sv)
#define ES_HELIX_LIVE_URL STR("https://api.twitch.tv/helix/eventsub/subscriptions"s)
#define ES_HELIX_DEV_URL STR("http://127.0.0.1:25708/helix/eventsub/subscriptions"s)

void R66ApiImpl::ESConnect(Self self)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	self->MyUserInfo.Await([self](const UserInfo&) {
		std::lock_guard<std::recursive_mutex> lock(self->Mutex);

		// UserInfo is not needed here, but we wait for it to avoid establishing
		// eventsub connections for users that aren't even logged in
		Websocket::Open(
			self->PAL,
			self->EventSubUseProxy ? ES_WEBSOCKET_DEV_URL : ES_WEBSOCKET_LIVE_URL,
			ES_WEBSOCKET_PROTOCOL,
			[self](std::shared_ptr<Websocket> sws) {
				Log(self, LogLevel::Debug, "successfully connected to eventsub"sv);
				sws->Recv(
					10, /* 10 seconds timeout for initial message */
					[self, sws](R66::string_view welcome) {
						std::lock_guard<std::recursive_mutex> lock(self->Mutex);

						auto json = Json::Parse(welcome);
						if (json.at("metadata").at("message_type").to_string() != STR("session_welcome"sv))
						{
							throw std::runtime_error("expected session_welcome, but got something else");
						}

						auto websocket = json.at("payload").at("session");
						string token = websocket.at("id").to_string();
						self->EventSubReadTimeout = 1 + (int32_t)websocket.at("keepalive_timeout_seconds").to_int();

						Log(self, LogLevel::Debug, "got eventsub token: %s"sv, to_utf8(token));
						self->EventSubToken = std::move(token);
						self->EventSubSocket = sws;

						ESTriggerSubscriptionUpdate(self);
						ESReceiving(self);
					},
					ESConnectionDead(self)
				);
			},
			ESConnectionDead(self)
		);
	});
}

void R66::R66ApiImpl::ESTriggerSubscriptionUpdate(Self self) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	if (!self->EventSubSocket)
	{
		// the socket is not even alive, so it's not possible to register any events
		return;
	}
	self->MyUserInfo.Await([self](const UserInfo& ui) { ESUpdateSubscriptions(self, ui.ChannelId.ToOwned()); });
}

void R66::R66ApiImpl::ESAddSubscription(Self self, const ESSubscription& sub) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	Log(self, LogLevel::Debug, "[ES] Subscribing to %s", to_utf8(sub.Type));
	auto& subscription = self->EventSubSubscriptions.try_emplace(sub.Key(), sub).first->second;
	if (subscription.RefCount == UINT32_MAX)
	{
		// not like this is ever going to happen, but just to be on the safe side
		return;
	}
	subscription.RefCount++;
	ESTriggerSubscriptionUpdate(self);
}

void R66::R66ApiImpl::ESDelSubscription(Self self, const ESSubscription& s) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	auto& sub = self->EventSubSubscriptions.at(s.Key());
	if (sub.RefCount == 0)
	{
		throw std::runtime_error("trying to release a subscription that is already at zero");
	}

	sub.RefCount -= 1;
	Log(self, LogLevel::Debug, "[ES] Unsubscribing from %s (rc=%d)", to_utf8(sub.Type), (int)sub.RefCount);
	ESTriggerSubscriptionUpdate(self);
}

void R66ApiImpl::ESOnConnectionDead(Self self, const std::exception& e)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	std::istringstream iss(e.what());
	std::string what;
	std::getline(iss, what, '\n');
	Log(self, LogLevel::Error, "EventSub error: %s"sv, what);
	// on error, we close the socket, sleep, and retry
	self->EventSubSocket.reset();
	self->PAL->Sleep(1000, [self]() { ESConnect(self); }, [](const std::exception&) {});
}

void R66ApiImpl::ESUpdateSubscriptions(Self self, string broadcasterId)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	if (self->EventSubSubscriptionUpdateInProgress)
	{
		// this loop is already running, no need to launch a second instance
		return;
	}
	if (!self->EventSubSocket)
	{
		// the socket is not even alive, so it's not possible to register any events
		return;
	}

	ESSubscription* subscription = nullptr;
	for (auto& s : self->EventSubSubscriptions)
	{
		auto isAlive = s.second.SocketToken == self->EventSubToken;
		auto shouldBeAlive = s.second.RefCount > 0;
		if (isAlive != shouldBeAlive)
		{
			subscription = &s.second;
			break;
		}
	}
	if (!subscription)
	{
		return;
	}

	std::shared_ptr<LoggedInApiCall> call = std::make_shared<LoggedInApiCall>(self->EventSubUseProxy ? ES_HELIX_DEV_URL : ES_HELIX_LIVE_URL);
	auto socketToken = self->EventSubToken;

	if (subscription->RefCount == 0)
	{
		// unregister
		call->Uri += STR("?id="sv);
		EncodeUriComponent(call->Uri, subscription->ActiveSubId);
		socketToken = string();
		call->Method = HttpMethod::Delete;
	}
	else
	{
		// register
		Json body, condition, transport;
		condition.set_property(subscription->FilterConditionName, Json(broadcasterId));
		if (subscription->Type == STR("channel.follow"sv))
		{
			condition.set_property("moderator_user_id", Json(broadcasterId));
		}
		transport.set_property("method", Json("websocket"sv));

		// Originally, we set socketToken as both "id" (as originally specified by the RFC) and "websocket_id" (as implemented originally).
		// The specification was later updated to require "session_id".
		transport.set_property("session_id", Json(socketToken));

		body.set_property("condition", condition);
		body.set_property("transport", transport);
		body.set_property("version", Json(subscription->Version));
		body.set_property("type", Json(subscription->Type));

		call->RequestBody = body.serialize();
		call->ContentType = STR("application/json"s);
		call->Method = HttpMethod::Post;
	}

	self->EventSubSubscriptionUpdateInProgress = true;
	CallApi(
		self,
		call,
		[self, broadcasterId, socketToken, sub = subscription->Key()](const WebRequestResult& r) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);
			self->EventSubSubscriptionUpdateInProgress = false;

			auto& subscription = self->EventSubSubscriptions.at(sub);
			try
			{
				subscription.ActiveSubId = Json::Parse(r.ResponseBody).at("data").at(0).at("id").to_string();
			}
			catch (...)
			{
			}

			// Log(self, "got affirmative response: %s", to_utf8(r.ResponseBody));
			subscription.SocketToken = socketToken;

			// loop
			ESUpdateSubscriptions(self, broadcasterId);
		},
		[self, broadcasterId, socketToken, sub = subscription->Key()](const std::exception& e) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			try
			{
				// using try+throw+catch instead of dynamic_cast here due to issues with dynamic_cast on PS5
				throw e;
			}
			catch (const BadHttpResponseException& badHttp)
			{
				if (badHttp.status_code() == 403)
				{
					// assume 403 is always fatal and should not cause a retry.
					// simply pretend that this is registered
					auto& subscription = self->EventSubSubscriptions.at(sub);
					subscription.SocketToken = socketToken;
					ESUpdateSubscriptions(self, broadcasterId);
					return;
				}
			}
			catch (...)
			{
			}

			self->EventSubSubscriptionUpdateInProgress = false;
			// close the socket, causing the recv job to fail
			self->EventSubSocket.reset();
			// meanwhile, stop looping here
			// once the websocket has reconnected, we will be triggered again
			Log(self, LogLevel::Error, "Closing down ES socket due to registration error: %s", e.what());
		}
	);
}


ESSubscription ChannelSubscribe(STR("channel.subscribe"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription ChannelResubscribe(STR("channel.subscription.message"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription ChannelFollow(STR("channel.follow"s), STR("2"s), "broadcaster_user_id"s);
ESSubscription ChannelCheer(STR("channel.cheer"s), STR("1"s), "broadcaster_user_id"s);

ESSubscription CustomRewardAdd(STR("channel.channel_points_custom_reward_redemption.add"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription CustomRewardUpdate(STR("channel.channel_points_custom_reward_redemption.update"s), STR("1"s), "broadcaster_user_id"s);

ESSubscription HypeTrainProgress(STR("channel.hype_train.progress"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription HypeTrainEnd(STR("channel.hype_train.end"s), STR("1"s), "broadcaster_user_id"s);

ESSubscription StreamOnline(STR("stream.online"s), STR("1"s), "broadcaster_user_id");
ESSubscription StreamOffline(STR("stream.offline"s), STR("1"s), "broadcaster_user_id");
ESSubscription UserUpdate(STR("user.update"s), STR("1"s), "user_id");
ESSubscription ChannelUpdate(STR("channel.update"s), STR("2"s), "broadcaster_user_id");

ESSubscription ChannelGettingRaided(STR("channel.raid"s), STR("1"s), "to_broadcaster_user_id");
ESSubscription ChannelRaidingSomeoneElse(STR("channel.raid"s), STR("1"s), "from_broadcaster_user_id");

template<class J> HypeTrainContribution ESParseHypeTrainContribution(J e) {
	HypeTrainContribution htc;
	htc.UserId = e.at("user_id").to_string();
	htc.UserName = e.at("user_name").to_string();
	auto type = e.at("type").to_string();
	if (type == STR("bits"sv))
	{
		htc.Type = HypeTrainContributionType::Bits;
	}
	else if (type == STR("subscription"sv))
	{
		htc.Type = HypeTrainContributionType::Subscription;
	}
	else
	{
		throw std::runtime_error("unknown contribution type");
	}
	htc.Total = e.at("total").to_int();
	return htc;
}


static bool str_startswith(string_view s, string_view prefix) {
	return s.rfind(prefix, 0) == 0;
}
void R66ApiImpl::ESReceiving(Self self)
{
	if (!self->EventSubSocket)
	{
		ESOnConnectionDead(self, std::runtime_error("we closed down our websocket connection"));
		return;
	}

	self->EventSubSocket->Recv(
		self->EventSubReadTimeout,
		[self](string_view message) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			auto envelope = Json::Parse(message);

			auto messageType = envelope.at("metadata").at("message_type").to_string();
			if (messageType == STR("session_keepalive"sv))
			{
				ESReceiving(self);
				return;
			}
			else if (messageType == STR("session_reconnect"sv))
			{
				string newUrl = envelope.at("payload").at("session").at("reconnect_url").to_string();
				Websocket::Open(
					self->PAL, newUrl, ES_WEBSOCKET_PROTOCOL,
					[self](std::shared_ptr<Websocket> sws) {
						std::lock_guard<std::recursive_mutex> lock(self->Mutex);

						Log(self, LogLevel::Debug, "successfully switched to new eventsub websocket"sv);

						// swap out the socket and resume regular operation over there
						std::swap(self->EventSubSocket, sws);
						ESReceiving(self);

						// now sws holds the old websocket, where we need to keep reading for up to 10 seconds
						ESReceivingOnDyingSocket(self, SteadyClock::now(), sws);
					},
					[self](const std::exception& e) {
						Log(self, LogLevel::Warning, "[ES] Failed to switch websockets. Disconnecting and starting over."sv);
						ESOnConnectionDead(self, e);
					}
				);
				// do NOT loop here; wait for the result of the websocket operation first
				return;
			}
			else if (messageType == STR("notification"sv))
			{
				auto payload = envelope.at("payload");
				ESReceiveNotification(self, payload, envelope.at("metadata").at("message_id").to_string());
			}
			else
			{
				Log(self, LogLevel::Error, "Unimplemented message_type: %s"sv, to_utf8(messageType));
			}


			ESReceiving(self);
		},
		ESConnectionDead(self)
	);
}

void R66::R66ApiImpl::ESReceivingOnDyingSocket(Self self, TimePoint startTs, std::shared_ptr<Websocket> sws) {
	auto now = SteadyClock::now();
	auto oldSocketAge = now - startTs;
	std::chrono::seconds oldSocketMaxAge(10);

	int32_t secondsRemaining = std::chrono::duration_cast<std::chrono::duration<int32_t>>(oldSocketMaxAge - oldSocketAge).count();

	if (secondsRemaining == 0)
	{
		Log(self, LogLevel::Debug, "Websocket switchover complete, closing old socket.");
		return;
	}

	sws->Recv(
		secondsRemaining,
		[self, startTs, sws](string_view message) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);
			auto envelope = Json::Parse(message);

			if (envelope.at("metadata").at("message_type").to_string() == STR("notification"sv))
			{
				auto payload = envelope.at("payload");
				ESReceiveNotification(self, payload, envelope.at("metadata").at("message_id").to_string());
			}
			// ignore other types of messages here

			ESReceivingOnDyingSocket(self, startTs, sws);
		},
		[self](const std::exception& e) {
			// this socket is expected to die, so if something goes wrong we don't really care
			// in particular, this is what happens on timeout (the more optimistically worded message above is unlikely to trigger)
			Log(self, LogLevel::Debug, "Error on old websocket during switchover, closing it: %s", e.what());
		}
	);
}

void R66::R66ApiImpl::ESReceiveNotification(const Self& self, const JsonRef& json, string_view messageId) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	auto now = SteadyClock::now();
	auto cutoffPoint = now - std::chrono::seconds(10);
	// clear out all message ids older than 10s
	auto& esemi = self->EventSubExistingMessageIds;
	while (!esemi.empty() && (esemi.front().first < cutoffPoint))
	{
		Log(self, LogLevel::Debug, "[ES] Clearing out id %s"sv, to_utf8(esemi.front().second));
		esemi.pop_front();
	}

	// have we seen this message before? if yes, drop it
	if (std::any_of(esemi.cbegin(), esemi.cend(), [=](const std::pair<TimePoint, string>& p) { return p.second == messageId; }))
	{
		Log(self, LogLevel::Info, "[ES] Dropping duplicate message"sv);
		return;
	}

	// remember that we have seen this message
	esemi.push_back(std::make_pair(now, string(messageId)));

	string_holder subTypeHolder;
	try
	{
		subTypeHolder = json.at("subscription").at("type").to_string();
	}
	catch (...)
	{
	}
	string_view subType = subTypeHolder;
	if (str_startswith(subType, STR("channel.poll."sv)))
	{
		ESEventPollUpdate(self, json, subType);
	}
	else if (str_startswith(subType, STR("channel.prediction."sv)))
	{
		ESEventPredictionUpdate(self, json, subType);
	}
	else if (subType == CustomRewardAdd.Type || subType == CustomRewardUpdate.Type)
	{
		ESEventChannelPointsUpdate(self, json, subType);
	}
	else if (subType == ChannelSubscribe.Type || subType == ChannelResubscribe.Type)
	{
		auto e = json.at("event");
		ChannelSubscribeEvent cse;
		cse.UserId = e.at("user_id").to_string();
		cse.UserLogin = e.at("user_login").to_string();
		cse.UserDisplayName = e.at("user_name").to_string();
		cse.Tier = e.at("tier").to_string();
		if (subType == ChannelSubscribe.Type)
		{
			// ChannelResubscribe does not have this property
			cse.IsGift = e.at("is_gift").to_bool();
		}
		else
		{
			cse.CumulativeMonths = (int32_t)e.at("cumulative_months").to_int();
			auto streak = e.at("streak_months");
			cse.StreakMonths = streak.is_null() ? -1 : (int32_t)streak.to_int();
			cse.DurationMonths = (int32_t)e.at("duration_months").to_int();
		}
		self->ChannelSubscribeListeners.Push(cse);
	}
	else if (subType == ChannelFollow.Type)
	{
		auto e = json.at("event");
		ChannelFollowEvent cfe;
		cfe.UserId = e.at("user_id").to_string();
		cfe.UserDisplayName = e.at("user_name").to_string();
		cfe.FollowedAt = e.at("followed_at").to_string();
		self->ChannelFollowListeners.Push(cfe);
	}
	else if (subType == ChannelCheer.Type)
	{
		auto e = json.at("event");
		ChannelCheerEvent cce;
		cce.IsAnonymous = e.at("is_anonymous").to_bool();
		cce.UserId = json_tostring(e.at("user_id"));
		cce.UserDisplayName = json_tostring(e.at("user_name"));
		cce.Message = e.at("message").to_string();
		cce.Bits = e.at("bits").to_int();
		self->ChannelCheerListeners.Push(cce);
	}
	else if (subType == ChannelGettingRaided.Type) // type is the same for raiding and getting raided
	{
		auto e = json.at("event");
		ChannelRaidEvent cre;
		cre.FromBroadcasterId = e.at("from_broadcaster_user_id").to_string();
		cre.FromBroadcasterName = e.at("from_broadcaster_user_name").to_string();
		cre.ToBroadcasterId = e.at("to_broadcaster_user_id").to_string();
		cre.ToBroadcasterName = e.at("to_broadcaster_user_name").to_string();
		cre.Viewers = e.at("viewers").to_int();
		self->ChannelRaidListeners.Push(cre);
	}
	else if (subType == HypeTrainProgress.Type)
	{
		auto e = json.at("event");
		HypeTrainEvent hte;
		hte.HypeTrainId = e.at("id").to_string();
		hte.BroadcasterId = e.at("broadcaster_user_id").to_string();
		hte.BroadcasterName = e.at("broadcaster_user_name").to_string();
		hte.Level = (int32_t)e.at("level").to_int();
		hte.TotalPoints = e.at("total").to_int();
		hte.Progress = e.at("progress").to_int();
		hte.Goal = e.at("goal").to_int();
		auto tops = e.at("top_contributions");
		for (size_t i = 0; i < tops.array_size(); i++)
		{
			hte.TopContributions.push_back(ESParseHypeTrainContribution(tops.at(i)));
		}
		hte.LastContribution = ESParseHypeTrainContribution(e.at("last_contribution"));
		hte.StartedAt = e.at("started_at").to_string();
		hte.ExpiresAt = e.at("expires_at").to_string();
		self->HypeTrainListeners.Push(hte);
	}
	else if (subType == HypeTrainEnd.Type)
	{
		auto e = json.at("event");
		HypeTrainEvent hte;
		hte.HypeTrainId = e.at("id").to_string();
		hte.BroadcasterId = e.at("broadcaster_user_id").to_string();
		hte.BroadcasterName = e.at("broadcaster_user_name").to_string();
		hte.Level = (int32_t)e.at("level").to_int();
		hte.TotalPoints = e.at("total").to_int();
		hte.Progress = -1;
		hte.Goal = -1;
		auto tops = e.at("top_contributions");
		for (size_t i = 0; i < tops.array_size(); i++)
		{
			hte.TopContributions.push_back(ESParseHypeTrainContribution(tops.at(i)));
		}
		hte.StartedAt = e.at("started_at").to_string();
		hte.EndedAt = e.at("ended_at").to_string();
		hte.CooldownEndsAt = e.at("cooldown_ends_at").to_string();
		self->HypeTrainListeners.Push(hte);
	}
	else if (subType == StreamOnline.Type || subType == StreamOffline.Type)
	{
		self->MyStreamInfo.Invalidate();
	}
	else if (subType == ChannelUpdate.Type)
	{
		auto e = json.at("event");
		if (self->MyStreamInfo.IsDone())
		{
			auto& si = self->MyStreamInfo.UnwrapMut();
			si.Title = e.at("title").to_string();
			si.Language = e.at("language").to_string();
			// TODO: categories are gone and replaced with tags

			// update the timestamp in order to avoid a quick stream info update
			// since helix is known to return delayed data here
			// (i.e. we would overwrite with old data quickly after showing the new data)
			self->MyStreamInfoLastUpdate = SteadyClock::now();
		}
	}
	else if (subType == UserUpdate.Type)
	{
		FetchMyOwnUserInfo(self);
	}
	else
	{
		Log(self, LogLevel::Debug, "[ES] got unknown ES notification: %s"sv, to_utf8(subType));
	}
}

ESSubscription PollProgress(STR("channel.poll.progress"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription PollEnd(STR("channel.poll.end"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription PredictionProgress(STR("channel.prediction.progress"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription PredictionLock(STR("channel.prediction.lock"s), STR("1"s), "broadcaster_user_id"s);
ESSubscription PredictionEnd(STR("channel.prediction.end"s), STR("1"s), "broadcaster_user_id"s);




#define ES_IMPL_EVENT(x, kind) \
    void R66::R66ApiImpl::WaitFor##x##Event(const EventStreamDesc& desc, ResolveFn<const x##Event&> resolve, RejectFn reject) { \
		if (desc.Kind != EventStreamKind::kind) { reject(std::runtime_error("wrong type of subscription for this function")); return; } \
		x##Listeners .Pop(desc.Token, std::move(resolve), std::move(reject)); \
	} \

struct ESMetadata {
	const ESSubscription* SubscriptionPtrs[4];
	IMulticastEventQueue* (*GetQueue)(R66ApiImpl*);
};

#define ES_MM_ENTRY(x, y, ...) { EventStreamKind::x, { { __VA_ARGS__ }, [](R66ApiImpl* t) -> IMulticastEventQueue* { return &t->y##Listeners; } } }


// How to add new event types:
// 1. Create type, wait function, and enum entry in Interface.cs
// 2. Add ES_IMPL_EVENT and ES_MM_ENTRY
// 3. Implement receiving code in ESReceiveNotification above
// If needed, adjust array size in ESMetadata below
ES_IMPL_EVENT(ChannelSubscribe, Subscription)
ES_IMPL_EVENT(ChannelFollow, Follower)
ES_IMPL_EVENT(ChannelCheer, Cheer)
ES_IMPL_EVENT(CustomReward, CustomRewardRedemption)
ES_IMPL_EVENT(HypeTrain, HypeTrain)
ES_IMPL_EVENT(ChannelRaid, ChannelRaid)

const std::map<EventStreamKind, ESMetadata> MetaMap = {
	ES_MM_ENTRY(Subscription, ChannelSubscribe, &ChannelSubscribe),
	ES_MM_ENTRY(Follower, ChannelFollow, &ChannelFollow),
	ES_MM_ENTRY(Cheer, ChannelCheer, &ChannelCheer),
	ES_MM_ENTRY(CustomRewardRedemption, CustomReward, &CustomRewardAdd, &CustomRewardUpdate),
	ES_MM_ENTRY(HypeTrain, HypeTrain, &HypeTrainProgress, &HypeTrainEnd),
	ES_MM_ENTRY(ChannelRaid, ChannelRaid, &ChannelGettingRaided, &ChannelRaidingSomeoneElse),
};


void R66::R66ApiImpl::SubscribeToEventStream(const EventStreamRequest& req, ResolveFn<const EventStreamDesc&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	EventStreamDesc desc;
	desc.Kind = req.Kind;
	auto& meta = MetaMap.at(req.Kind);
	for (auto subDesc : meta.SubscriptionPtrs)
	{
		if (!subDesc) break;
		ESAddSubscription(shared_from_this(), *subDesc);
	}
	desc.Token = meta.GetQueue(this)->Register();
	resolve(desc);
}
void R66::R66ApiImpl::CloseEventStream(const EventStreamDesc& desc, ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	auto& meta = MetaMap.at(desc.Kind);
	for (auto subDesc : meta.SubscriptionPtrs)
	{
		if (!subDesc) break;
		ESDelSubscription(shared_from_this(), *subDesc);
	}
	meta.GetQueue(this)->Unregister(desc.Token);
	resolve();
}
