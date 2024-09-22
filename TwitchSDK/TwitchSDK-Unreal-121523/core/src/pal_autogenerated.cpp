#include "pal.hpp"
#include "marshalling.hpp"

using namespace R66;
using namespace R66::PAL;
using namespace R66::Marshalling;

void ProxyPAL::WebRequest(
    HttpMethod method,
    R66::string_view uri,
    R66::string_view contentType,
    R66::string_view clientId,
    R66::string_view authorization,
    R66::string_view requestBody,
    std::function<void(const WebRequestResult&)> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedWebRequestRequest req(
            method,
            uri,
            contentType,
            clientId,
            authorization,
            requestBody,
            [cb](const ManagedWebRequestResult& r) { WebRequestResult res; res.HttpStatus = r.HttpStatus; res.ResponseBody = r.ResponseBody;  cb(res); },
            rej
        );
        Impl(Parent, 1126061481, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::Sleep(
    int32_t milliseconds,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedSleepRequest req(
            milliseconds,
            [cb](const ManagedNone&) { cb(); },
            rej
        );
        Impl(Parent, -764117084, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::ReadFile(
    R66::string_view path,
    std::function<void(R66::string_view)> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedReadFileRequest req(
            path,
            [cb](const ManagedPlainString& r) { cb(r.Data); },
            rej
        );
        Impl(Parent, 1884918045, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::WriteFile(
    R66::string_view path,
    R66::string_view data,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedWriteFileRequest req(
            path,
            data,
            [cb](const ManagedNone&) { cb(); },
            rej
        );
        Impl(Parent, 981515029, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::Log(
    LogLevel level,
    R66::string_view message,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedLogRequest req(
            level,
            message,
            [cb](const ManagedNone&) { cb(); },
            rej
        );
        Impl(Parent, -1432180053, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::CreateWebSocket(
    R66::string_view url,
    R66::string_view protocol,
    std::function<void(int32_t)> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedCreateWebSocketRequest req(
            url,
            protocol,
            [cb](const ManagedPlainInt& r) { cb(r.Data); },
            rej
        );
        Impl(Parent, -2108203796, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::SendWebSocketMessage(
    int32_t handle,
    R66::string_view message,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedSendWebSocketMessageRequest req(
            handle,
            message,
            [cb](const ManagedNone&) { cb(); },
            rej
        );
        Impl(Parent, -1797785895, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::RecvWebSocketMessage(
    int32_t handle,
    int32_t timeoutSeconds,
    std::function<void(R66::string_view)> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedRecvWebSocketMessageRequest req(
            handle,
            timeoutSeconds,
            [cb](const ManagedPlainString& r) { cb(r.Data); },
            rej
        );
        Impl(Parent, 1978333561, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

void ProxyPAL::CloseWebSocket(
    int32_t handle,
    std::function<void()> cb,
    std::function<void(const std::exception&)> rej)
{
    try {
        if (!Parent) throw MissingPalImpl("Has disposed!");

        ManagedCloseWebSocketRequest req(
            handle,
            [cb](const ManagedNone&) { cb(); },
            rej
        );
        Impl(Parent, 491657886, &req);
    }
    catch (const std::exception& e) {
        rej(e);
    }
}

