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

		static std::optional<World> Read(int);

		static void Initialize();
		static void Create(int, int, const common::XY<double>&, double, double, double, double, int, int);
		static std::optional<common::XY<double>> ReadSize(int);
		static std::optional<double> ReadMinimumIslandDistance(int);
		static std::optional<double> ReadViewDistance(int);
		static std::optional<double> ReadDockDistance(int);
		static std::optional<double> ReadWindHeading(int);
		static std::optional<int> ReadCurrencyItemTypeId(int);
		static std::optional<int> ReadDay(int);
		static void WriteWindHeading(int, double);
		static void WriteDay(int, int);
	};
}