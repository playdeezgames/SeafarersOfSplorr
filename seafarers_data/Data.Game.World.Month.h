#pragma once
#include <string>
#include <vector>
#include <optional>
namespace data::game::world
{
	struct Month
	{
		Month() = delete;
		static void Initialize();
		static std::vector<int> AllOrdinals(int worldId);
		static void Write(int worldId, int ordinal, const std::string& name, int days);
		static std::optional<std::string> ReadName(int worldId, int ordinal);
		static std::optional<int> ReadDays(int worldId, int ordinal);
		static int YearLength(int worldId);
		static void Clear(int worldId);
	};
}