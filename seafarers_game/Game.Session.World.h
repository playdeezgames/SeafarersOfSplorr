#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Item.Types.h"
#include "Game.Session.SkillCategories.h"
#include "Game.Session.Tribes.h"
#include "Game.Session.World.Bounds.h"
#include "Game.Session.World.Calendar.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.Islands.h"
#include "Game.Session.World.Professions.h"
#include "Game.Session.World.ShipTypes.h"
#include "Game.Session.World.Skills.h"
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

		constexpr session::Demigods GetDemigods() const { return session::Demigods(); }
		constexpr Commodities GetCommodities() const { return Commodities(); }
		constexpr world::Islands GetIslands() const { return world::Islands(); }
		constexpr item::Types GetItemSubtypes() const { return item::Types(); }
		constexpr Tribes GetTribes() const { return Tribes(); }
		constexpr world::ShipTypes GetShipTypes() const { return world::ShipTypes(); }
		constexpr world::Skills GetSkills() const { return world::Skills(); }
		constexpr world::Professions GetProfessions() const { return world::Professions(); }
		constexpr SkillCategories GetSkillCategories() const { return SkillCategories(); }

		item::Type GetCurrencyItemSubtype() const;
		game::Difficulty GetDifficulty() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int worldId;
	};
}
