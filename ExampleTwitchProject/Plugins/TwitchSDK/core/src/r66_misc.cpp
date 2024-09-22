#include "r66_impl.hpp"



void R66ApiImpl::CheckUserSubscription(string_view broadcasterId, ResolveFn<const UserSubscriptionCheckResult&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	MyUserInfo.Await(
		[self = shared_from_this(), broadcasterId = string(broadcasterId), resolve, reject](const UserInfo& ui)
		{
			string url = STR("https://api.twitch.tv/helix/subscriptions/user?broadcaster_id="s) + broadcasterId;
			url += STR("&user_id="sv);
			url += ui.ChannelId;
			R66ApiImpl::CallApi(
				self,
				std::make_shared<LoggedInApiCall>(url),
				[self, resolve, reject](const WebRequestResult& r)
				{
					std::lock_guard<std::recursive_mutex> lock(self->Mutex);

					UserSubscriptionCheckResult state;
					if (r.HttpStatus == 404)
					{
						// leave state on default values (IsSubscribed = false)
					}
					else
					{
						auto j = Json::Parse(r.ResponseBody);

						auto s = j.at("data").at(0);
						state.IsSubscribed = true;
						state.IsGift = s.at("is_gift").to_bool();
						state.Tier = s.at("tier").to_string();

						if (state.IsGift)
						{
							state.GifterLogin = s.at("gifter_login").to_string();
							state.GifterName = s.at("gifter_name").to_string();
						}
					}
					resolve(state);
				},
				reject
			);
		}
	);
}

void R66ApiImpl::CreateClip(bool hasDelay, ResolveFn<const ClipInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	MyUserInfo.Await(
		[self = shared_from_this(), hasDelay, resolve, reject](const UserInfo& ui) {
			string url = STR("https://api.twitch.tv/helix/clips?broadcaster_id="s);
			url += ui.ChannelId;
			url += STR("&has_delay="sv);
			url += hasDelay ? STR("true"sv) : STR("false"sv);
			auto call = std::make_shared<LoggedInApiCall>(url);
			call->Method = HttpMethod::Post;
			call->ContentType = STR("text/plain"sv);
			R66ApiImpl::CallApi(
				self,
				call,
				[self, hasDelay, resolve, reject](const WebRequestResult& r) {
					try
					{
						auto j = Json::Parse(r.ResponseBody);
						auto clipId = j.at("data").at(0).at("id").to_string();
						R66ApiImpl::FetchClipCreationProgress(self, clipId, 0, resolve, reject);
					}
					catch (...)
					{
						BadHttpResponse(r, reject);
					}
				},
				reject
			);
		}
	);
}

void R66::R66ApiImpl::FetchClipCreationProgress(Self self, string id, int attempts, ResolveFn<const ClipInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	if (attempts > 15)
	{
		// give up at 15 seconds (according to twitch)
		reject(std::runtime_error("Twitch failed to create the clip. Please try again later."));
		return;
	}

	string url = STR("https://api.twitch.tv/helix/clips?id="s);
	url += id;
	CallApi(
		self,
		std::make_shared<LoggedInApiCall>(url),
		[self, id, attempts, resolve, reject](const WebRequestResult& r) {
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			ClipInfo clip;

			try
			{
				auto j = Json::Parse(r.ResponseBody);
				auto s = j.at("data").at(0);
				clip.Id = s.at("id").to_string();
				clip.Url = s.at("url").to_string();
				clip.EmbedUrl = s.at("embed_url").to_string();
				clip.CreatedAt = s.at("created_at").to_string();
				clip.ThumbnailUrl = s.at("thumbnail_url").to_string();
				clip.Duration = (int32_t)s.at("duration").to_int();
			}
			catch (...)
			{
				// Sleep 1s
				self->PAL->Sleep(1000,
					[=]() { FetchClipCreationProgress(self, id, attempts + 1, resolve, reject); },
					[](const std::exception&) {});
				return;
			}

			resolve(clip);
		},
		reject
	);
}



