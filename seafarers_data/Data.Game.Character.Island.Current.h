#pragma once
#include <vector>
#include <optional>
namespace data::game::character
{
	struct CurrentIsland
	{
		CurrentIsland() = delete;
		static void Initialize();
		static std::optional<int> Read(int characterId);
		static void Write(int characterId, int islandId);
		static void Clear(int characterId);
		static std::vector<int> All(int islandId);
	};
}
