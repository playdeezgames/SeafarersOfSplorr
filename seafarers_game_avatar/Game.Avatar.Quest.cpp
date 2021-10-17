#include <Data.Game.Avatar.Quest.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Quest.h>
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include <Game.Player.h>
namespace game::avatar
{
	const double REPUTATION_REWARD = 1.0;
	const double REPUTATION_PENALTY = -1.0;

	AcceptQuestResult Quest::Accept(const common::XY<double>& location)
	{
		if (data::game::avatar::Quest::Read(Player::GetAvatarId()))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::Quest::Read(location);
		if (quest)
		{
			data::game::avatar::Quest::Write(
				Player::GetAvatarId(),
				std::optional<data::game::avatar::Quest>({
					quest.value().destination,
					quest.value().reward,
					quest.value().itemName,
					quest.value().personName,
					quest.value().professionName ,
					quest.value().receiptEmotion}));
			data::game::island::Quest::Clear(location);
			game::Islands::SetKnown(quest.value().destination, game::avatar::Statistics::GetTurnsRemaining());
			data::game::island::Known::Write(quest.value().destination);
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	bool Quest::Complete(const common::XY<double>& location)
	{
		auto quest = data::game::avatar::Quest::Read(Player::GetAvatarId());
		if (quest.has_value() && quest.value().destination == location)
		{
			game::avatar::Statistics::ChangeMoney(quest.value().reward);
			game::avatar::Statistics::ChangeReputation(REPUTATION_REWARD);
			data::game::avatar::Quest::Write(Player::GetAvatarId(), std::nullopt);
			return true;
		}
		return false;
	}

	bool Quest::Abandon()
	{
		if (data::game::avatar::Quest::Read(Player::GetAvatarId()))
		{
			game::avatar::Statistics::ChangeReputation(REPUTATION_PENALTY);
			data::game::avatar::Quest::Write(Player::GetAvatarId(), std::nullopt);
			return true;
		}
		return false;
	}

	std::optional<game::Quest> Quest::Read()
	{
		auto quest = data::game::avatar::Quest::Read(Player::GetAvatarId());
		if (quest)
		{
			return std::optional<game::Quest>({
				quest.value().destination,
				quest.value().reward,
				quest.value().itemName,
				quest.value().personName,
				quest.value().professionName,
				quest.value().receiptEmotion});
		}
		return std::nullopt;
	}

}