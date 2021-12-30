#include "Game.Session.World.ShipType.h"
namespace game::session::world
{
	double ShipType::GetSpeedFactor() const
	{
		return
			(shipType == game::ShipType::RAFT) ? (1.0) :
			(shipType == game::ShipType::JUNK) ? (1.25) :
			(throw "NOPE!");
	}
}