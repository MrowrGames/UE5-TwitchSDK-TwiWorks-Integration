#include "r66_impl.hpp"


#define AUTH_DATA_FILE STR("twitch-r66-authorization.json"sv)

void R66::R66ApiImpl::LoadAuthTokens(Self self) {
	self->PAL->ReadFile(
		AUTH_DATA_FILE,
		[self](string_view data)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			if (data.length() > 0)
			{
				try {
					auto j = Json::Parse(data);
					R66ApiImpl::TryParseAuthTokens(self, j);
					Log(self, LogLevel::Debug, "Using auth info from existing data file"sv);
				}
				catch (const std::exception& e)
				{
					Log(self, LogLevel::Debug, "Error while trying to parse existing data file: %s"sv, e.what());
				}
			}

			InitDone(self);
		},
		[self](const std::exception& e)
		{
			Log(self, LogLevel::Debug, "Error while trying to read existing data file: %s"sv, e.what());
			InitDone(self);
		}
	);
}

void R66ApiImpl::TryParseAuthTokens(Self self, const Json& j)
{
	AuthorizationResult ar;
	ar.AccessToken = j.at("access_token").to_string();
	ar.RefreshToken = j.at("refresh_token").to_string();
	auto scopes = j.at("scope");
	auto num_scopes = scopes.array_size();
	for (size_t i = 0; i < num_scopes; i++)
	{
		ar.Scopes.push_back(scopes.at(i).to_string());
	}
	self->AuthResult.Resolve(std::move(ar));
	self->MyAuthStatus = AuthStatus::LoggedIn;
}

void R66ApiImpl::RefreshAccessToken(Self self)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	if (!self->AuthResult.IsDone()) return; // race probably

	string refresh = self->AuthResult.Unwrap().RefreshToken;
	self->AuthResult.Invalidate();

	string uri = STR("https://id.twitch.tv/oauth2/token?grant_type=refresh_token"s);
	uri += STR("&refresh_token="sv);
	uri += refresh;
	uri += STR("&client_id="sv);
	uri += self->ClientId;
	self->PAL->WebRequest(
		HttpMethod::Post,
		uri,
		STR("text/plain"sv),
		string_view(),
		string_view(),
		string_view(),
		[self](const WebRequestResult& r)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			if (r.HttpStatus == 400)
			{
				// probably "Invalid OAuth token" meaning that we have been logged out
				Deauthenticate(self, true);
				return;
			}
			R66ApiImpl::TryParseAuthTokens(self, Json::Parse(r.ResponseBody));

			self->PAL->WriteFile(
				AUTH_DATA_FILE,
				r.ResponseBody,
				[self]()
				{
					Log(self, LogLevel::Debug, "Auth token file successfully written!"sv);
				},
				[self](const std::exception& e)
				{
					Log(self, LogLevel::Warning, "Failed to write auth token file: %s"sv, e.what());
				}
			);
		},
		[self](const std::exception&)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);
			// on error, give up and make sure that the user can reauthenticate
			// TODO: retry here
			self->AuthInfo.Invalidate();
		}
	);
}

void R66::R66ApiImpl::Deauthenticate(Self self, bool deletePersistently) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	self->MyAuthStatus = AuthStatus::LoggedOut;
	self->AuthResult.Invalidate();
	self->AuthInfo.Invalidate();
	self->CachedCustomRewardList.Invalidate();
	if (deletePersistently)
	{
		self->PAL->WriteFile(
			AUTH_DATA_FILE,
			string_view(), // overwrite with empty file
			[]() {},
			[](const std::exception&) {}
		);
	}
}

