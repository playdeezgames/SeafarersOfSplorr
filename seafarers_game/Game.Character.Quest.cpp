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
	static void AcceptQuest(const data::game::island::Quest& quest)
	{
		auto fromIslandId = quest.fromIslandId;
		data::game::character::Quest::Write(
			Player::GetCharacterId(),
			std::optional<data::game::character::Quest>({
				quest.toIslandId,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::Quest::Clear(fromIslandId);
		game::Islands::SetKnown(quest.toIslandId, game::character::Statistics::GetTurnsRemaining(game::Player::GetCharacterId()));
		data::game::island::Known::Write(quest.toIslandId);
	}

	AcceptQuestResult Quest::Accept(int fromIslandId)
	{
		if (data::game::character::Quest::Read(Player::GetCharacterId()))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::Quest::Read(fromIslandId);
		if (quest)
		{
			AcceptQuest(quest.value());
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	static void CompleteQuest(const data::game::character::Quest& quest)
	{
		game::character::Statistics::ChangeMoney(game::Player::GetCharacterId(), quest.reward);
		game::character::Statistics::ChangeReputation(game::Player::GetCharacterId(), World::GetReputationReward());
		data::game::character::Quest::Write(Player::GetCharacterId(), std::nullopt);
	}

	bool Quest::Complete(const common::XY<double>& location)
	{
		auto quest = data::game::character::Quest::Read(Player::GetCharacterId());
		auto destination = data::game::Island::Read(quest.value().toIslandId).value().location;
		if (quest.has_value() && destination == location)
		{
			CompleteQuest(quest.value());
			return true;
		}
		return false;
	}

	static void AbandonQuest()
	{
		game::character::Statistics::ChangeReputation(game::Player::GetCharacterId(), World::GetReputationPenalty());
		data::game::character::Quest::Write(Player::GetCharacterId(), std::nullopt);
	}

	bool Quest::Abandon()
	{
		if (data::game::character::Quest::Read(Player::GetCharacterId()))
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
				quest.toIslandId,
				data::game::Island::Read(quest.toIslandId).value().location,
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
				data::game::character::Quest::Read(Player::GetCharacterId()),
				ToQuest);
	}
}