#include "Game.Session.Island.StreetVendor.MenuItem.h"
#include <Data.Game.StreetVendor.MenuItem.h>
#include <Common.RNG.h>
#include "Game.Session.Character.Counter.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Colors.h"
#include "Game.Session.Character.Plights.h"
#include <Data.Game.Item.Type.Property.h>
#include "Game.Session.Character.h"
namespace game::session::island::street_vendor
{
	using MenuItemData = data::game::street_vendor::MenuItem;
	using PropertyData = data::game::item::type::Property;

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
		auto itemTypeId = GetItemTypeId();
		//TODO: give character the item
		game::session::Character(characterId).Eat(itemTypeId);
	}
}