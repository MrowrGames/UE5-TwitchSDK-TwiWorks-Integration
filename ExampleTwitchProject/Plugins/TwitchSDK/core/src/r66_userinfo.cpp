#include "r66_impl.hpp"



void R66ApiImpl::FetchMyOwnUserInfo(Self self)
{
	R66ApiImpl::FetchUserInfo(self, string(), string(),
		[self](UserInfo&& ui)
		{
			self->MyUserInfo.Resolve(std::move(ui));
		},
		[self](const std::exception& e)
		{
			Log(self, LogLevel::Warning, "Error fetching user info: %s"sv, e.what());
			// on error, we sleep and retry
			self->PAL->Sleep(1000, [self]() { FetchMyOwnUserInfo(self); }, [](const std::exception&) {});
		}
	);
}
void R66ApiImpl::FetchUserInfo(Self self, string login, string id, ResolveFn<UserInfo&&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	string url = STR("https://api.twitch.tv/helix/users"s);
	if (!login.empty()) {
		url += STR("?login="sv);
		url += login;
	}
	else if (!id.empty()) {
		url += STR("?id="sv);
		url += id;
	}
	R66ApiImpl::CallApi(
		self,
		std::make_shared<LoggedInApiCall>(url),
		[self, login, id, resolve, reject](const WebRequestResult& r)
		{
			auto j = Json::Parse(r.ResponseBody);
			auto u = j.at("data").at(0);
			UserInfo userinfo;
			userinfo.ChannelId = u.at("id").to_string();
			userinfo.LoginName = u.at("login").to_string();
			userinfo.DisplayName = u.at("display_name").to_string();
			userinfo.UserType = u.at("type").to_string();
			userinfo.BroadcasterType = u.at("broadcaster_type").to_string();
			userinfo.Description = u.at("description").to_string();
			userinfo.ProfileImageUrl = u.at("profile_image_url").to_string();
			userinfo.OfflineImageUrl = u.at("offline_image_url").to_string();
			userinfo.ViewCount = u.at("view_count").to_int();
			userinfo.CreatedAt = u.at("created_at").to_string();
			try {
				userinfo.Email = u.at("email").to_string();
			}
			catch (const JsonMissingKeyError&) {
			}

			resolve(std::move(userinfo));
		},
		reject
	);
}

void R66ApiImpl::GetMyUserInfo(ResolveFn<const UserInfo&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	// ignore return value of Await, we need to fetch this regardless
	MyUserInfo.Await(std::move(resolve), std::move(reject));
}

void R66ApiImpl::GetUserInfoById(string_view id, ResolveFn<const UserInfo&> resolve, RejectFn reject)
{
	R66ApiImpl::FetchUserInfo(shared_from_this(), string(), string(id), resolve, reject);
}
void R66ApiImpl::GetUserInfoByLoginName(string_view name, ResolveFn<const UserInfo&> resolve, RejectFn reject)
{
	R66ApiImpl::FetchUserInfo(shared_from_this(), string(name), string(), resolve, reject);
}
