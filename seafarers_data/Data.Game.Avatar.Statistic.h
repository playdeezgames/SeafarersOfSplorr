#pragma once
#include <optional>
namespace data::game::avatar//20211014
{
	struct Statistic
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, int, const Statistic&);
		static std::optional<Statistic> Read(int, int);
	};
}
