#include <Data.Game.Character.CurrentIsland.h>
#include "Game.Characters.Islands.h"
#include "Game.Session.h"
namespace game::characters
{
	std::list<int> Islands::All(int islandId, const game::characters::State& state)
	{
		auto characterIds = data::game::character::CurrentIsland::All(islandId);
		std::list<int> result;
		for (auto characterId : characterIds)
		{
			if (state == game::Session().GetCharacters().GetCharacter(characterId).GetState())
			{
				result.push_back(characterId);
			}
		}
		return result;
	}
}