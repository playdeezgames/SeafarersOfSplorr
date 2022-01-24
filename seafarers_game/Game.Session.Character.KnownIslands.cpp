#include <algorithm>
#include <Data.Game.Character.Island.Known.h>
#include "Game.Session.Character.KnownIslands.h"
#include <iterator>
namespace game::session::character
{
	bool KnownIslands::HasAny() const
	{
		return !data::game::character::island::Known::All(characterId).empty();
	}

	void KnownIslands::AddKnownIsland(int islandId) const
	{
		data::game::character::island::Known::Write(characterId, islandId);
	}

	std::vector<KnownIsland> KnownIslands::GetAll() const
	{
		auto islandIds = data::game::character::island::Known::All(characterId);
		std::vector<KnownIsland> result;
		std::transform(
			islandIds.begin(),
			islandIds.end(),
			std::back_inserter(result),
			[this](int islandId) 
			{
				return KnownIsland(characterId, islandId);
			});
		return result;
	}

}