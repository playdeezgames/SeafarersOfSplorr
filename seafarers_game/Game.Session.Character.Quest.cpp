#include <format>
#include "Game.Characters.Quests.h"
#include "Game.Session.Character.Quest.h"
namespace game::session::character
{
	Quest::Quest(int characterId)
		: characterId(characterId)
	{

	}

	std::optional<Island> Quest::GetDestinationIsland() const
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

	std::optional<std::string> Quest::GetCompletionMessage() const
	{
		auto quest = game::characters::Quests::Read(characterId);
		if (quest)
		{
			std::format("{} the {} is {} when given the {}.",
				quest.value().personName,
				quest.value().professionName,
				quest.value().receiptEmotion,
				quest.value().itemName);
		}
		return std::nullopt;
	}
}