#pragma once
#include <Common.XY.h>
namespace game::session::world
{
	struct Bounds
	{
		constexpr Bounds() {}
		common::XY<double> GetSize() const;
		bool ClampLocation(common::XY<double>&) const;
	};
}
