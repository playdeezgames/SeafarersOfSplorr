#include "Game.Session.Island.StreetVendor.MenuItems.h"
#include <Data.Game.StreetVendor.MenuItem.h>
#include <algorithm>
#include <iterator>
namespace game::session::island::street_vendor
{
	std::vector<MenuItem> MenuItems::GetMenuItems() const
	{
		auto menuIds = data::game::street_vendor::MenuItem::ReadForFeature(featureId);
		std::vector<MenuItem> result;
		std::transform(
			menuIds.begin(),
			menuIds.end(),
			std::back_inserter(result),
			[](int menuId) 
			{
				return MenuItem(menuId);
			});
		return result;
	}
}