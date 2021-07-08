#pragma once
#include <optional>
namespace data::game::avatar::Statistics
{
	struct StatisticData
	{
		double minimum;
		double maximum;
		double current;
	};

	void Write(int, const StatisticData&);
	std::optional<StatisticData> Read(int);
}
