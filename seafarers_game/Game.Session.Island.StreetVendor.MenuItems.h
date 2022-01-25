#pragma once
#include <vector>
#include "Game.Session.Island.StreetVendor.MenuItem.h"
namespace game::session::island::street_vendor
{
	struct MenuItems
	{
		constexpr explicit MenuItems(int featureId) : featureId(featureId) {}
		std::vector<MenuItem> GetMenuItems() const;
	private:
		int featureId;
	};
}
