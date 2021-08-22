#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals::WorldMap
{
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::WorldMap
{
	void SetDestination(const std::string&, const std::string&, const std::optional<common::XY<int>>&);
	std::optional<common::XY<double>> GetDestination(const std::string&, const std::string&);
	std::optional<common::XY<double>> GetHoverIsland(const std::string&, const std::string&);
}