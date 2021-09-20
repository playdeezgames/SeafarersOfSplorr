#pragma once
#include <Common.XY.h>
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Fishboard
	{
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
		static void HandleMouseMotion(const std::string&, const std::string&, const common::XY<int>&);
		static common::XY<int> ReadCursor(const std::string&, const std::string&);
	};
}
