#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game::character
{
	struct Mark
	{
		static void Initialize();
		static std::optional<common::XY<double>> ReadMark(int characterId, const std::string& name);
		static bool HasAny(int characterId);
		static std::list<std::string> ReadMarkNames(int characterId);
	};
}