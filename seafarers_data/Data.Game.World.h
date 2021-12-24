#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct World
	{
		static void Initialize();

		static void Write(int, int, double, int, int);
		static void WriteWindHeading(int, double);
		static void WriteDay(int, int);

		static std::optional<double> ReadWindHeading(int);
		static std::optional<int> ReadCurrencyItemTypeId(int);
		static std::optional<int> ReadDay(int);
		static std::optional<int> ReadDifficulty(int);
	};
}