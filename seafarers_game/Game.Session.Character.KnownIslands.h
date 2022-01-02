#pragma once
#include "Game.Session.Character.KnownIsland.h"
#include "Game.Session.Island.h"
#include <list>
namespace game::session::character
{
	struct KnownIslands
	{
		constexpr explicit KnownIslands(int characterId) : characterId(characterId) {}
		constexpr KnownIsland GetKnownIsland(const Island& island) const 
		{ 
			return KnownIsland(characterId, island.operator int()); 
		}
		void AddKnownIsland(const Island& island) const;
		bool HasAny() const;
		std::list<KnownIsland> GetAll() const;
	private:
		int characterId;
	};
}
