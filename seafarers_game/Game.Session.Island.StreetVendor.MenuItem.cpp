#include "Game.Session.Island.StreetVendor.MenuItem.h"
#include "Game.Colors.h"
#include "Game.Session.Character.Counter.h"
#include "Game.Session.Character.h"
#include "Game.Session.Character.ItemTypes.h"
#include "Game.Session.Character.Messages.h"
#include "Game.Session.Character.Plights.h"
#include <Common.RNG.h>
#include <Data.Game.Item.Type.Property.h>
#include <Data.Game.StreetVendor.MenuItem.h>
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

	void MenuItem::Give(int characterId) const
	{
		game::session::character::ItemTypes(characterId)
			.AddItemTypeQuantity(GetItemTypeId(), 1);
	}
}