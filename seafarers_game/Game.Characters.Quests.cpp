#include <Common.Utility.Optional.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Character.QuestLegacy.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.QuestLegacy.h>
#include "Game.Characters.Quests.h"
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
		game::Session().GetCharacters().GetCharacter(characterId).GetKnownIslands().AddKnownIsland(quest.toIslandId);
		data::game::character::KnownIsland::Write(characterId, quest.toIslandId);
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
		auto markets = game::Session().GetWorld().GetIslands().GetIsland(quest.toIslandId).GetMarkets();
		auto currencyItem = game::Session().GetWorld().GetCurrencyItemSubtype();
		int quantity = markets.GetPurchaseQuantity(currencyItem, quest.reward);
		character.GetItems().AddItemQuantity(currencyItem, quantity);
		character.GetCounters().GetCounter(game::characters::Counter::REPUTATION).Change(1);
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
		game::Session().GetCharacters().GetCharacter(characterId).GetCounters().GetCounter(game::characters::Counter::REPUTATION).Change(-1);
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
				data::game::Island::ReadLocation(quest.toIslandId).value(),
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