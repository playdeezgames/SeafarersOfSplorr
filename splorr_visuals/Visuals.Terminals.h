#pragma once
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Terminals
	{
		static void WriteText(const std::string&, const std::string&, const std::string&);
		static void WriteLine(const std::string&, const std::string&, const std::string&);
		static void SetForeground(const std::string&, const std::string&, const std::string_view&);
		static void SetBackground(const std::string&, const std::string&, const std::string&);
		static void Backspace(const std::string&, const std::string&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}