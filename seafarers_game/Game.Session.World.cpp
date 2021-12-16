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
		GetWind().ApplyTurnEffects();
	}
}