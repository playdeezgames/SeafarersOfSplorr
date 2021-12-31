#pragma once
#include <list>
#include <optional>
namespace data::game::character
{
	struct CurrentIsland
	{
		static void Initialize();
		static std::optional<int> Read(int characterId);
		static void Write(int characterId, int islandId);
		static void Clear(int characterId);
		static std::list<int> All(int islandId);
	};
}
