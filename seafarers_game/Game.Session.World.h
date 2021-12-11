#pragma once
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		World();
		world::Wind GetWind() const;
	};
}
