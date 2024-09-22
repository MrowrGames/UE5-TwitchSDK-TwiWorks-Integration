#include "r66_impl.hpp"


static PredictionInfo ParsePredictionInfo(string_view json) {
	PredictionInfo res;

	try
	{
		auto j = Json::Parse(json);
		auto p = j.at("data").at(0);
		res.Id = p.at("id").to_string();
		res.BroadcasterId = p.at("broadcaster_id").to_string();
		res.Title = p.at("title").to_string();
		res.WinningOutcomeId = json_tostring(p.at("winning_outcome_id"));

		auto outcomes = p.at("outcomes");
		for (size_t i = 0; i < outcomes.array_size(); i++)
		{
			auto outcome = outcomes.at(i);
			PredictionOutcome po;
			po.Id = outcome.at("id").to_string();
			po.Title = outcome.at("title").to_string();
			po.Users = outcome.at("users").to_int();
			po.ChannelPoints = outcome.at("channel_points").to_int();
			po.Color = outcome.at("color").to_string();
			res.Outcomes.push_back(std::move(po));
		}

		// res.PredictionWindow = p.at("prediction_window").to_int();
		auto status = p.at("status").to_string();
		if (status == STR("ACTIVE"sv))
			res.Status = PredictionStatus::Active;
		else if (status == STR("LOCKED"sv))
			res.Status = PredictionStatus::Locked;
		else if (status == STR("RESOLVED"sv))
			res.Status = PredictionStatus::Resolved;
		else if (status == STR("CANCELED"sv))
			res.Status = PredictionStatus::Canceled;
		else
			throw std::runtime_error("invalid prediction state: "s + to_utf8(status));

		res.CreatedAt = p.at("created_at").to_string();
		res.EndedAt = json_tostring(p.at("ended_at"));
		res.LockedAt = json_tostring(p.at("locked_at"));
	}
	catch (...)
	{
	}

	return res;
}
enum class Phase {
	Progress,
	Lock,
	End,
};
static PredictionInfo ParseESPredictionInfo(const JsonRef& j, Phase phase) {
	PredictionInfo res;

	try
	{
		auto p = j.at("event");
		res.Id = p.at("id").to_string();
		res.BroadcasterId = p.at("broadcaster_user_id").to_string();
		res.Title = p.at("title").to_string();

		auto outcomes = p.at("outcomes");
		for (size_t i = 0; i < outcomes.array_size(); i++)
		{
			auto outcome = outcomes.at(i);
			PredictionOutcome po;
			po.Id = outcome.at("id").to_string();
			po.Title = outcome.at("title").to_string();
			po.Users = outcome.at("users").to_int();
			po.ChannelPoints = outcome.at("channel_points").to_int();
			po.Color = outcome.at("color").to_string();
			res.Outcomes.push_back(std::move(po));
		}

		res.CreatedAt = p.at("started_at").to_string();
		switch (phase)
		{
		case Phase::Progress:
			res.Status = PredictionStatus::Active;
			res.LockedAt = json_tostring(p.at("locks_at"));
			break;
		case Phase::Lock:
			res.Status = PredictionStatus::Locked;
			res.LockedAt = json_tostring(p.at("locked_at"));
			break;
		case Phase::End:
			auto status = p.at("status").to_string();
			if (status == STR("resolved"sv))
				res.Status = PredictionStatus::Resolved;
			else if (status == STR("canceled"sv))
				res.Status = PredictionStatus::Canceled;
			else
				throw std::runtime_error("invalid prediction state: "s + to_utf8(status));

			res.EndedAt = json_tostring(p.at("ended_at"));
			break;
		}

	}
	catch (...)
	{
		// FIXME: remove, temp
		throw;
	}

	return res;
}

