#include "Game.Session.Island.StreetVendor.h"
#include <algorithm>
#include <map>
#include <Common.RNG.h>
#include <Data.Game.StreetVendor.MenuItem.h>
namespace game::session::island
{
	static const std::map<size_t, size_t> foodMenuSizeGenerator =
	{
		{2, 2},
		{3, 1}
	};

	void StreetVendor::Populate(const game::Difficulty&) const
	{
		auto itemCount = common::RNG::FromGenerator(foodMenuSizeGenerator);
		int cost = common::RNG::FromRange(1,3);
		int satiety = 5;
		while (itemCount > 0)
		{
			auto cookingSkill = common::RNG::FromRange(80, 100);
			data::game::street_vendor::MenuItem::Create(featureId, "BAD_NAME", cost, satiety, cookingSkill);
			cost += common::RNG::FromRange(1, 3);
			satiety *= 2;
			itemCount--;
		}
	}

	void StreetVendor::ApplyTurnEffects() const
	{
		//deliberately blank... maybe change the menu from time to time?
	}
}