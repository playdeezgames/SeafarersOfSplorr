#include <Common.Utility.Optional.h>
#include <Data.Game.Character.QuestLegacy.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.QuestLegacy.h>
#include "Game.Characters.Quests.h"
#include "Game.Characters.Statistics.h"
#include "Game.Islands.h"
#include "Game.Session.h"
namespace game::characters
{
	static void AcceptQuest(int characterId, const data::game::island::QuestLegacy& quest)
	{
		auto fromIslandId = quest.fromIslandId;
		data::game::character::QuestLegacy::Write(
			characterId,
			std::optional<data::game::character::QuestLegacy>({
				quest.toIslandId,
				quest.reward,
				quest.itemName,
				quest.personName,
				quest.professionName ,
				quest.receiptEmotion }));
		data::game::island::QuestLegacy::Clear(fromIslandId);
		game::Islands::SetKnown(quest.toIslandId, game::characters::statistics::Turns::Remaining(characterId).value());
		data::game::island::Known::Write(quest.toIslandId);
	}

	AcceptQuestResult Quests::Accept(int characterId, int fromIslandId)
	{
		if (data::game::character::QuestLegacy::Read(characterId))
		{
			return AcceptQuestResult::ALREADY_HAS_QUEST;
		}
		auto quest = data::game::island::QuestLegacy::Read(fromIslandId);
		if (quest)
		{
			AcceptQuest(characterId, quest.value());
			return AcceptQuestResult::ACCEPTED_QUEST;
		}
		return AcceptQuestResult::NO_QUEST_TO_ACCEPT;
	}

	static void CompleteQuest(int characterId, const data::game::character::QuestLegacy& quest)
	{
		auto character = game::Session().GetCharacters().GetCharacter(characterId);
		auto markets = game::Session().GetIslands().GetIsland(quest.toIslandId).GetMarkets();
		auto currencyItem = game::Session().GetWorld().GetCurrencyItemSubtype();
		int quantity = markets.GetPurchaseQuantity(currencyItem, quest.reward);
		character.GetItems().AddItemQuantity(currencyItem, quantity);
		game::characters::statistics::Reputation::Change(characterId, 1.0);
		data::game::character::QuestLegacy::Write(characterId, std::nullopt);
	}

	bool Quests::Complete(int characterId, int islandId)
	{
		auto quest = data::game::character::QuestLegacy::Read(characterId);
		if (quest.has_value() && quest.value().toIslandId == islandId)
		{
			CompleteQuest(characterId, quest.value());
			return true;
		}
		return false;
	}

	static void AbandonQuest(int characterId)
	{
		game::characters::statistics::Reputation::Change(characterId, -1.0);
		data::game::character::QuestLegacy::Write(characterId, std::nullopt);
	}

	bool Quests::Abandon(int characterId)
	{
		if (data::game::character::QuestLegacy::Read(characterId))
		{
			AbandonQuest(characterId);
			return true;
		}
		return false;
	}

	static game::Quest ToQuest(const data::game::character::QuestLegacy& quest)
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
			common::utility::Optional::Map<data::game::character::QuestLegacy, game::Quest>(
				data::game::character::QuestLegacy::Read(characterId),
				ToQuest);
	}
}