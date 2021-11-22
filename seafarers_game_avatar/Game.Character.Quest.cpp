#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Quest.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Quest.h>
#include "Game.Character.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include <Game.Player.h>
#include <Game.World.h>
namespace game::character
{
	static void AcceptQuest(const data::game::island::Quest& quest, const common::XY<double>& location)
	{
		data::game::character::Quest::Write(
			Player::GetAvatarId(),
			std::optional<data::game::character::Quest>({
				quest.destination,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::Quest::Clear(location);
		game::Islands::SetKnown(quest.destination, game::avatar::Statistics::GetTurnsRemaining(game::Player::GetAvatarId()));
		data::game::island::Known::Write(quest.destination);
	}

	AcceptQuestResult Quest::Accept(const common::XY<double>& location)
	{
		if (data::game::character::Quest::Read(Player::GetAvatarId()))
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

	static void CompleteQuest(const data::game::character::Quest& quest)
	{
		game::avatar::Statistics::ChangeMoney(game::Player::GetAvatarId(), quest.reward);
		game::avatar::Statistics::ChangeReputation(game::Player::GetAvatarId(), World::GetReputationReward());
		data::game::character::Quest::Write(Player::GetAvatarId(), std::nullopt);
	}

	bool Quest::Complete(const common::XY<double>& location)
	{
		auto quest = data::game::character::Quest::Read(Player::GetAvatarId());
		if (quest.has_value() && quest.value().destination == location)
		{
			CompleteQuest(quest.value());
			return true;
		}
		return false;
	}

	static void AbandonQuest()
	{
		game::avatar::Statistics::ChangeReputation(game::Player::GetAvatarId(), World::GetReputationPenalty());
		data::game::character::Quest::Write(Player::GetAvatarId(), std::nullopt);
	}

	bool Quest::Abandon()
	{
		if (data::game::character::Quest::Read(Player::GetAvatarId()))
		{
			AbandonQuest();
			return true;
		}
		return false;
	}

	static game::Quest ToQuest(const data::game::character::Quest& quest)
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
			common::utility::Optional::Map<data::game::character::Quest, game::Quest>(
				data::game::character::Quest::Read(Player::GetAvatarId()),
				ToQuest);
	}
}