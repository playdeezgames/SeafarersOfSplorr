#pragma once
#include <Common.XY.h>
namespace game::session::world
{
	struct Bounds
	{
		constexpr explicit Bounds(int worldId) : worldId(worldId) {}
		common::XY<double> GetSize() const;
		bool ClampLocation(common::XY<double>& location) const;
		void Reset() const {}
	private:
		int worldId;
	};
}
