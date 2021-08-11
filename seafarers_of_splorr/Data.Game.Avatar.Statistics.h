#pragma once
#include <optional>
namespace data::game::avatar::Statistics
{
	struct StatisticData
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;
	};

	void Write(int, const StatisticData&);
	std::optional<StatisticData> Read(int);
}
