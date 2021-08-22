#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::World
{
	struct Data
	{
		int version;
		common::XY<double> size;
		double minimumIslandDistance;
		double viewDistance;
		double dockDistance;
		double windHeading;
	};

	void Write(const Data&);
	std::optional<Data> Read();
}