void R66ApiImpl::ESEventPredictionUpdate(Self self, const JsonRef& json, string_view key) {
	Phase p;
	if (key == PredictionProgress.Type)
		p = Phase::Progress;
	else if (key == PredictionLock.Type)
		p = Phase::Lock;
	else if (key == PredictionEnd.Type)
		p = Phase::End;
	else
		throw std::runtime_error("bad key");

	auto info = ParseESPredictionInfo(json, p);
	Log(self, LogLevel::Debug, "[ES] got poll update for %s"sv, to_utf8(info.Id));
	self->PendingPredictionInfoUpdates[info.Id.ToOwned()].GetTask().ResolveSPMC(std::move(info));
	PollPredictionStatus<PredictionInfo>::Cleanup(self->PendingPredictionInfoUpdates);
}

void R66ApiImpl::CreatePrediction(const PredictionDefinition& def, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	MyUserInfo.Await(
		[self = shared_from_this(), def = std::make_shared<PredictionDefinition>(def), resolve, reject](const UserInfo& ui)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			Json body;
			body.set_property("broadcaster_id", Json(ui.ChannelId));
			body.set_property("title", Json(def->Title));
			body.set_property("prediction_window", Json(def->Duration));

			std::vector<Json> choices(def->Outcomes.size());
			for (size_t i = 0; i < choices.size(); i++)
			{
				choices[i].set_property("title", Json(def->Outcomes[i]));
			}
			body.set_array("outcomes", std::move(choices));

			R66ApiImpl::ESAddSubscription(self, PredictionProgress);
			R66ApiImpl::ESAddSubscription(self, PredictionLock);
			R66ApiImpl::ESAddSubscription(self, PredictionEnd);

			R66ApiImpl::CallApi(
				self,
				std::make_shared<LoggedInApiCall>(STR("https://api.twitch.tv/helix/predictions"s), body.serialize()),
				[self, def, resolve, reject](const WebRequestResult& r) {
					std::lock_guard<std::recursive_mutex> lock(self->Mutex);

					auto res = ParsePredictionInfo(r.ResponseBody);
					resolve(res);
				},
				reject
			);
		}
	);
}

void R66ApiImpl::EndPrediction(const EndPredictionRequest& req, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	Json body;
	body.set_property("broadcaster_id", Json(req.BroadcasterId));
	body.set_property("id", Json(req.PredictionId));
	string_view status;
	switch (req.Status)
	{
	case PredictionStatus::Active:
		throw std::runtime_error("Passing PredictionStatus::Active for EndPrediction() is invalid!");
	case PredictionStatus::Locked:
		status = STR("LOCKED"sv);
		break;
	case PredictionStatus::Resolved:
		status = STR("RESOLVED"sv);
		break;
	case PredictionStatus::Canceled:
		status = STR("CANCELED"sv);
		break;
	}
	body.set_property("status", Json(status));
	body.set_property("winning_outcome_id", Json(req.WinningOutcomeId));

	R66ApiImpl::CallApi(
		shared_from_this(),
		std::make_shared<LoggedInApiCall>(STR("https://api.twitch.tv/helix/predictions"s), body.serialize(), HttpMethod::Patch),
		[resolve, reject](const WebRequestResult& r) 		{
			auto res = ParsePredictionInfo(r.ResponseBody);
			resolve(res);
		},
		reject
	);
}

void R66ApiImpl::WaitForPredictionUpdate(string_view pollId, ResolveFn<const PredictionInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	PendingPredictionInfoUpdates[string(pollId)].GetTask().AwaitSPMC(std::move(resolve), std::move(reject));
	PollPredictionStatus<PredictionInfo>::Cleanup(PendingPredictionInfoUpdates);
}

void R66ApiImpl::UnsubscribeFromPrediction(string_view id, ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	R66ApiImpl::ESDelSubscription(shared_from_this(), PredictionProgress);
	R66ApiImpl::ESDelSubscription(shared_from_this(), PredictionLock);
	R66ApiImpl::ESDelSubscription(shared_from_this(), PredictionEnd);
	resolve();
}
