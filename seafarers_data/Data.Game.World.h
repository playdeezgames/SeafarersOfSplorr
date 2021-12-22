#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct World
	{
		int version;
		common::XY<double> size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
		double windHeading;
		int currencyItemTypeId;
		int day;

		static void Initialize();
		static int Write(const World&);
		static std::optional<World> Read();//TODO: this needs to take a world id
	};
}