#pragma once
#include <optional>
namespace data::game::character
{
	struct StatisticLegacy
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, int, const StatisticLegacy&);
		static std::optional<StatisticLegacy> Read(int, int);
	};
}
