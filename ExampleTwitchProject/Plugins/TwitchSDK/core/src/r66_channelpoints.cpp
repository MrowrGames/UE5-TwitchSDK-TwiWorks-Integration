#include "r66_impl.hpp"


void R66::R66ApiImpl::ESEventChannelPointsUpdate(Self self, const JsonRef& j, string_view key) {
	auto e = j.at("event");
	CustomRewardEvent cre;
	cre.RedemptionId = e.at("id").to_string();
	cre.BroadcasterId = e.at("broadcaster_user_id").to_string();
	cre.BroadcasterName = e.at("broadcaster_user_name").to_string();
	cre.RedeemerId = e.at("user_id").to_string();
	cre.RedeemerName = e.at("user_name").to_string();
	auto status = e.at("status").to_string();
	if (status == STR("unfulfilled"sv))
	{
		cre.Status = CustomRewardRedemptionState::Unfulfilled;
	}
	else if (status == STR("fulfilled"sv))
	{
		cre.Status = CustomRewardRedemptionState::Fulfilled;
	}
	else if (status == STR("canceled"sv))
	{
		cre.Status = CustomRewardRedemptionState::Canceled;
	}
	else
	{
		cre.Status = CustomRewardRedemptionState::Unknown;
	}
	cre.CustomRewardId = e.at("reward").at("id").to_string();
	cre.CustomRewardTitle = e.at("reward").at("title").to_string();
	cre.CustomRewardCost = e.at("reward").at("cost").to_int();
	cre.CustomRewardPrompt = e.at("reward").at("prompt").to_string();
	cre.RedeemedAt = e.at("redeemed_at").to_string();
	self->CustomRewardListeners.Push(cre);
}


void R66::R66ApiImpl::ReplaceCustomRewards(const CustomRewardList& req, ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	CustomRewardsTarget = req.Rewards;

	if (!CustomRewardsUpdateInProgress.Await([resolve](const VoidResult&) { resolve(); }, std::move(reject)))
	{
		// if an update task is already active, they will now aim for the new target list
		return;
	}

	// else we need to launch an update task
	MyUserInfo.Await(
		[self = shared_from_this()](const UserInfo& ui)
		{
			CPApplyUpdates(self, ui.ChannelId.ToOwned());
		}
	);
}

bool Equals(const CustomRewardDefinition& a, const CustomRewardDefinition& b) {
	if (a.Title != b.Title) return false;
	if (a.Cost != b.Cost) return false;
	if (a.BackgroundColor.size() && b.BackgroundColor.size() && (a.BackgroundColor != b.BackgroundColor)) return false;
	if (a.ShouldRedemptionsSkipRequestQueue != b.ShouldRedemptionsSkipRequestQueue) return false;
	if (a.IsEnabled != b.IsEnabled) return false;

	if (a.IsUserInputRequired != b.IsUserInputRequired) return false;
	if (a.IsUserInputRequired && (a.Prompt != b.Prompt)) return false;

	if (a.IsMaxPerStreamEnabled != b.IsMaxPerStreamEnabled) return false;
	if (a.IsMaxPerStreamEnabled && (a.MaxPerStream != b.MaxPerStream)) return false;

	if (a.IsMaxPerUserPerStreamEnabled != b.IsMaxPerUserPerStreamEnabled) return false;
	if (a.IsMaxPerUserPerStreamEnabled && (a.MaxPerUserPerStream != b.MaxPerUserPerStream)) return false;

	if (a.IsGlobalCooldownEnabled != b.IsGlobalCooldownEnabled) return false;
	if (a.IsGlobalCooldownEnabled && (a.GlobalCooldownSeconds != b.GlobalCooldownSeconds)) return false;

	return true;
}

