#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.World.Wind.h"
#include "Game.Session.World.Bounds.h"
namespace game::session
{
	struct World
	{
		World();
		world::Bounds GetBounds() const;
		world::Wind GetWind() const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
		int GetVersion() const;
	};
}
