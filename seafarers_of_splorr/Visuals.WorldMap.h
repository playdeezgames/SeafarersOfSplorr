#pragma once
#include <optional>
#include "Common.XY.h"
#include <string>
namespace visuals::WorldMap
{
	void SetDestination(const std::string&, const std::string&, const std::optional<common::XY<int>>&);
	std::optional<common::XY<double>> GetDestination(const std::string&, const std::string&);
	std::optional<common::XY<double>> GetHoverIsland(const std::string&, const std::string&);
}