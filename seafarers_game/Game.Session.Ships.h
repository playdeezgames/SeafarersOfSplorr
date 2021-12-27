#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	struct Ships
	{
		Ship GetShip(int) const;
		void Reset() const;
		void Populate(const Difficulty&) const;
		void ApplyTurnEffects() const;
	};
}
