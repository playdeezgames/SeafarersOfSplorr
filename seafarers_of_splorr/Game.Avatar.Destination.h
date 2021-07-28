#pragma once
#include <optional>
#include "Common.XY.h"	
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination();
	void SetDestination(const std::optional<common::XY<double>>&);
}