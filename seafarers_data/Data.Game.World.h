#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	class World
	{
	public:
		struct Data
		{
			int version;
			common::XY<double> size;
			double minimumIslandDistance;
			double viewDistance;
			double dockDistance;
			double windHeading;
		};
		static void Write(const Data&);
		static std::optional<Data> Read();
	};
}