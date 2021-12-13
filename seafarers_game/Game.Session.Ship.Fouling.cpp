#include "Game.Ship.Statistics.h"
#include "Game.Session.Ship.Fouling.h"
namespace game::session::ship
{
	Fouling::Fouling(int shipId, const Side& side)
		: shipId(shipId)
		, side(side)
	{
	}

	double Fouling::GetPercentage()
	{
		return 
			game::ship::Statistics::GetFoulingPercentage(
			shipId,
			game::Side::PORT);
	}
}