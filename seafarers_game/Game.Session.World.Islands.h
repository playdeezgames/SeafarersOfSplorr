#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.h"
#include <vector>
namespace game::session::world
{
	struct Islands
	{
		constexpr Islands() {}
		constexpr Island GetIsland(int islandId) const { return Island(islandId); }
		std::vector<Island> GetIslands() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	};
}
