#include "Game.Ships.h"
#include "Game.Session.Ships.h"
namespace game::session
{
	Ship Ships::GetShip(int shipId) const
	{
		return Ship(shipId);
	}

	void Ships::Reset() const
	{
		game::Ships::Reset();
	}

	void Ships::ApplyTurnEffects() const
	{
		game::Ships::ApplyTurnEffects();
	}
}