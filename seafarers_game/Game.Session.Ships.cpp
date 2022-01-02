#include <algorithm>
#include <Data.Game.Ship.h>
#include "Game.Session.Ships.h"
namespace game::session
{
	void Ships::Reset() const
	{
		data::game::Ship::Clear();
	}

	void Ships::ApplyTurnEffects() const
	{
		auto shipIds = data::game::Ship::All();
		std::for_each(
			shipIds.begin(), 
			shipIds.end(), 
			[](int shipId) 
			{
				Ship(shipId).ApplyTurnEffects();
			});
	}
}