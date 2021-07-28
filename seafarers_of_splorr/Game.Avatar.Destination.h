#pragma once
#include <optional>
#include "Common.XY.h"	
namespace game::avatar::destination
{
	std::optional<common::XY<double>> GetDestination(int);
	void SetDestination(int,const std::optional<common::XY<double>>&);
}