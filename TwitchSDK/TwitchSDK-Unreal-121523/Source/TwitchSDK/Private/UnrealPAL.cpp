#include "UnrealPAL.h"

#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/Online/WebSockets/Public/WebSocketsModule.h"
#include "Runtime/Online/WebSockets/Public/IWebSocket.h"
#include "Containers/Ticker.h"
#include "Containers/Queue.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Async/Async.h"

using namespace R66;

UnrealPAL::UnrealPAL()
    : ShuttingDown(false), NextWebsocketHandle(0)
{
}

void UnrealPAL::Shutdown()
{
    UE_LOG(LogTwitchSDK, Log, TEXT("Shutting down"));
    ShuttingDown = true;
    Websockets.Empty(0);
    // The way we currently handle shutdown is that we simply stop responding to PAL calls.
    // The reason this does not leak memory is because we correctly drop all std::function objects.
    // That should release the associated shared_ptr instances and ideally the entire plugin.
}


void UnrealPAL::WebRequest(
    R66::HttpMethod method,
    R66::string_view uri,
    R66::string_view contentType,
    R66::string_view clientId,
    R66::string_view authorization,
    R66::string_view requestBody,
    std::function<void(const R66::WebRequestResult&)> cb,
    std::function<void(const std::exception&)> rej
)
{
    if (ShuttingDown) return;

    FHttpRequestRef req = FHttpModule::Get().CreateRequest();

    bool hasBody = false;
    switch (method)
    {
    case R66::HttpMethod::Get: req->SetVerb(TEXT("GET")); break;
    case R66::HttpMethod::Post: req->SetVerb(TEXT("POST")); hasBody = true; break;
    case R66::HttpMethod::Put: req->SetVerb(TEXT("PUT")); hasBody = true; break;
    case R66::HttpMethod::Patch: req->SetVerb(TEXT("PATCH")); hasBody = true; break;
    case R66::HttpMethod::Delete: req->SetVerb(TEXT("DELETE")); break;
    default: rej(std::runtime_error("unimplemented verb")); return;
    }

    req->SetURL(ToFString(uri));

    if (hasBody)
    {
        req->SetContentAsString(ToFString(requestBody));
        req->SetHeader(TEXT("Content-Type"), ToFString(contentType) + TEXT("; charset=utf-8"));
    }

    if (clientId.size() > 0)
    {
        req->SetHeader(TEXT("Client-Id"), ToFString(clientId));
    }
    if (authorization.size() > 0)
    {
        req->SetHeader(TEXT("Authorization"), ToFString(authorization));
    }

    FString userAgent = FString::Printf(TEXT("%s Twitch-Route-66-Unreal/0.2"), *FGenericPlatformHttp::GetDefaultUserAgent());
    req->SetHeader(TEXT("User-Agent"), userAgent);

    req->OnProcessRequestComplete().BindLambda([cb, rej](FHttpRequestPtr req, FHttpResponsePtr resp, bool success) {
        try {
            if (!success)
            {
                throw std::runtime_error("unable to connect");
            }

            R66::WebRequestResult wrr;
            wrr.HttpStatus = resp->GetResponseCode();
            FString body = resp->GetContentAsString();
            wrr.ResponseBody = FromFString(body);
            UE_LOG(LogTwitchSDK, Log, TEXT("http complete %d %d %s"), success, wrr.HttpStatus, *body);
            cb(wrr);
        }
        catch (const std::exception& e) {
            rej(e);
        }
    });
    req->ProcessRequest();
}
void UnrealPAL::Sleep(
    int32_t milliseconds,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej
)
{
    if (ShuttingDown) return;

    FTSTicker::GetCoreTicker().AddTicker(
        TEXT("TwitchSDK"),
        ((float)milliseconds) / 1000.f,
        [cb, rej](float) {
            try
            {
                cb();
            }
            catch (const std::exception& e)
            {
                rej(e);
            }

            return false;
        }
    );
}
void UnrealPAL::ReadFile(
    R66::string_view path,
    std::function<void(R66::string_view)> cb,
    std::function<void(const std::exception&)> rej
)
{
    if (ShuttingDown) return;

    FString fpath = FPaths::Combine(FPaths::ProjectSavedDir(), ToFString(path));
    Async(
        EAsyncExecution::ThreadPool,
        [cb, rej, fpath]() {
            FString outs;
            if (FFileHelper::LoadFileToString(outs, *fpath))
            {
                try {
                    cb(FromFString(outs));
                }
                catch (const std::exception& e) {
                    rej(e);
                }
            }
            else
            {
                rej(std::runtime_error("LoadFileToString failed"));
            }
        }
    );
}

