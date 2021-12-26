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
		//bool HasAny() const;
		//std::optional<Island> TryGetFirst() const;
		//Island GetFirst() const;
		//size_t GetCount() const;
		std::list<Island> GetAll() const;
		//void Reset(const Difficulty&) const;
		//void ApplyTurnEffects() const;
	};
}
