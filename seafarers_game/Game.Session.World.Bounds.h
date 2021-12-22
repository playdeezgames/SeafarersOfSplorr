#pragma once
#include <Common.XY.h>
namespace game::session::world
{
	struct Bounds
	{
		constexpr Bounds(int worldId) : worldId(worldId) {}
		common::XY<double> GetSize() const;
		bool ClampLocation(common::XY<double>&) const;
	private:
		int worldId;
	};
}
