#pragma once
#include <string>
namespace game::session::character
{
	struct KnownIsland
	{
		constexpr KnownIsland(int characterId, int islandId) 
			: characterId(characterId)
			, islandId(islandId) {}
		constexpr int GetIslandId() const { return islandId; }

		bool IsKnown() const;
		std::string GetDisplayName() const;
	private:
		int characterId;
		int islandId;
	};
}
