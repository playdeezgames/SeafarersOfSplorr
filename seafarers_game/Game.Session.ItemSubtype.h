#pragma once
#include "Game.Session.ItemSubtype.Commodities.h"
#include <string>
namespace game::session
{
	struct ItemSubtype
	{
		constexpr ItemSubtype(int itemSubtypeId): itemSubtypeId(itemSubtypeId) {}
		constexpr explicit operator int() const { return itemSubtypeId; }
		constexpr item_subtype::Commodities GetCommodities() const { return item_subtype::Commodities(itemSubtypeId); }
		constexpr bool operator==(const ItemSubtype& other) const { return itemSubtypeId == other.itemSubtypeId; }
		std::string GetName() const;
	private:
		int itemSubtypeId;
	};
}