void R66::R66ApiImpl::CPApplyUpdates(Self self, string uid) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	auto failure = [self](const std::exception& e) {
		Log(self, LogLevel::Debug, "[CP] FAILURE !!!"sv);
		std::lock_guard<std::recursive_mutex> lock(self->Mutex);
		self->CustomRewardsUpdateInProgress.Reject(e);
		self->CustomRewardsUpdateInProgress.Invalidate();
		// need to re-fetch the list as well
		self->CachedCustomRewardList.Invalidate();
	};

	if (!self->CachedCustomRewardList.Await(
		[self, uid, failure](const std::shared_ptr<std::vector<std::pair<string, CustomRewardDefinition>>>& v)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			// let the updates begin!
			// we have:
			// * a list of current definitions (with id)
			// * a list of desired definitions

			// the strategy is:
			// - if any definition we don't desire currently exists, delete it
			// - else if we can find any missing definition, add it
			// - else the lists are equivalent and we are done

			// this algorithm currently never updates definitions (that might get confusing really fast)

			Log(self, LogLevel::Debug, "[CP] In CP loop"sv);
			for (auto existing = v->begin(); existing != v->end(); ++existing)
			{
				bool isDesired = false;
				for (auto& candidate : self->CustomRewardsTarget)
				{
					if (Equals(existing->second, candidate))
					{
						isDesired = true;
						break;
					}
				}

				if (!isDesired)
				{
					// delete this
					Log(self, LogLevel::Debug, "[CP] Deleting %s"sv, to_utf8(existing->second.Title).data());
					string uri = STR("https://api.twitch.tv/helix/channel_points/custom_rewards?broadcaster_id="s);
					uri += uid;
					uri += STR("&id="sv);
					uri += existing->first;
					auto call = std::make_shared<LoggedInApiCall>(uri);
					call->Method = HttpMethod::Delete;
					CallApi(self, call, [self, uid](const WebRequestResult&) { CPApplyUpdates(self, uid); }, failure);
					v->erase(existing);
					return;
				}
			}

			for (auto& desired : self->CustomRewardsTarget)
			{
				bool exists = false;
				for (auto& candidate : *v)
				{
					if (Equals(desired, candidate.second))
					{
						exists = true;
						break;
					}
				}

				if (!exists)
				{
					// create this
					Log(self, LogLevel::Debug, "[CP] Creating %s"sv, to_utf8(desired.Title).data());
					Json body;
					body.set_property("title", Json(desired.Title));
					body.set_property("cost", Json(desired.Cost));
					body.set_property("prompt", Json(desired.Prompt));
					body.set_property("is_enabled", Json(desired.IsEnabled));
					body.set_property("background_color", Json(desired.BackgroundColor));
					body.set_property("is_user_input_required", Json(desired.IsUserInputRequired));
					body.set_property("is_max_per_stream_enabled", Json(desired.IsMaxPerStreamEnabled));
					body.set_property("max_per_stream", Json(desired.MaxPerStream));
					body.set_property("is_max_per_user_per_stream_enabled", Json(desired.IsMaxPerUserPerStreamEnabled));
					body.set_property("max_per_user_per_stream", Json(desired.MaxPerUserPerStream));
					body.set_property("is_global_cooldown_enabled", Json(desired.IsGlobalCooldownEnabled));
					body.set_property("global_cooldown_seconds", Json(desired.GlobalCooldownSeconds));
					body.set_property("should_redemptions_skip_request_queue", Json(desired.ShouldRedemptionsSkipRequestQueue));

					string uri = STR("https://api.twitch.tv/helix/channel_points/custom_rewards?broadcaster_id="s);
					uri += uid;
					CallApi(
						self,
						std::make_shared<LoggedInApiCall>(uri, body.serialize()),
						[self, uid, added = CustomRewardDefinition(desired)](const WebRequestResult& r) mutable
						{
							std::lock_guard<std::recursive_mutex> lock(self->Mutex);
							auto json = Json::Parse(r.ResponseBody);
							string id = json.at("data").at(0).at("id").to_string();
							self->CachedCustomRewardList.Unwrap()->push_back(std::make_pair(std::move(id), std::move(added)));
							CPApplyUpdates(self, uid);
						},
						failure);
					return;
				}
			}

			Log(self, LogLevel::Debug, "[CP] Over and out!"sv);
			self->CustomRewardsUpdateInProgress.Resolve(VoidResult());
			self->CustomRewardsUpdateInProgress.Invalidate();
		},
		failure
	))
	{
		return;
	}

	string uri = STR("https://api.twitch.tv/helix/channel_points/custom_rewards?only_manageable_rewards=true&broadcaster_id="s) + uid;
	R66ApiImpl::CallApi(
		self,
		std::make_shared<LoggedInApiCall>(uri),
		[self](const WebRequestResult& res) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			auto response = Json::Parse(res.ResponseBody);
			auto data = response.at("data");
			std::vector<std::pair<string, CustomRewardDefinition>> list;
			for (size_t i = 0; i < data.array_size(); i++)
			{
				auto reward = data.at(i);
				CustomRewardDefinition r;
				r.BackgroundColor = reward.at("background_color").to_string();
				r.IsEnabled = reward.at("is_enabled").to_bool();
				r.Cost = reward.at("cost").to_int();
				r.Title = reward.at("title").to_string();
				r.Prompt = reward.at("prompt").to_string();
				r.IsUserInputRequired = reward.at("is_user_input_required").to_bool();
				r.IsMaxPerStreamEnabled = reward.at("max_per_stream_setting").at("is_enabled").to_bool();
				r.MaxPerStream = (int32_t)reward.at("max_per_stream_setting").at("max_per_stream").to_int();
				r.IsMaxPerUserPerStreamEnabled = reward.at("max_per_user_per_stream_setting").at("is_enabled").to_bool();
				r.MaxPerUserPerStream = (int32_t)reward.at("max_per_user_per_stream_setting").at("max_per_user_per_stream").to_int();
				r.IsGlobalCooldownEnabled = reward.at("global_cooldown_setting").at("is_enabled").to_bool();
				r.GlobalCooldownSeconds = (int32_t)reward.at("global_cooldown_setting").at("global_cooldown_seconds").to_int();
				r.ShouldRedemptionsSkipRequestQueue = reward.at("should_redemptions_skip_request_queue").to_bool();
				list.push_back(std::make_pair(reward.at("id").to_string(), std::move(r)));
			}

			self->CachedCustomRewardList.Resolve(std::make_shared<std::vector<std::pair<string, CustomRewardDefinition>>>(std::move(list)));
		},
		[self](const std::exception& e)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			self->CachedCustomRewardList.Reject(e);
			// invalidate right away so that they can try again
			self->CachedCustomRewardList.Invalidate();
		}
	);
}

