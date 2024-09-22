// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TwitchSDK.h"
#include <pal.hpp>

#include <memory>

/**
 *
 */
class WebsocketHolder;
class UnrealPAL : public R66::PAL::PlatformAbstractionLayer, public std::enable_shared_from_this<UnrealPAL>
{
    bool ShuttingDown;

    FCriticalSection WebsocketsLock;
    int NextWebsocketHandle;
    TMap<int, std::shared_ptr<WebsocketHolder>> Websockets;
public:
	UnrealPAL();

    void Shutdown();

    virtual void WebRequest(
        R66::HttpMethod method,
        R66::string_view uri,
        R66::string_view contentType,
        R66::string_view clientId,
        R66::string_view authorization,
        R66::string_view requestBody,
        std::function<void(const R66::WebRequestResult&)> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void Sleep(
        int32_t milliseconds,
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void ReadFile(
        R66::string_view path,
        std::function<void(R66::string_view)> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void WriteFile(
        R66::string_view path,
        R66::string_view data,
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void Log(
        R66::LogLevel level,
        R66::string_view message,
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void CreateWebSocket(
        R66::string_view url,
        R66::string_view protocol,
        std::function<void(int32_t)> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void SendWebSocketMessage(
        int32_t handle,
        R66::string_view message,
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void RecvWebSocketMessage(
        int32_t handle,
        int32_t timeoutSeconds,
        std::function<void(R66::string_view)> cb,
        std::function<void(const std::exception&)> rej
    ) override;
    virtual void CloseWebSocket(
        int32_t handle,
        std::function<void()> cb,
        std::function<void(const std::exception&)> rej
    ) override;
};
