#pragma once
#include "Game.Session.Character.KnownIsland.h"

#include <vector>
namespace game::session::character
{
	struct KnownIslands
	{
		constexpr explicit KnownIslands(int characterId) : characterId(characterId) {}
		constexpr KnownIsland GetKnownIsland(const Island& island) const 
		{ 
			return KnownIsland(characterId, island.operator int()); 
		}
		void AddKnownIsland(int islandId) const;
		bool HasAny() const;
		std::vector<KnownIsland> GetAll() const;
	private:
		int characterId;
	};
}
