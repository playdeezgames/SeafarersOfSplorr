#pragma once
#include "json.hpp"
#include "Visuals.DrawerFunction.h"
namespace visuals::Sublayout
{
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
