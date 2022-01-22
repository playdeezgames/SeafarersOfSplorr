#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.h"
#include <vector>
namespace game::session::world
{
	struct Islands
	{
		Islands() = delete;
		static constexpr Island GetIsland(int islandId) { return Island(islandId); }
		static std::vector<Island> GetIslands();
		static void Reset();
		static void Populate(const Difficulty& difficulty);
		static void ApplyTurnEffects();
	};
}