void R66::R66ApiImpl::CreateStreamMarker(string_view description, ResolveFn<const StreamMarkerInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	// clamp description length to 140 characters, as documented
	if (description.size() > 140)
	{
		description = description.substr(0, 140);
	}

	MyUserInfo.Await(
		[self = shared_from_this(), description = string(description), resolve, reject](const UserInfo& ui) {
			Json req;
			req.set_property("user_id", Json(ui.ChannelId));
			if (description.size())
			{
				req.set_property("description", Json(description));
			}

			R66ApiImpl::CallApi(
				self,
				std::make_shared<LoggedInApiCall>(STR("https://api.twitch.tv/helix/streams/markers"s), req.serialize()),
				[resolve, reject](const WebRequestResult& r) {
					if (r.HttpStatus == 404)
					{
						reject(std::runtime_error("Channel is offline or VODs are disabled on this channel."));
						return;
					}

					StreamMarkerInfo marker;

					auto j = Json::Parse(r.ResponseBody);
					auto m = j.at("data").at(0);
					marker.Id = m.at("id").to_string();
					marker.CreatedAt = m.at("created_at").to_string();
					marker.Description = m.at("description").to_string();
					marker.PositionSeconds = m.at("position_seconds").to_int();

					resolve(marker);
				},
				reject
			);
		}
	);
}

void set_if_relevant(Json& j, const std::string& key, string_view value) {
	if (value.size())
	{
		j.set_property(key, Json(value));
	}
}

void R66::R66ApiImpl::ModifyChannelInformation(const ModifyChannelInfoRequest& req, ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	MyUserInfo.Await(
		[self = shared_from_this(), mci = ModifyChannelInfoRequest(req), resolve, reject](const UserInfo& ui) {
			Json req;
			set_if_relevant(req, "game_id"s, mci.GameId);
			set_if_relevant(req, "broadcaster_language"s, mci.Language);
			set_if_relevant(req, "title"s, mci.Title);
			if (mci.Delay >= 0)
			{
				req.set_property("delay"s, Json(mci.Delay));
			}
			if (mci.Tags.size() || mci.ForceUpdateTags)
			{
				std::vector<Json> tags;
				for (auto& tag : mci.Tags)
				{
					tags.push_back(Json(tag));
				}
				req.set_array("tags", std::move(tags));
			}

			string uri = STR("https://api.twitch.tv/helix/channels?broadcaster_id="s);
			uri += ui.ChannelId;

			R66ApiImpl::CallApi(
				self,
				std::make_shared<LoggedInApiCall>(uri, req.serialize(), HttpMethod::Patch),
				[resolve, reject](const WebRequestResult& r) {
					// no response
					resolve();
				},
				reject
			);
		}
	);
}

void R66::R66ApiImpl::GetBitsLeaderboard(const BitsLeaderboardRequest& req, ResolveFn<const BitsLeaderboard&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	string url = STR("https://api.twitch.tv/helix/bits/leaderboard?"s);
	if (req.Count > 0)
	{
		std::string s = std::to_string(req.Count);
		url += STR("&count="sv);
		url += from_utf8(s);
	}
	if (req.Period.size())
	{
		url += STR("&period="sv);
		url += req.Period;
	}
	if (req.StartedAt.size())
	{
		url += STR("&started_at="sv);
		url += req.StartedAt;
	}
	if (req.UserId.size())
	{
		url += STR("&user_id="sv);
		url += req.UserId;
	}

	R66ApiImpl::CallApi(
		shared_from_this(),
		std::make_shared<LoggedInApiCall>(url),
		[resolve, reject](const WebRequestResult& r) {
			BitsLeaderboard ret;

			try
			{
				auto j = Json::Parse(r.ResponseBody);
				auto data = j.at("data");
				for (size_t i = 0; i < data.array_size(); i++)
				{
					auto ele = data.at(i);
					BitsLeaderboardEntry entry;
					entry.UserId = ele.at("user_id").to_string();
					entry.UserName = ele.at("user_name").to_string();
					entry.Rank = ele.at("rank").to_int();
					entry.Score = ele.at("score").to_int();
					ret.Data.push_back(std::move(entry));
				}

				ret.StartedAt = j.at("date_range").at("started_at").to_string();
				ret.EndedAt = j.at("date_range").at("ended_at").to_string();
			}
			catch (...)
			{
			}

			resolve(ret);
		},
		reject
	);
}
