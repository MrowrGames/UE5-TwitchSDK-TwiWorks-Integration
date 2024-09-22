#include "r66_impl.hpp"


R66ApiImpl::R66ApiImpl(std::shared_ptr<PlatformAbstractionLayer> pal, string_view clientId, bool useESProxy)
	: PAL(pal), ClientId(clientId), MyAuthStatus(AuthStatus::Loading), EventSubUseProxy(useESProxy)
{
}

std::shared_ptr<R66Api> R66Api::Initialize(
	std::shared_ptr<PlatformAbstractionLayer> pal,
	string_view clientId,
	bool useEventSubProxy
)
{
	auto s = std::make_shared<R66ApiImpl>(pal, clientId, useEventSubProxy);
	s->Init();
	return s;
}

void R66::R66ApiImpl::UpdateOAuthCredentials(string_view clientId) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	ClientId = clientId;
}

void R66ApiImpl::InitDone(Self self)
{
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);

	R66ApiImpl::FetchMyOwnUserInfo(self);

	if (self->MyAuthStatus == AuthStatus::Loading)
	{
		self->MyAuthStatus = AuthStatus::LoggedOut;
	}

	self->Initialized.Resolve(VoidResult());
}

void R66ApiImpl::Init()
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);

	if (Initialized.IsDone())
	{
		// invoking this too often is allowed and does nothing
		return;
	}

	auto clientId = to_utf8(ClientId);
	Log(shared_from_this(), LogLevel::Debug, "Initializing with client id %s"sv, (std::string_view)clientId);

	R66ApiImpl::LoadAuthTokens(shared_from_this());
	R66ApiImpl::ESAddSubscription(shared_from_this(), StreamOnline);
	R66ApiImpl::ESAddSubscription(shared_from_this(), StreamOffline);
	R66ApiImpl::ESAddSubscription(shared_from_this(), ChannelUpdate);
	R66ApiImpl::ESAddSubscription(shared_from_this(), UserUpdate);
	R66ApiImpl::ESConnect(shared_from_this());
}

void R66ApiImpl::RefreshOAuthToken(ResolveFn<string_view> resolve) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	R66ApiImpl::RefreshAccessToken(shared_from_this());
	AuthResult.Await(
		[resolve](const AuthorizationResult& r) {
				resolve(r.AccessToken);
		}
	);
}

void BadHttpResponse(const R66::WebRequestResult& web, const R66::RejectFn& reject) {
	int64_t istatus = -1;
	std::string error, status, message;
	try
	{
		auto j = Json::Parse(web.ResponseBody);
		message = j.at("message").to_std_string();
		istatus = j.at("status").to_int();
		status = std::to_string(istatus);
		error = j.at("error").to_std_string();
	}
	catch (...)
	{
	}
	std::string rejection;
	if (error.size() + status.size() + message.size())
	{
		rejection += status;
		rejection += " "sv;
		rejection += error;
		rejection += ": "sv;
		rejection += message;
	}
	else
	{
		rejection += "Unable to handle response from Twitch API: "sv;
		rejection += to_utf8(web.ResponseBody);
	}
	reject(BadHttpResponseException(rejection, istatus));
}

// General TODOs:
//
// TODO: verify that we don't run into reference cycles with shared_ptrs in task callbacks here
// TODO: there is a race condition where a (very slow) call uses expired credentials, then another request is faster,
// triggers refresh, and the refresh completes before the first call fails with 401, causing it to drop the new credentials
// and request new ones AGAIN. Most likely this is so rare that we don't care, but we need to test.
// TODO: biggest TODO is timeouts for things so we don't hang forever

void R66::R66ApiImpl::CallApi(Self self, std::shared_ptr<LoggedInApiCall> call, ResolveFn<const WebRequestResult&> resolve, RejectFn reject, string authHeaderOverride) {
	std::lock_guard<std::recursive_mutex> lock(self->Mutex);
	self->AuthResult.Await(
		[self, call, resolve, reject, authHeaderOverride](const AuthorizationResult& ar)
		{
			auto authHeader = ar.AuthorizationHeader();
			if (authHeaderOverride.size())
			{
				authHeader = authHeaderOverride;
			}
			self->PAL->WebRequest(
				call->Method,
				call->Uri,
				call->ContentType,
				self->ClientId,
				authHeader,
				call->RequestBody,
				[self, call, resolve, reject, authHeaderOverride](const WebRequestResult& r)
				{
					if (r.HttpStatus == 401)
					{
						// theoretically, we could check the message here:
						// {"error":"Unauthorized", "status" : 401, "message" : "Invalid OAuth token"}
						// {"error":"Unauthorized", "status" : 401, "message" : "Missing scope: user:edit:broadcast or channel:manage:broadcast or channel_editor"}
						// However, docs give no guidance on this other than to simply retry once.
						if (call->IsReauthenticated)
						{
							// most likely missing scopes
							BadHttpResponse(r, reject);
						}
						else
						{
							RefreshAccessToken(self);
							call->IsReauthenticated = true;
							CallApi(self, call, resolve, reject);
						}
					}
					// TODO: handle 429 correctly (other than dumb retry)
					else if ((r.HttpStatus >= 500 && r.HttpStatus < 600) || r.HttpStatus == 429)
					{
						// trigger retry on 5XX
						if (!CallApiRetry(self, call, resolve, reject, authHeaderOverride))
						{
							BadHttpResponse(r, reject);
						}
					}
					// By default, we allow 404 and 2XX status codes to proceed, and treat everything else as an error.
					else if (r.HttpStatus == 404 || (r.HttpStatus >= 200 && r.HttpStatus < 300))
					{
						resolve(r);
					}
					else
					{
						BadHttpResponse(r, reject);
					}
				},
				[self, call, resolve, reject, authHeaderOverride](const std::exception& e)
				{
					if (!CallApiRetry(self, call, resolve, reject, authHeaderOverride))
					{
						reject(e);
					}
				}
			);
		}
	);
}
#define MAX_REQUEST_RETRY 3
#define REQUEST_RETRY_SLEEP_MS 1000
bool R66::R66ApiImpl::CallApiRetry(Self self, std::shared_ptr<LoggedInApiCall> call, ResolveFn<const WebRequestResult&> resolve, RejectFn reject, string authHeaderOverride) {
	if (call->RetryCount >= MAX_REQUEST_RETRY)
	{
		return false;
	}

	call->RetryCount++;
	self->PAL->Sleep(REQUEST_RETRY_SLEEP_MS,
		[=]() { CallApi(self, call, resolve, reject, authHeaderOverride); },
		[](const std::exception&) {}
	);
	return true;
}

void R66::R66ApiImpl::PrepareShutdown(ResolveFn<void> resolve, RejectFn reject) {
	ReplaceCustomRewards(CustomRewardList(), resolve, reject);
}
