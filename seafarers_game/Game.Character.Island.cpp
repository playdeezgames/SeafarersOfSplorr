#include <Data.Game.Character.Island.h>
#include "Game.Character.Island.h"
#include "Game.Character.h"
namespace game::character
{
	std::optional<int> Island::Read(int characterId)
	{
		return data::game::character::Island::Read(characterId);
	}

	void Island::Write(int characterId, int islandId)
	{
		data::game::character::Island::Write(characterId, islandId);
	}

	void Island::Clear(int characterId)
	{
		data::game::character::Island::Clear(characterId);
	}

	std::list<int> Island::All(int islandId, const game::character::State& state)
	{
		auto characterIds = data::game::character::Island::All(islandId);
		std::list<int> result;
		for (auto characterId : characterIds)
		{
			if (state == Character::GetState(characterId))
			{
				result.push_back(characterId);
			}
		}
		return result;
	}
}