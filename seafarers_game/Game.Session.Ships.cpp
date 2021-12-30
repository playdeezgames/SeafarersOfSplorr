#include <Data.Game.Ship.h>
#include "Game.Session.Ships.h"
namespace game::session
{
	Ship Ships::GetShip(int shipId) const
	{
		return Ship(shipId);
	}

	void Ships::Reset() const
	{
		data::game::Ship::Clear();
	}

	void Ships::ApplyTurnEffects() const
	{
		auto shipIds = data::game::Ship::All();
		for (auto shipId : shipIds)
		{
			Ship(shipId).ApplyTurnEffects();
		}
	}
}