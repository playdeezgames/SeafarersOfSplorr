#pragma once
#include <map>
#include <optional>
namespace data::game::island
{
	struct Tribe
	{
		Tribe() = delete;
		static void Initialize();
		static void Clear();
		static void Write(int islandId, int tribeId, int presence);
		static std::optional<size_t> ReadPresence(int islandId, int tribeId);
		static std::map<int, size_t> All(int);
		static std::map<int, size_t> AllTotals();
	};
}
