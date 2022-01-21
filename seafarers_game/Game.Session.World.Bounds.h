#pragma once
#include <Common.XY.h>
#include "Game.Session.World.h"
namespace game::session::world
{
	struct Bounds
	{
		constexpr explicit Bounds() : worldId(WORLD_ID) {}
		common::XY<double> GetSize() const;
		bool ClampLocation(common::XY<double>& location) const;
		void Reset() const {}
	private:
		int worldId;
	};
}
