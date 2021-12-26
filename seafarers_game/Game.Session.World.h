#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.Item.Types.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Calendar.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Islands.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		constexpr World(int worldId) : worldId(worldId) {}
		constexpr item::Types GetItemSubtypes() const { return item::Types(); }
		constexpr Commodities GetCommodities() const { return Commodities(); }
		constexpr world::Bounds GetBounds() const { return world::Bounds(worldId); }
		constexpr world::Wind GetWind() const { return world::Wind(worldId); }
		constexpr world::Distances GetDistances() const { return world::Distances(worldId); }
		constexpr world::Calendar GetCalendar() const { return world::Calendar(worldId); }
		constexpr world::Islands GetIslands() const { return world::Islands(); }
		item::Type GetCurrencyItemSubtype() const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
		game::Difficulty GetDifficulty() const;
	private:
		int worldId;
	};
}
