#include "r66_impl.hpp"



void R66ApiImpl::GetMyStreamInfo(ResolveFn<const StreamInfo&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	bool needFetch = MyStreamInfo.Await(std::move(resolve), std::move(reject));

	auto streamInfoAge = SteadyClock::now() - MyStreamInfoLastUpdate;
	std::chrono::seconds maxAge(60);
	if (streamInfoAge > maxAge)
	{
		needFetch = true;
		// need to update the timestamp in here (instead of on completion) so that
		// we don't kick off a new fetch task each time somebody invokes this before the update completes
		MyStreamInfoLastUpdate = SteadyClock::now();
	}

	if (needFetch) {
		R66ApiImpl::FetchMyStreamInfo(shared_from_this());
	}
}
void R66ApiImpl::FetchMyStreamInfo(Self self)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	self->MyUserInfo.Await([self](const UserInfo& userinfo)
	{
		std::lock_guard<std::recursive_mutex> lock(self->Mutex);
		StreamQuery query;
		query.UserIds.push_back(userinfo.ChannelId);
		self->QueryStreams(query,
			[self](const StreamQueryResult& res) {
				std::lock_guard<std::recursive_mutex> lock(self->Mutex);
				StreamInfo si;
				if (res.Streams.size() > 0)
				{
					si = res.Streams[0];
				}
				self->MyStreamInfo.Resolve(std::move(si));
			},
			[self, query](const std::exception& e) {
				Log(self, LogLevel::Warning, "Error fetching my stream info (retrying soon): %s"sv, e.what());
				self->PAL->Sleep(1000,
					[self]() { R66ApiImpl::FetchMyStreamInfo(self); },
					[](const std::exception& e) {}
				);
			}
		);
	});
}
void R66ApiImpl::QueryStreams(const StreamQuery& query, ResolveFn<const StreamQueryResult&> resolve, RejectFn reject)
{
	string url = STR("https://api.twitch.tv/helix/streams?"s);
	if (query.After.size()) {
		url += STR("&after="sv);
		url += query.After;
	}
	if (query.First) {
		std::string s = std::to_string(query.First);
		url += STR("&first="sv);
		url += from_utf8(s);
	}
	for (auto& x : query.GameIds) {
		url += STR("&game_id="sv);
		url += x;
	}
	for (auto& x : query.Languages) {
		url += STR("&language="sv);
		url += x;
	}
	for (auto& x : query.UserIds) {
		url += STR("&user_id="sv);
		url += x;
	}
	for (auto& x : query.UserLogins) {
		url += STR("&user_login="sv);
		url += x;
	}
	R66ApiImpl::CallApi(
		shared_from_this(),
		std::make_shared<LoggedInApiCall>(url),
		[resolve, reject](const WebRequestResult& r)
		{
			auto j = Json::Parse(r.ResponseBody);
			StreamQueryResult res;
			try {
				auto data = j.at("data");
				auto arraylen = data.array_size();
				for (size_t i = 0; i < arraylen; i++) {
					auto s = data.at(i);
					StreamInfo si;
					si.Id = s.at("id").to_string();
					si.UserId = s.at("user_id").to_string();
					si.UserLogin = s.at("user_login").to_string();
					si.UserName = s.at("user_name").to_string();
					si.GameId = s.at("game_id").to_string();
					si.GameName = s.at("game_name").to_string();
					si.Type = s.at("type").to_string();
					si.Title = s.at("title").to_string();
					si.ViewerCount = s.at("viewer_count").to_int();
					si.StartedAt = s.at("started_at").to_string();
					si.Language = s.at("language").to_string();
					si.ThumbnailUrl = s.at("thumbnail_url").to_string();
					si.IsMature = s.at("is_mature").to_bool();

					auto tags = s.at("tags");
					for (size_t ii = 0; ii < tags.array_size(); ii++) {
						si.Tags.push_back(tags.at(ii).to_string());
					}

					res.Streams.push_back(std::move(si));
				}

				res.PaginationCursor = j.at("pagination").at("cursor").to_string();
			}
			catch (...) {
			}

			resolve(res);
		},
		reject);
}
