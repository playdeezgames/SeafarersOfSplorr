#include <Data.Game.World.h>
#include "Game.Session.World.Distances.h"
namespace game::session::world
{
	double Distances::GetView() const
	{
		return data::game::World::Read().value().viewDistance;
	}

	double Distances::GetDock() const
	{
		return data::game::World::Read().value().dockDistance;
	}

	double Distances::GetIslandSpacing() const
	{
		return data::game::World::Read().value().minimumIslandDistance;
	}
}