void R66::R66ApiImpl::ResolveCustomReward(const CustomRewardResolveRequest& req, ResolveFn<void> resolve, RejectFn reject) {
	string uri = STR("https://api.twitch.tv/helix/channel_points/custom_rewards/redemptions?id="s);
	uri += req.RedemptionId;
	uri += STR("&broadcaster_id="sv);
	uri += req.BroadcasterId;
	uri += STR("&reward_id="sv);
	uri += req.CustomRewardId;

	string_view resolution;
	Json body;
	switch (req.Resolution)
	{
	case CustomRewardRedemptionState::Fulfilled:
		resolution = STR("FULFILLED"sv);
		break;
	case CustomRewardRedemptionState::Canceled:
		resolution = STR("CANCELED"sv);
		break;
	default:
		reject(std::runtime_error("Invalid resolution value"));
		return;
	}
	body.set_property("status", Json(resolution));

	R66ApiImpl::CallApi(
		shared_from_this(),
		std::make_shared<LoggedInApiCall>(uri, body.serialize(), HttpMethod::Patch),
		[resolve](const WebRequestResult& r) {
			// no response
			resolve();
		},
		reject
	);
}

//void R66::R66ApiImpl::WaitForCustomRewardEvent(const EventStreamDesc& desc, ResolveFn<const CustomRewardEvent&> resolve, RejectFn reject) {}

