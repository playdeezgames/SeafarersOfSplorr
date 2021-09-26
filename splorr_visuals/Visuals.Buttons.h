#pragma once
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Buttons
	{
		static void SetHoverButton(const std::string&, const std::string&);
		static void ClearHoverButton(const std::string&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}