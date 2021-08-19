#pragma once
#include <optional>
#include "Common.XY.h"
#include <list>
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
	const std::list<game::avatar::Destination>& All();
	std::optional<common::XY<double>> GetDestination(const game::avatar::Destination&);
	void SetDestination(const game::avatar::Destination&,const std::optional<common::XY<double>>&);
}