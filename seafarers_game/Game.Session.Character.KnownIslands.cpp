#include <Data.Game.Character.KnownIsland.h>
#include "Game.Session.Character.KnownIslands.h"
namespace game::session::character
{
	bool KnownIslands::HasAny() const
	{
		return !data::game::character::KnownIsland::All(characterId).empty();
	}

	void KnownIslands::AddKnownIsland(const Island& island) const
	{
		data::game::character::KnownIsland::Write(characterId, island.operator int());
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