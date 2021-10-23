#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Avatar.Quest.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Quest.h>
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include <Game.Player.h>
#include <Game.World.h>
namespace game::avatar//20211018
{
	static void AcceptQuest(const data::game::island::Quest& quest, const common::XY<double>& location)
	{
		data::game::avatar::Quest::Write(
			Player::GetAvatarId(),
			std::optional<data::game::avatar::Quest>({
				quest.destination,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::Quest::Clear(location);
		game::Islands::SetKnown(quest.destination, game::avatar::Statistics::GetTurnsRemaining());
		data::game::island::Known::Write(quest.destination);
	}

	AcceptQuestResult Quest::Accept(const common::XY<double>& location)
	{
		if (data::game::avatar::Quest::Read(Player::GetAvatarId()))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::Quest::Read(location);
		if (quest)
		{
			AcceptQuest(quest.value(), location);
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	static void CompleteQuest(const data::game::avatar::Quest& quest)
	{
		game::avatar::Statistics::ChangeMoney(quest.reward);
		game::avatar::Statistics::ChangeReputation(World::GetReputationReward());
		data::game::avatar::Quest::Write(Player::GetAvatarId(), std::nullopt);
	}

	bool Quest::Complete(const common::XY<double>& location)
	{
		auto quest = data::game::avatar::Quest::Read(Player::GetAvatarId());
		if (quest.has_value() && quest.value().destination == location)
		{
			CompleteQuest(quest.value());
			return true;
		}
		return false;
	}

	static void AbandonQuest()
	{
		game::avatar::Statistics::ChangeReputation(World::GetReputationPenalty());
		data::game::avatar::Quest::Write(Player::GetAvatarId(), std::nullopt);
	}

	bool Quest::Abandon()
	{
		if (data::game::avatar::Quest::Read(Player::GetAvatarId()))
		{
			AbandonQuest();
			return true;
		}
		return false;
	}

	static game::Quest ToQuest(const data::game::avatar::Quest& quest)
	{
		return 
			{
				quest.destination,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName,
				quest.receiptEmotion
			};
	}

	std::optional<game::Quest> Quest::Read()
	{
		return
			common::utility::Optional::MapOptional<data::game::avatar::Quest, game::Quest>(
				data::game::avatar::Quest::Read(Player::GetAvatarId()),
				ToQuest);
	}
}