#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <functional>
#include "Game.Islands.DarkAlley.h"
namespace game::islands
{
	static std::optional<double> ExtractDarkAlleyValue(int islandId, std::function<double(const data::game::island::DarkAlley&)> extractor)
	{
		auto result = data::game::island::DarkAlley::Read(islandId);
		if (result)
		{
			return extractor(result.value());
		}
		return std::nullopt;
	}

	std::optional<double> DarkAlley::GetRuffianBrawling(int islandId)
	{
		return ExtractDarkAlleyValue(
			islandId,
			[](const data::game::island::DarkAlley& data)
			{
				return data.ruffianBrawlingStrength;
			});
	}

	std::optional<double> DarkAlley::GetMinimumWager(int islandId)
	{
		return ExtractDarkAlleyValue(
			islandId,
			[](const data::game::island::DarkAlley& data)
			{
				return data.minimumWager;
			});
	}

	static const double ANTE_MULTIPLIER = 0.1;

	std::optional<double> DarkAlley::GetAnte(int islandId)
	{
		auto minimum = GetMinimumWager(islandId);
		if (minimum)
		{
			return minimum.value() * ANTE_MULTIPLIER;
		}
		return std::nullopt;
	}

}