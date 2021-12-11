#include "Game.Session.World.Wind.h"
#include "Game.World.h"
namespace game::session::world
{
	Wind::Wind()
	{

	}

	double Wind::GetHeading() const
	{
		return game::World::GetWindHeading();
	}

	double Wind::GetMultiplier(double angle) const
	{
		return game::World::GetWindSpeedMultiplier(angle);
	}

}