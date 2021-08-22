#pragma once
#include <Common.XY.h>
#include <optional>
namespace game::islands::DarkAlley
{
	std::optional<double> GetRuffianBrawling(const common::XY<double>&);
	std::optional<double> GetMinimumWager(const common::XY<double>&);
	std::optional<double> GetAnte(const common::XY<double>&);
}
