#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	struct Ships
	{
		constexpr Ships() {}
		constexpr Ship GetShip(int shipId) const { return Ship(shipId); }
		void Reset() const;
		constexpr void Populate(const Difficulty& difficulty) const {}
		void ApplyTurnEffects() const;
	};
}
