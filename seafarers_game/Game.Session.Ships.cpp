#include "Game.Session.Ships.h"
namespace game::session
{
	Ship Ships::GetShip(int shipId) const
	{
		return Ship(shipId);
	}
}