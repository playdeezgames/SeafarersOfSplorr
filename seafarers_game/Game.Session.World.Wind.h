#pragma once
#include "Game.Session.World.h"
namespace game::session::world
{
	struct Wind
	{
		constexpr explicit Wind() : worldId(WORLD_ID) {};
		double GetHeading() const;
		double GetSpeedMultiplier(double heading) const;
		void SetHeading(double heading) const;
		void ApplyTurnEffects() const;
		void Reset() const {}
	private:
		int worldId;
	};
}