void UnrealPAL::WriteFile(
    R66::string_view path,
    R66::string_view data,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej
)
{
    if (ShuttingDown) return;

    FString fpath = FPaths::Combine(FPaths::ProjectSavedDir(), ToFString(path));
    FString fdata(ToFString(data));
    Async(
        EAsyncExecution::ThreadPool,
        [cb, rej, fpath, fdata]() {
            if (FFileHelper::SaveStringToFile(fdata, *fpath))
            {
                try {
                    cb();
                }
                catch (const std::exception& e) {
                    rej(e);
                }
            }
            else
            {
                rej(std::runtime_error("SaveStringToFile failed"));
            }
        }
    );
}
void UnrealPAL::Log(
    R66::LogLevel level,
    R66::string_view message,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej
)
{
    FString msg = ToFString(message);
    switch (level)
    {
    case R66::LogLevel::Debug: UE_LOG(LogTwitchSDK, Verbose, TEXT("%s"), *msg); break;
    case R66::LogLevel::Warning: UE_LOG(LogTwitchSDK, Warning, TEXT("%s"), *msg); break;
    case R66::LogLevel::Error: UE_LOG(LogTwitchSDK, Error, TEXT("%s"), *msg); break;
    default: UE_LOG(LogTwitchSDK, Display, TEXT("%s"), *msg); break;
    }
}


class WebsocketHolder : public std::enable_shared_from_this<WebsocketHolder> {
    FCriticalSection Lock;
    bool IsClosed = false;
    TQueue<FString> MessageQueue;
    TQueue<std::tuple<R66::ResolveFn<R66::string_view>, R66::RejectFn>> WaitForMessageCallbacks;

    FTickerDelegate TimeoutDelegate;
    FTSTicker::FDelegateHandle TimeoutHandle;
    int32_t CurrentTimeout = 100;
public:
    TSharedRef<IWebSocket> Socket;
    WebsocketHolder(const FString& url, const FString& protocol)
        : Socket(FWebSocketsModule::Get().CreateWebSocket(url, protocol)) {
    }

