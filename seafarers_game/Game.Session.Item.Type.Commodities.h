#pragma once
#include "Game.Session.Item.Type.Commodity.h"
#include <list>
namespace game::session::item::type
{
	struct Commodities
	{
		constexpr explicit Commodities(int itemSubtypeId) : itemSubtypeId(itemSubtypeId) {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(itemSubtypeId, commodity); }
		std::list<Commodity> GetAll() const;
	private:
		int itemSubtypeId;
	};
}
