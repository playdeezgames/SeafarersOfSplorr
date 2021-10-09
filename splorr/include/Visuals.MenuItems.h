#pragma once
#include <Common.Utility.h>
#include <format>
#include <functional>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct MenuItems
	{
		static void SetText(const std::string&, const std::string&, const std::string&);
		template<typename ...Ts>
		static void SetText(const std::string& layoutName, const std::string& menuItemId, const std::string& formatString, Ts... args)
		{
			SetText(layoutName, menuItemId, std::format(formatString, args...));
		}
		static void SetEnabled(const std::string&, const std::string&, bool);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}