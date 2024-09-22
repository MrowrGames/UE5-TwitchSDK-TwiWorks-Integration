#include "r66_impl.hpp"

static PollInfo ParsePollInfo(string_view json) {
	PollInfo res;

	try {
		auto j = Json::Parse(json);
		auto p = j.at("data").at(0);
		res.Id = p.at("id").to_string();
		res.BroadcasterId = p.at("broadcaster_id").to_string();
		res.Title = p.at("title").to_string();

		auto choices = p.at("choices");
		for (size_t i = 0; i < choices.array_size(); i++) {
			auto choice = choices.at(i);
			PollChoiceInfo pci;
			pci.Id = choice.at("id").to_string();
			pci.Title = choice.at("title").to_string();
			pci.Votes = choice.at("votes").to_int();
			pci.ChannelPointsVotes = choice.at("channel_points_votes").to_int();
			res.Choices.push_back(std::move(pci));
		}

		res.ChannelPointsVotingEnabled = p.at("channel_points_voting_enabled").to_bool();
		res.ChannelPointsPerVote = (int32_t)p.at("channel_points_per_vote").to_int();

		auto status = p.at("status").to_string();
		if (status == STR("ACTIVE"sv))
			res.Status = PollStatus::Active;
		else if (status == STR("COMPLETED"sv))
			res.Status = PollStatus::Completed;
		else if (status == STR("ARCHIVED"sv))
			res.Status = PollStatus::Archived;
		else if (status == STR("TERMINATED"sv))
			res.Status = PollStatus::Terminated;
		else if (status == STR("MODERATED"sv))
			res.Status = PollStatus::Moderated;
		else
			res.Status = PollStatus::Invalid;

		// res.Duration = p.at("duration").to_int();
		res.StartedAt = p.at("started_at").to_string();
		res.EndedAt = p.at("ended_at").to_string(); // optional, may be missing and throw (but fine because it's the last one)
	}
	catch (...) {
	}

	return res;
}

static PollInfo ParseESPollInfo(const JsonRef& j, bool hasEnded) {
	PollInfo res;

	try {
		auto p = j.at("event");
		res.Id = p.at("id").to_string();
		res.BroadcasterId = p.at("broadcaster_user_id").to_string();
		res.Title = p.at("title").to_string();

		auto choices = p.at("choices");
		for (size_t i = 0; i < choices.array_size(); i++) {
			auto choice = choices.at(i);
			PollChoiceInfo pci;
			pci.Id = choice.at("id").to_string();
			pci.Title = choice.at("title").to_string();
			pci.Votes = choice.at("votes").to_int();
			pci.ChannelPointsVotes = choice.at("channel_points_votes").to_int();
			res.Choices.push_back(std::move(pci));
		}

		res.ChannelPointsVotingEnabled = p.at("channel_points_voting").at("is_enabled").to_bool();
		res.ChannelPointsPerVote = (int32_t)p.at("channel_points_voting").at("amount_per_vote").to_int();

		if (hasEnded)
		{
			auto status = p.at("status").to_string();
			if (status == STR("completed"sv))
				res.Status = PollStatus::Completed;
			else if (status == STR("archived"sv))
				res.Status = PollStatus::Archived;
			else if (status == STR("terminated"sv))
				res.Status = PollStatus::Terminated;
			else
				res.Status = PollStatus::Invalid;
		}
		else
		{
			// not ended, so it must be in progress
			res.Status = PollStatus::Active;
		}

		// Missing: res.Duration = p.at("duration").to_int();
		res.StartedAt = p.at("started_at").to_string();
		res.EndedAt = p.at("ended_at").to_string(); // optional, may be missing and throw (but fine because it's the last one)
	}
	catch (...) {
	}

	return res;
}

void R66ApiImpl::ESEventPollUpdate(Self self, const JsonRef& json, string_view key) {
	auto info = ParseESPollInfo(json, key == PollEnd.Type);
	Log(self, LogLevel::Debug, "[ES] got poll update for %s"sv, to_utf8(info.Id));
	self->PendingPollInfoUpdates[info.Id.ToOwned()].GetTask().ResolveSPMC(std::move(info));
	PollPredictionStatus<PollInfo>::Cleanup(self->PendingPollInfoUpdates);
}


void R66ApiImpl::CreatePoll(const PollDefinition& def, ResolveFn<const PollInfo&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	MyUserInfo.Await(
		[self = shared_from_this(), def = std::make_shared<PollDefinition>(def), resolve, reject](const UserInfo& ui)
		{
			std::lock_guard<std::recursive_mutex> lock(self->Mutex);

			Json body;
			body.set_property("broadcaster_id", Json(ui.ChannelId));
			body.set_property("title", Json(def->Title));
			body.set_property("duration", Json(def->Duration));
			body.set_property("bits_voting_enabled", Json(def->BitsVotingEnabled));
			body.set_property("bits_per_vote", Json(def->BitsPerVote));
			body.set_property("channel_points_voting_enabled", Json(def->ChannelPointsVotingEnabled));
			body.set_property("channel_points_per_vote", Json(def->ChannelPointsPerVote));

			std::vector<Json> choices(def->Choices.size());
			for (size_t i = 0; i < choices.size(); i++) {
				choices[i].set_property("title", Json(def->Choices[i]));
			}
			body.set_array("choices", std::move(choices));

			R66ApiImpl::ESAddSubscription(self, PollProgress);
			R66ApiImpl::ESAddSubscription(self, PollEnd);

			R66ApiImpl::CallApi(
				self,
				std::make_shared<LoggedInApiCall>(STR("https://api.twitch.tv/helix/polls"s), body.serialize()),
				[self, def, resolve, reject](const WebRequestResult& r)
				{
					std::lock_guard<std::recursive_mutex> lock(self->Mutex);

					auto res = ParsePollInfo(r.ResponseBody);
					resolve(res);
				},
				reject
			);
		}
	);
}

void R66ApiImpl::EndPoll(const EndPollRequest& req, ResolveFn<const PollInfo&> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	Json body;
	body.set_property("broadcaster_id", Json(req.BroadcasterId));
	body.set_property("id", Json(req.PollId));
	body.set_property("status", Json(req.ShowResults ? STR("TERMINATED"sv) : STR("ARCHIVED"sv)));

	R66ApiImpl::CallApi(
		shared_from_this(),
		std::make_shared<LoggedInApiCall>(STR("https://api.twitch.tv/helix/polls"s), body.serialize(), HttpMethod::Patch),
		[resolve, reject](const WebRequestResult& r)
		{
			auto res = ParsePollInfo(r.ResponseBody);
			resolve(res);
		},
		reject
	);
}

void R66ApiImpl::WaitForPollUpdate(string_view pollId, ResolveFn<const PollInfo&> resolve, RejectFn reject)
{
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	PendingPollInfoUpdates[string(pollId)].GetTask().AwaitSPMC(std::move(resolve), std::move(reject));
	PollPredictionStatus<PollInfo>::Cleanup(PendingPollInfoUpdates);
}

void R66ApiImpl::UnsubscribeFromPoll(string_view id, ResolveFn<void> resolve, RejectFn reject) {
	std::lock_guard<std::recursive_mutex> lock(Mutex);
	R66ApiImpl::ESDelSubscription(shared_from_this(), PollProgress);
	R66ApiImpl::ESDelSubscription(shared_from_this(), PollEnd);
	resolve();
}
