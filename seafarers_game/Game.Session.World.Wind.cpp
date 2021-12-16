#include "Game.Session.World.Wind.h"
#include "Game.World.h"
namespace game::session::world
{
	Wind::Wind()
	{

	}

	double Wind::GetHeading() const
	{
		return game::World::GetWindHeadingLegacy();
	}

	double Wind::GetMultiplier(double angle) const
	{
		return game::World::GetWindSpeedMultiplierLegacy(angle);
	}

	void Wind::SetHeading(double heading) const
	{
		game::World::SetWindHeadingLegacy(heading);
	}

}