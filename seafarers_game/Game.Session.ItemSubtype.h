#pragma once
#include "Game.Session.ItemSubtype.Commodities.h"
namespace game::session
{
	struct ItemSubtype
	{
		constexpr ItemSubtype(int itemSubtypeId): itemSubtypeId(itemSubtypeId) {}
		constexpr explicit operator int() const { return itemSubtypeId; }
		constexpr item_subtype::Commodities GetCommodities() const { return item_subtype::Commodities(itemSubtypeId); }
		//TODO: determine base purchase value
		//TODO: determine base sell value
	private:
		int itemSubtypeId;
	};
}