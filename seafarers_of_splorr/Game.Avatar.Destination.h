#pragma once
#include <optional>
#include "Common.XY.h"
namespace game::avatar
{
	enum class Destination
	{
		ONE,
		TWO,
		THREE,
		FOUR
	};
}
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination(const game::avatar::Destination&);
	void SetDestination(const game::avatar::Destination&,const std::optional<common::XY<double>>&);
}