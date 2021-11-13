#include <Data.Game.Ship.h>
#include "Game.Ship.h"
#include "Game.Ships.h"
namespace game
{
	void Ships::Reset(const game::Difficulty&)
	{
		data::game::Ship::Clear();
	}

	void Ships::ApplyTurnEffects()
	{
		auto shipIds = data::game::Ship::All();
		for (auto shipId : shipIds)
		{
			game::Ship::ApplyTurnEffects(shipId);
		}
	}
}