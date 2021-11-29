#pragma once
#include <optional>
namespace game::islands
{
	struct DarkAlley
	{
		static std::optional<double> GetRuffianBrawling(int);
		static std::optional<double> GetMinimumWager(int);
		static std::optional<double> GetAnte(int);
	};
}
