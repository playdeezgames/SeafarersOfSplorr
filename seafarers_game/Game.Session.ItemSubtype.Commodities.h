#pragma once
#include "Game.Session.ItemSubtype.Commodity.h"
#include <list>
namespace game::session::item_subtype
{
	struct Commodities
	{
		constexpr Commodities(int itemSubtypeId) : itemSubtypeId(itemSubtypeId) {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(itemSubtypeId, commodity); }
		std::list<Commodity> GetAll() const;
	private:
		int itemSubtypeId;
	};
}
