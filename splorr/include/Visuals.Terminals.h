#pragma once
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Terminals
	{
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}