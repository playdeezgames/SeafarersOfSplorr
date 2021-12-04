#include <Data.Game.Character.Island.h>
#include "Game.Characters.Islands.h"
#include "Game.Characters.h"
namespace game::characters
{
	std::optional<int> Islands::Read(int characterId)
	{
		return data::game::character::Island::Read(characterId);
	}

	void Islands::Write(int characterId, int islandId)
	{
		data::game::character::Island::Write(characterId, islandId);
	}

	void Islands::Clear(int characterId)
	{
		data::game::character::Island::Clear(characterId);
	}

	std::list<int> Islands::All(int islandId, const game::characters::State& state)
	{
		auto characterIds = data::game::character::Island::All(islandId);
		std::list<int> result;
		for (auto characterId : characterIds)
		{
			if (state == Characters::GetState(characterId))
			{
				result.push_back(characterId);
			}
		}
		return result;
	}
}