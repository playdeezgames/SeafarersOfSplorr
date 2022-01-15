#pragma once
#include <vector>
namespace data::game::character::island
{
	struct Known
	{
		Known() = delete;
		static void Initialize();
		static void Write(int characterId, int islandId);
		static bool Has(int characterId, int islandId);
		static void Clear(int characterId);
		static void Clear();
		static std::vector<int> All(int characterId);
	};
}
