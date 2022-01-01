#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game
{
	struct World
	{
		static void Initialize();

		static void Write(
			int worldId,
			int difficulty,
			double windHeading,
			int currencyItemTypeId,
			int day);
		static void WriteWindHeading(int worldId, double windHeading);
		static void WriteDay(int worldId, int day);

		static std::optional<double> ReadWindHeading(int worldId);
		static std::optional<int> ReadCurrencyItemTypeId(int worldId);
		static std::optional<int> ReadDay(int worldId);
		static std::optional<int> ReadDifficulty(int worldId);
	};
}