    void Connect(
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) {
        // because these callbacks are stored for the lifetime of the socket, we need to
        // use a weak_ptr reference instead of a shared_ptr or else we create a reference
        // cycle between WebsocketHolder and the IWebSocket
        //
        // In every calback, we simply exit if the WebsocketHolder is dead.
        // Note that we do not bother to unregister because the IWebSocket is supposed to go
        // away soon regardless.
        std::weak_ptr<WebsocketHolder> wself = shared_from_this();
        Socket->OnClosed().AddLambda(
            [wself](int32 statusCode, const FString& reason, bool wasClean)
            {
                auto self = wself.lock();
                if (!self) return;

                FScopeLock lock(&self->Lock);
                self->IsClosed = true;

                // drain all callbacks
                std::tuple<R66::ResolveFn<R66::string_view>, R66::RejectFn> cb;
                std::runtime_error exc("websocket closed");
                while (self->WaitForMessageCallbacks.Dequeue(cb)) {
                    std::get<1>(cb)(exc);
                }
            }
        );
        Socket->OnMessage().AddLambda(
            [wself](const FString& fs)
            {
                auto self = wself.lock();
                if (!self) return;

                FScopeLock lock(&self->Lock);

                // either someone is waiting to receive a message already
                // or we need to enqueue it so they can receive it later

                std::tuple<R66::ResolveFn<R66::string_view>, R66::RejectFn> waitingCallback;
                if (self->WaitForMessageCallbacks.Dequeue(waitingCallback)) {
                    std::get<0>(waitingCallback)(FromFString(fs));
                }
                else {
                    self->MessageQueue.Enqueue(fs);
                }

                // reset timeout timer
                auto& coreTicker = FTSTicker::GetCoreTicker();
                if (self->TimeoutHandle.IsValid())
                {
                    coreTicker.RemoveTicker(self->TimeoutHandle);
                }
                self->TimeoutHandle = coreTicker.AddTicker(self->TimeoutDelegate, self->CurrentTimeout);
            }
        );
        TimeoutDelegate.BindLambda([wself] (float) {
            auto self = wself.lock();
            if (!self) return false;

            FScopeLock lock(&self->Lock);
            self->Socket->Close();
            // this should simply trigger the OnClosed() handler above and cause a clean shutdown of the websocket

            return false;
        });
        // on the error path, the rejection callback causes the websocket to be removed from the UnrealPAL
        // which then deallocates the WebsocketHolder, which should deallocate the IWebSocket, which should
        // deallocate the error callback, which should deallocate the shared_ptr to the UnrealPAL
        Socket->OnConnectionError().AddLambda(
            [rej](const FString& fs)
            {
                auto utf8 = R66::to_utf8(FromFString(fs)).ToOwned();
                rej(std::runtime_error(utf8));
            }
        );
        // on the success path, cb does not contain any references in the first place since it merely
        // returns an integer upstream.
        // callers then index into the map using this handle.
        Socket->OnConnected().AddLambda(std::move(cb));


        Socket->Connect();
    }

    void Recv(
        std::function<void(R66::string_view)> cb,
        std::function<void(const std::exception&)> rej
    ) {
        FScopeLock lock(&Lock);

        FString fs;
        if (MessageQueue.Dequeue(fs)) {
            try {
                cb(FromFString(fs));
            }
            catch (const std::exception& e) {
                rej(e);
            }
            return;
        }

        if (IsClosed) {
            rej(std::runtime_error("websocket closed"));
            return;
        }

        WaitForMessageCallbacks.Enqueue(std::make_tuple(std::move(cb), std::move(rej)));
    }

    ~WebsocketHolder() {
        // NB: Unreal treats it as an error when you forget to invoke Close() before dropping the object
        Socket->Close();
    }
};

void UnrealPAL::CreateWebSocket(
    R66::string_view url,
    R66::string_view protocol,
    std::function<void(int32_t)> cb,
    std::function<void(const std::exception&)> rej
)
{
    FScopeLock lock(&WebsocketsLock);
    int handle = NextWebsocketHandle++;
    auto ws = std::make_shared<WebsocketHolder>(ToFString(url), ToFString(protocol));
    Websockets.Add(handle, ws);
    ws->Connect(
        [cb, rej, handle]()
        {
            try {
                cb(handle);
            }
            catch (const std::exception& e) {
                // TODO: this potentially leaks a websocket, which would be very bad
                rej(e);
            }
        },
        [rej, handle, self = shared_from_this()](const std::exception& e)
        {
            // if opening the websocket did not work, we must remove it from the map
            FScopeLock lock(&self->WebsocketsLock);
            self->Websockets.Remove(handle);
            rej(e);
        }
    );

}
void UnrealPAL::SendWebSocketMessage(
    int32_t handle,
    R66::string_view message,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej
)
{
    FScopeLock lock(&WebsocketsLock);
    Websockets[handle]->Socket->Send(ToFString(message));
    try {
        cb();
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void UnrealPAL::RecvWebSocketMessage(
    int32_t handle,
    int32_t timeoutSeconds,
    std::function<void(R66::string_view)> cb,
    std::function<void(const std::exception&)> rej
)
{
    FScopeLock lock(&WebsocketsLock);
    Websockets[handle]->Recv(cb, rej);
}
void UnrealPAL::CloseWebSocket(
    int32_t handle,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej
)
{
    FScopeLock lock(&WebsocketsLock);
    Websockets.Remove(handle);
    try {
        cb();
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
