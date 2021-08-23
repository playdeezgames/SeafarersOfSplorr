#include <Data.Game.Island.DarkAlley.h>
#include <functional>
#include "Game.Islands.DarkAlley.h"
namespace game::islands::DarkAlley
{
	static std::optional<double> ExtractDarkAlleyValue(const common::XY<double>& location, std::function<double(const data::game::island::DarkAlley&)> extractor)
	{
		auto result = data::game::island::DarkAlley::Read(location);
		if (result)
		{
			return extractor(result.value());
		}
		return std::nullopt;
	}

	std::optional<double> GetRuffianBrawling(const common::XY<double>& location)
	{
		return ExtractDarkAlleyValue(
			location,
			[](const data::game::island::DarkAlley& data)
			{
				return data.ruffianBrawlingStrength;
			});
	}

	std::optional<double> GetMinimumWager(const common::XY<double>& location)
	{
		return ExtractDarkAlleyValue(
			location,
			[](const data::game::island::DarkAlley& data)
			{
				return data.minimumWager;
			});
	}

	const double ANTE_MULTIPLIER = 0.1;

	std::optional<double> GetAnte(const common::XY<double>& location)
	{
		auto minimum = GetMinimumWager(location);
		if (minimum)
		{
			return minimum.value() * ANTE_MULTIPLIER;
		}
		return std::nullopt;
	}

}