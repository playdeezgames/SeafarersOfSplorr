#include "Game.Session.Island.StreetVendor.MenuItem.h"
#include <Data.Game.StreetVendor.MenuItem.h>
namespace game::session::island::street_vendor
{
	using MenuItemData = data::game::street_vendor::MenuItem;

	std::string MenuItem::GetName() const
	{
		return MenuItemData::ReadName(menuItemId).value();
	}

	int MenuItem::GetPrice() const
	{
		return MenuItemData::ReadPrice(menuItemId).value();
	}
}