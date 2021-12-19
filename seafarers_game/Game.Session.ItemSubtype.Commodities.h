#pragma once
#include "Game.Session.ItemSubtype.Commodity.h"
namespace game::session::item_subtype
{
	struct Commodities
	{
		constexpr Commodities(int itemSubtypeId) : itemSubtypeId(itemSubtypeId) {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(itemSubtypeId, commodity); }
	private:
		int itemSubtypeId;
	};
}
