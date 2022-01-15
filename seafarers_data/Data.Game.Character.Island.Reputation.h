#pragma once
#include <optional>
namespace data::game::character::island
{
	struct Reputation
	{
		Reputation() = delete;
		static void Initialize();
		static std::optional<int> Read(int characterId, int islandId);
		static void Write(int characterId, int islandId, int reputation);
	};
}
