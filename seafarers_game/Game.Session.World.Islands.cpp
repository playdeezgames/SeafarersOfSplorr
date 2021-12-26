#include <Data.Game.Island.h>
#include "Game.Session.World.Islands.h"
namespace game::session::world
{
	using IslandData = data::game::Island;

	std::list<Island> Islands::GetAll() const
	{
		auto islands = IslandData::All();
		std::list<Island> result;
		for (auto island : islands)
		{
			result.push_back(Island(island.id));
		}
		return result;
	}
}