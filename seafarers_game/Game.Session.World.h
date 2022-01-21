#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Item.Type.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Calendar.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Wind.h"
namespace game::session
{
	constexpr int WORLD_ID = 1;
	struct World
	{
		constexpr explicit World() : worldId(WORLD_ID) {}

		constexpr world::Bounds GetBounds() const { return world::Bounds(worldId); }
		constexpr world::Calendar GetCalendar() const { return world::Calendar(worldId); }
		constexpr world::Distances GetDistances() const { return world::Distances(worldId); }
		constexpr world::Wind GetWind() const { return world::Wind(worldId); }

		item::Type GetCurrencyItemSubtype() const;
		game::Difficulty GetDifficulty() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int worldId;
	};
}
