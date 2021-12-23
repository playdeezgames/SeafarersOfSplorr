#pragma once
#include "Game.Session.Island.h"
namespace game::session::character
{
	struct KnownIsland
	{
		constexpr KnownIsland(int characterId, int islandId) : characterId(characterId), islandId(islandId) {}
		constexpr Island GetIsland() const { return Island(islandId); }

		bool IsKnown() const;
		std::string GetDisplayName() const;
	private:
		int characterId;
		int islandId;
	};
}
