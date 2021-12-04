#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Quest.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Quest.h>
#include "Game.Characters.Quests.h"
#include "Game.Characters.Statistics.h"
#include "Game.Islands.h"
#include "Game.World.h"
namespace game::characters
{
	static void AcceptQuest(int characterId, const data::game::island::Quest& quest)
	{
		auto fromIslandId = quest.fromIslandId;
		data::game::character::Quest::Write(
			characterId,
			std::optional<data::game::character::Quest>({
				quest.toIslandId,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::Quest::Clear(fromIslandId);
		game::Islands::SetKnown(quest.toIslandId, game::characters::statistics::Turns::Remaining(characterId).value());
		data::game::island::Known::Write(quest.toIslandId);
	}

	AcceptQuestResult Quests::Accept(int characterId, int fromIslandId)
	{
		if (data::game::character::Quest::Read(characterId))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::Quest::Read(fromIslandId);
		if (quest)
		{
			AcceptQuest(characterId, quest.value());
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	static void CompleteQuest(int characterId, const data::game::character::Quest& quest)
	{
		game::characters::statistics::Money::Change(characterId, quest.reward);
		game::characters::statistics::Reputation::Change(characterId, World::GetReputationReward());
		data::game::character::Quest::Write(characterId, std::nullopt);
	}

	bool Quests::Complete(int characterId, int islandId)
	{
		auto quest = data::game::character::Quest::Read(characterId);
		if (quest.has_value() && quest.value().toIslandId == islandId)
		{
			CompleteQuest(characterId, quest.value());
			return true;
		}
		return false;
	}

	static void AbandonQuest(int characterId)
	{
		game::characters::statistics::Reputation::Change(characterId, World::GetReputationPenalty());
		data::game::character::Quest::Write(characterId, std::nullopt);
	}

	bool Quests::Abandon(int characterId)
	{
		if (data::game::character::Quest::Read(characterId))
		{
			AbandonQuest(characterId);
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

	std::optional<game::Quest> Quests::Read(int characterId)
	{
		return
			common::utility::Optional::Map<data::game::character::Quest, game::Quest>(
				data::game::character::Quest::Read(characterId),
				ToQuest);
	}
}