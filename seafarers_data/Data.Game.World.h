#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game//20211014
{
	struct World
	{
		int version;
		common::XY<double> size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
		double windHeading;
		double unfoulingLaborMultiplier;

		static void Write(const World&);
		static std::optional<World> Read();
	};
}