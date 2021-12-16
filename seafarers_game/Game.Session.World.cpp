#include <Common.RNG.h>
#include "Game.Session.World.h"
#include "Game.World.h"
namespace game::session
{
	World::World()
	{

	}

	world::Wind World::GetWind() const
	{
		return world::Wind();
	}

	void World::Reset(const Difficulty& difficulty) const
	{
		game::World::Reset(difficulty);
	}

	void World::ApplyTurnEffects() const
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		game::World::SetWindHeadingLegacy(game::World::GetWindHeadingLegacy() + common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}
}