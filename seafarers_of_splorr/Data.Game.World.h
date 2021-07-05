#pragma once
#include "Common.XY.h"
#include <optional>
namespace data::game::World
{
	struct WorldData
	{
		int version;
		common::XY<double> size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;

	};

	void Write(const WorldData&);
	std::optional<WorldData> Read();
}