#pragma once
#include <Common.XY.h>
#include <format>
#include <json.hpp>
#include <optional>
#include <string>
#include "Visuals.DrawerFunction.h"
namespace visuals
{
	struct Texts
	{
		static void SetText(const std::string&, const std::string&, const std::optional<std::string>&);
		template<typename ...Ts>
		static void SetText(const std::string& layoutName, const std::string& textId, const std::string& formatString, Ts... args)
		{
			SetText(layoutName, textId, std::optional<std::string>(std::format(formatString, args...)));
		}
		static void SetColor(const std::string&, const std::string&, const std::string&);
		static void SetLocation(const std::string&, const std::string&, const common::XY<int>&);
		static DrawerFunction Internalize(const std::string&, const nlohmann::json&);
	};
}