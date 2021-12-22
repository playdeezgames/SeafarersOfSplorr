#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.Item.Types.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Calendar.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	struct World
	{
		constexpr World() : worldId(0) {}
		constexpr item::Types GetItemSubtypes() const { return item::Types(); }
		constexpr Commodities GetCommodities() const { return Commodities(); }
		constexpr world::Bounds GetBounds() const { return world::Bounds(); }
		constexpr world::Wind GetWind() const { return world::Wind(); }
		constexpr world::Distances GetDistances() const { return world::Distances(); }
		constexpr world::Calendar GetCalendar() const { return world::Calendar(worldId); }
		item::Type GetCurrencyItemSubtype() const;
		void Reset(const Difficulty&);
		void ApplyTurnEffects() const;
		int GetVersion() const;
	private:
		int worldId;
	};
}
