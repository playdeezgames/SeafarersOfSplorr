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
#include "Game.Session.Island.Markets.h"
#include "Game.Session.World.h"
namespace game::session::island::street_vendor
{
	using MenuItemData = data::game::street_vendor::MenuItem;
	using PropertyData = data::game::item::type::Property;

	int MenuItem::GetPrice(int islandId) const
	{
		auto unitPrice = game::session::island::Markets(islandId).GetUnitPurchaseValue(GetItemTypeId());
		auto currencyUnitPrice = game::session::island::Markets(islandId).GetUnitPurchaseValue(World::GetCurrencyItemSubtype().operator int());
		return (int)std::ceil(unitPrice / currencyUnitPrice);
	}

	int MenuItem::GetItemTypeId() const
	{
		return MenuItemData::ReadItemTypeId(menuItemId).value();
	}

	void MenuItem::Give(int characterId) const
	{
		game::session::character::Messages(characterId).Add(game::Colors::GREEN, "You buy the item.");
		game::session::character::ItemTypes(characterId)
			.AddItemTypeQuantity(GetItemTypeId(), 1);
	}
}