#pragma once
#include <string>
namespace game::session::island::street_vendor
{
	struct MenuItem
	{
		constexpr explicit MenuItem(int menuItemId) : menuItemId(menuItemId) {}
		constexpr int ToId() const { return menuItemId; }
		std::string GetName() const;
		int GetPrice() const;
	private:
		int menuItemId;
	};
}