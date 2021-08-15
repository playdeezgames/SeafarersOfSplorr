#include "Data.Game.Island.DarkAlley.h"
#include "Game.Islands.DarkAlley.h"
namespace game::islands::DarkAlley
{
	std::optional<double> GetRuffianBrawling(const common::XY<double>& location)
	{
		auto result = data::game::island::DarkAlley::Read(location);
		if (result)
		{
			return result.value().ruffianBrawlingStrength;
		}
		return std::nullopt;
	}
}