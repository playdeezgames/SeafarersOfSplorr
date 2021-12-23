#include <Data.Game.Character.KnownIsland.h>
#include "Game.Session.Character.KnownIslands.h"
namespace game::session::character
{
	bool KnownIslands::HasAny() const
	{
		return !data::game::character::KnownIsland::All(characterId).empty();
	}

	std::list<KnownIsland> KnownIslands::GetAll() const
	{
		auto islandIds = data::game::character::KnownIsland::All(characterId);
		std::list<KnownIsland> result;
		for (auto islandId : islandIds)
		{
			result.push_back(KnownIsland(characterId, islandId));
		}
		return result;
	}

}