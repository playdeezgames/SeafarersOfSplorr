#pragma once
#include <json.hpp>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Terminals
	{
		static void WriteText(const std::string& layoutName, const std::string& terminalName, const std::string& text);
		static void WriteLine(const std::string& layoutName, const std::string& terminalName, const std::string& text);
		static void SetForeground(const std::string& layoutName, const std::string& terminalName, const std::string_view& color);
		static void SetBackground(const std::string& layoutName, const std::string& terminalName, const std::string& color);
		static void Backspace(const std::string& layoutName, const std::string& terminalName);
		static DrawerFunction Internalize(const std::string& layoutName, const nlohmann::json& model);
	};
}