#pragma once
#include "Game.Session.World.h"
namespace game::session::world
{
	struct Distances
	{
		constexpr explicit Distances() : worldId(WORLD_ID) {}
		constexpr double GetView() const { return 10.0; }
		constexpr double GetDock() const { return 1.0; }
		double GetIslandSpacing() const;
		void Reset() const {}
	private:
		int worldId;
	};
}
