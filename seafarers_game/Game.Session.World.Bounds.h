#pragma once
#include <Common.XY.h>
namespace game::session::world
{
	struct Bounds
	{
		Bounds() = delete;
		static common::XY<double> GetSize();
		static void ClampLocation(common::XY<double>& location);
	};
}
