#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		World();
		world::Bounds GetBounds() const;
		world::Wind GetWind() const;
		world::Distances GetDistances() const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
		int GetVersion() const;
	};
}
