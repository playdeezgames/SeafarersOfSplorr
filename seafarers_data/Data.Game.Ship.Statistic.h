#pragma once
#include <optional>
namespace data::game::ship
{
	struct Statistic
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double current;

		static void Write(int, int, const Statistic&);
		static std::optional<Statistic> Read(int, int);
		static void Clear(int);
	};
}
