#pragma once
#include <optional>
namespace data::game::avatar
{
	struct Statistics
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, const Statistics&);
		static std::optional<Statistics> Read(int);
	};
}
