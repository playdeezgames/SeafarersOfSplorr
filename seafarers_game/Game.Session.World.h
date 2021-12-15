#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		World();
		world::Wind GetWind() const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
	};
}
