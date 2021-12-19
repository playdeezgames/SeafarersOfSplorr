#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.ItemSubtypes.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		constexpr World() {}
		constexpr ItemSubtypes GetItemSubtypes() const { return ItemSubtypes(); }
		constexpr Commodities GetCommodities() const { return Commodities(); }
		constexpr world::Bounds GetBounds() const { return world::Bounds(); }
		constexpr world::Wind GetWind() const { return world::Wind(); }
		constexpr world::Distances GetDistances() const { return world::Distances(); }
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
		int GetVersion() const;
	};
}
