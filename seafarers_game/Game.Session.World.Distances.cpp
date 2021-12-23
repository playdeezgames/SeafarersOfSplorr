#include <Data.Game.World.h>
#include "Game.Session.World.Distances.h"
namespace game::session::world
{
	double Distances::GetView() const
	{
		return data::game::World::ReadViewDistance(worldId).value();
	}

	double Distances::GetDock() const
	{
		return data::game::World::ReadDockDistance(worldId).value();
	}

	double Distances::GetIslandSpacing() const
	{
		return data::game::World::ReadMinimumIslandDistance(worldId).value();
	}
}