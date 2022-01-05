#pragma once
#include <Common.XY.h>
#include <vector>
#include <optional>
#include <string>
namespace data::game::character
{
	struct Mark
	{
		Mark() = delete;
		static void Initialize();
		static std::optional<common::XY<double>> ReadMark(int characterId, const std::string& name);
		static bool HasAny(int characterId);
		static std::vector<std::string> ReadMarkNames(int characterId);
		static void WriteMark(int characterId, const std::string& name, const common::XY<double>& location);
		static void ClearMark(int characterId, const std::string& name);
	};
}