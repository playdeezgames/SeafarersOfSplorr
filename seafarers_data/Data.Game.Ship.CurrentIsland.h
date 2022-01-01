#pragma once
#include <optional>
namespace data::game::ship
{
	struct CurrentIsland
	{
		CurrentIsland() = delete;
		static void Initialize();
		static void Write(int shipId, int islandId);
		static std::optional<int> ReadIslandId(int shipId);
		static void ClearForShip(int shipId);
		static void ClearAll();
	};
}
