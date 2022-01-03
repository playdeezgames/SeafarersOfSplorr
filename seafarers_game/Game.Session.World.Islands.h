#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.h"
#include <list>
namespace game::session::world
{
	struct Islands
	{
		constexpr Islands() {}
		constexpr Island GetIsland(int islandId) const { return Island(islandId); }
		std::list<Island> GetAll() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	};
}
