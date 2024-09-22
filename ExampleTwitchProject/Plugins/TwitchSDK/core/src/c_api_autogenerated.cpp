#include <stdexcept>
#include "c_api.hpp"

using namespace R66;
using namespace R66::PAL;
using namespace R66::Marshalling;


void R66Api_GetAuthState(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedNone* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        api->get()->GetAuthState(
            [cb, pl](const AuthState& r) {
                ManagedAuthState managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetAuthenticationInfo(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->GetAuthenticationInfo(
            v,
            [cb, pl](const AuthenticationInfo& r) {
                ManagedAuthenticationInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_LogOut(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedNone* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        api->get()->LogOut(
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetMyUserInfo(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedNone* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        api->get()->GetMyUserInfo(
            [cb, pl](const UserInfo& r) {
                ManagedUserInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetUserInfoById(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->GetUserInfoById(
            v,
            [cb, pl](const UserInfo& r) {
                ManagedUserInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetUserInfoByLoginName(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->GetUserInfoByLoginName(
            v,
            [cb, pl](const UserInfo& r) {
                ManagedUserInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetMyStreamInfo(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedNone* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        api->get()->GetMyStreamInfo(
            [cb, pl](const StreamInfo& r) {
                ManagedStreamInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_QueryStreams(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedStreamQuery* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        StreamQuery v(*p);
        api->get()->QueryStreams(
            v,
            [cb, pl](const StreamQueryResult& r) {
                ManagedStreamQueryResult managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CheckUserSubscription(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->CheckUserSubscription(
            v,
            [cb, pl](const UserSubscriptionCheckResult& r) {
                ManagedUserSubscriptionCheckResult managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CreateClip(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainBool* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->CreateClip(
            v,
            [cb, pl](const ClipInfo& r) {
                ManagedClipInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CreateStreamMarker(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->CreateStreamMarker(
            v,
            [cb, pl](const StreamMarkerInfo& r) {
                ManagedStreamMarkerInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CreatePoll(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPollDefinition* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        PollDefinition v(*p);
        api->get()->CreatePoll(
            v,
            [cb, pl](const PollInfo& r) {
                ManagedPollInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForPollUpdate(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->WaitForPollUpdate(
            v,
            [cb, pl](const PollInfo& r) {
                ManagedPollInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_EndPoll(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEndPollRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EndPollRequest v(*p);
        api->get()->EndPoll(
            v,
            [cb, pl](const PollInfo& r) {
                ManagedPollInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_UnsubscribeFromPoll(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->UnsubscribeFromPoll(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CreatePrediction(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPredictionDefinition* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        PredictionDefinition v(*p);
        api->get()->CreatePrediction(
            v,
            [cb, pl](const PredictionInfo& r) {
                ManagedPredictionInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForPredictionUpdate(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->WaitForPredictionUpdate(
            v,
            [cb, pl](const PredictionInfo& r) {
                ManagedPredictionInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_EndPrediction(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEndPredictionRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EndPredictionRequest v(*p);
        api->get()->EndPrediction(
            v,
            [cb, pl](const PredictionInfo& r) {
                ManagedPredictionInfo managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_UnsubscribeFromPrediction(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedPlainString* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        auto v = p->Data;
        api->get()->UnsubscribeFromPrediction(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_ModifyChannelInformation(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedModifyChannelInfoRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        ModifyChannelInfoRequest v(*p);
        api->get()->ModifyChannelInformation(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_GetBitsLeaderboard(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedBitsLeaderboardRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        BitsLeaderboardRequest v(*p);
        api->get()->GetBitsLeaderboard(
            v,
            [cb, pl](const BitsLeaderboard& r) {
                ManagedBitsLeaderboard managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_ReplaceCustomRewards(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedCustomRewardList* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        CustomRewardList v(*p);
        api->get()->ReplaceCustomRewards(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_ResolveCustomReward(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedCustomRewardResolveRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        CustomRewardResolveRequest v(*p);
        api->get()->ResolveCustomReward(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_SubscribeToEventStream(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamRequest* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamRequest v(*p);
        api->get()->SubscribeToEventStream(
            v,
            [cb, pl](const EventStreamDesc& r) {
                ManagedEventStreamDesc managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForChannelSubscribeEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForChannelSubscribeEvent(
            v,
            [cb, pl](const ChannelSubscribeEvent& r) {
                ManagedChannelSubscribeEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForChannelFollowEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForChannelFollowEvent(
            v,
            [cb, pl](const ChannelFollowEvent& r) {
                ManagedChannelFollowEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForChannelCheerEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForChannelCheerEvent(
            v,
            [cb, pl](const ChannelCheerEvent& r) {
                ManagedChannelCheerEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForCustomRewardEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForCustomRewardEvent(
            v,
            [cb, pl](const CustomRewardEvent& r) {
                ManagedCustomRewardEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForHypeTrainEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForHypeTrainEvent(
            v,
            [cb, pl](const HypeTrainEvent& r) {
                ManagedHypeTrainEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_WaitForChannelRaidEvent(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->WaitForChannelRaidEvent(
            v,
            [cb, pl](const ChannelRaidEvent& r) {
                ManagedChannelRaidEvent managed(r);
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_CloseEventStream(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedEventStreamDesc* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        EventStreamDesc v(*p);
        api->get()->CloseEventStream(
            v,
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
void R66Api_PrepareShutdown(std::shared_ptr<R66::R66Api>* api, const R66::Marshalling::ManagedNone* p, R66::Marshalling::TaskCallback cb, void* pl) {
    RejectCallback rej(cb, pl);
    try {
        if (!p->IsValid()) throw std::runtime_error("wrong parameter type");
        api->get()->PrepareShutdown(
            [cb, pl]() {
                ManagedNone managed;
                cb(&managed, pl);
            },
            rej
        );
    }
    catch (const std::exception& e) {
        rej(e);
    }
}
