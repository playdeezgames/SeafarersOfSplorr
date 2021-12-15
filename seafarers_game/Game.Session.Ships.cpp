#include "Game.Ships.h"
#include "Game.Session.Ships.h"
namespace game::session
{
	Ship Ships::GetShip(int shipId) const
	{
		return Ship(shipId);
	}

	void Ships::Reset(const Difficulty& difficulty) const
	{
		game::Ships::Reset(difficulty);
	}

	void Ships::ApplyTurnEffects() const
	{
		game::Ships::ApplyTurnEffects();
	}
}