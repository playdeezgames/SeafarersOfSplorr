#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Quest.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Quest.h>
#include "Game.Character.Quest.h"
#include "Game.Character.Statistics.h"
#include "Game.Islands.h"
#include "Game.Player.h"
#include "Game.World.h"
namespace game::character
{
	static void AcceptQuest(const data::game::island::Quest& quest, const common::XY<double>& location)
	{
		auto fromIslandId = data::game::Island::Find(location).value();
		auto toIsland = data::game::Island::Read(quest.toIslandId).value();
		data::game::character::Quest::Write(
			Player::GetAvatarId(),
			std::optional<data::game::character::Quest>({
				toIsland.location,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::Quest::Clear(fromIslandId);
		game::Islands::SetKnown(toIsland.location, game::character::Statistics::GetTurnsRemaining(game::Player::GetAvatarId()));
		data::game::island::Known::Write(toIsland.location);
	}

	AcceptQuestResult Quest::Accept(const common::XY<double>& location)
	{
		if (data::game::character::Quest::Read(Player::GetAvatarId()))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto fromIslandId = data::game::Island::Find(location).value();
		auto quest = data::game::island::Quest::Read(fromIslandId);
		if (quest)
		{
			AcceptQuest(quest.value(), location);
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	static void CompleteQuest(const data::game::character::Quest& quest)
	{
		game::character::Statistics::ChangeMoney(game::Player::GetAvatarId(), quest.reward);
		game::character::Statistics::ChangeReputation(game::Player::GetAvatarId(), World::GetReputationReward());
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
		game::character::Statistics::ChangeReputation(game::Player::GetAvatarId(), World::GetReputationPenalty());
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