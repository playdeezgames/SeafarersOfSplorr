#pragma once
#include <string>
namespace game::session::island::street_vendor
{
	struct MenuItem
	{
		constexpr explicit MenuItem(int menuItemId) : menuItemId(menuItemId) {}
		constexpr int ToId() const { return menuItemId; }
		int GetItemTypeId() const;
		int GetPrice(int islandId) const;
		void Give(int characterId) const;
	private:
		int menuItemId;
	};
}