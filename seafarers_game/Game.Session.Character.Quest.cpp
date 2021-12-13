#include <format>
#include "Game.Characters.Quests.h"
#include "Game.Session.Character.Quest.h"
namespace game::session::character
{
	Quest::Quest(int characterId)
		: characterId(characterId)
	{

	}

	static std::optional<Island> TryGetDestinationIsland(int characterId)
	{
		auto quest = game::characters::Quests::Read(characterId);
		if (quest)
		{
			return Island(quest.value().toIslandId);
		}
		return std::nullopt;
	}

	void Quest::Abandon() const
	{
		game::characters::Quests::Abandon(characterId);
	}

	static std::optional<std::string> TryGetCompletionMessage(int characterId)
	{
		auto quest = game::characters::Quests::Read(characterId);
		if (quest)
		{
			return std::format("{} the {} is {} when given the {}.",
				quest.value().personName,
				quest.value().professionName,
				quest.value().receiptEmotion,
				quest.value().itemName);
		}
		return std::nullopt;
	}

	std::string Quest::GetCompletionMessage() const
	{
		return TryGetCompletionMessage(characterId).value();
	}

	Island Quest::GetDestinationIsland() const
	{
		return TryGetDestinationIsland(characterId).value();
	}
}