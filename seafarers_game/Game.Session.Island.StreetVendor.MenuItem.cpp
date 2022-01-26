#include "Game.Session.Island.StreetVendor.MenuItem.h"
#include <Data.Game.StreetVendor.MenuItem.h>
#include <Common.RNG.h>
#include "Game.Session.Character.Counter.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Colors.h"
#include "Game.Session.Character.Plights.h"
namespace game::session::island::street_vendor
{
	using MenuItemData = data::game::street_vendor::MenuItem;

	int MenuItem::GetPrice() const
	{
		return MenuItemData::ReadPrice(menuItemId).value();
	}

	int MenuItem::GetItemTypeId() const
	{
		return MenuItemData::ReadItemTypeId(menuItemId).value();
	}

	void MenuItem::Feed(int characterId) const
	{
		auto cookingSkill = MenuItemData::ReadCookingSkill(menuItemId).value();
		auto satiety = MenuItemData::ReadSatiety(menuItemId).value();
		if (common::RNG::Roll<100>() <= cookingSkill)
		{
			auto counter = game::session::character::Counter(characterId, game::characters::Counter::SATIETY);
			counter.Change(satiety);
			game::session::character::Messages(characterId).Add(game::Colors::GREEN, "Delicious!");
		}
		else
		{
			game::session::character::Plights(characterId).Inflict(game::characters::Plight::FOOD_POISONING, satiety);
			game::session::character::Messages(characterId).Add(game::Colors::RED, "You got food poisoning!");
		}
	}
}