void R66ApiImpl::CheckAuthenticationState(Self self)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	string requestBody;
	requestBody += STR("client_id="sv);
	EncodeUriComponent(requestBody, self->ClientId);
	requestBody += STR("&device_code="sv);
	EncodeUriComponent(requestBody, self->AuthInfo.Unwrap().DeviceCode);

	auto reject = [self](const std::exception& e)
	{
		std::lock_guard<std::recursive_mutex> lock(self->Mutex);
		Log(self, LogLevel::Info, "Fatal error when waiting for code result: %s"sv, e.what());
		// unset AuthInfo and start over
		self->AuthInfo.Invalidate();
		self->MyAuthStatus = AuthStatus::LoggedOut;
	};
	self->PAL->WebRequest(
		HttpMethod::Post,
		STR("https://id.twitch.tv/oauth2/token?grant_type=urn:ietf:params:oauth:grant-type:device_code"sv),
		STR("application/x-www-form-urlencoded"sv),
		string_view(),
		string_view(),
		requestBody,
		[self, reject](const WebRequestResult& r)
		{
			auto j = Json::Parse(r.ResponseBody);
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			try {
				R66ApiImpl::TryParseAuthTokens(self, j);
			}
			catch (const JsonMissingKeyError&) {
				if (j.at("message").to_std_string() == "authorization_pending"sv)
				{
					// waiting/pending
					self->PAL->Sleep(self->AuthInfo.Unwrap().Interval * 1000, [self]() { CheckAuthenticationState(self); }, reject);
					return;
				}
				else {
					throw;
				}
			}

			self->PAL->WriteFile(
				AUTH_DATA_FILE,
				r.ResponseBody,
				[self]()
				{
					Log(self, LogLevel::Debug, "Auth token file successfully written!"sv);
				},
				[self](const std::exception& e)
				{
					Log(self, LogLevel::Warning, "Failed to write auth token file: %s"sv, e.what());
				}
			);
		},
		reject
	);
}


void R66ApiImpl::GetAuthenticationInfo(string_view scopes, ResolveFn<const AuthenticationInfo&> resolve, RejectFn reject)
{
	Initialized.Await([self = shared_from_this(), scopes = string(scopes), resolve, reject](const VoidResult&) {
		R66ApiImpl::FetchAuthenticationInfo(self, scopes, resolve, reject);
	});
}

void R66ApiImpl::FetchAuthenticationInfo(Self self, string scopes, ResolveFn<const AuthenticationInfo&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	if (self->AuthResult.IsDone())
	{
		Log(self, LogLevel::Info, "Already authenticated!"sv);
		resolve(AuthenticationInfo());
		return;
	}

	if (!self->AuthInfo.Await([r = std::move(resolve)](const AuthenticationInfoInternal& ai) { r(ai.Inner); }, std::move(reject)))
	{
		return;
	}

	Log(self, LogLevel::Debug, "Starting authorization flow"sv);
	self->MyAuthStatus = AuthStatus::Loading;

	string requestBody;
	requestBody += STR("client_id="sv);
	EncodeUriComponent(requestBody, self->ClientId);
	requestBody += STR("&scopes="sv);
	EncodeUriComponent(requestBody, scopes);

	self->PAL->WebRequest(
		HttpMethod::Post,
		STR("https://id.twitch.tv/oauth2/device"sv),
		STR("application/x-www-form-urlencoded"sv),
		string_view(),
		string_view(),
		requestBody,
		[self](const WebRequestResult& r)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			if (r.HttpStatus != 200)
			{
				// throw in order to drop directly into the reject path below
				BadHttpResponse(r, [&](const std::exception& e) { throw e; });
			}

			auto j = Json::Parse(r.ResponseBody);
			AuthenticationInfoInternal ai;
			ai.Inner.Uri = j.at("verification_uri").to_string();
			ai.Inner.UserCode = j.at("user_code").to_string();
			ai.DeviceCode = j.at("device_code").to_string();
			ai.Interval = (int)j.at("interval").to_int();
			self->AuthInfo.Resolve(std::move(ai));
			// We ignore expires_in as recommened at https://dev.twitch.tv/docs/authentication

			// launch the worker that periodically polls the authentication state
			// while this worker is running, ai.IsSet is true so that we will only
			// request new authinfo once the worker errors (for whatever reason)
			self->MyAuthStatus = AuthStatus::WaitingForCode;
			R66ApiImpl::CheckAuthenticationState(self);
		},
		[self](const std::exception& e)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);
			self->MyAuthStatus = AuthStatus::LoggedOut;

			self->AuthInfo.Reject(e);
			// invalidate right away so that they can try again
			self->AuthInfo.Invalidate();
		}
	);
}

void R66::R66ApiImpl::GetAuthState(ResolveFn<const AuthState&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	AuthState as;
	as.Status = MyAuthStatus;
	if (AuthResult.IsDone())
	{
		for (auto& scope : AuthResult.Unwrap().Scopes)
		{
			as.Scopes.emplace_back(scope);
		}
	}
	resolve(as);
}

void R66::R66ApiImpl::LogOut(ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	R66ApiImpl::Deauthenticate(shared_from_this(), true);
	resolve();
}
