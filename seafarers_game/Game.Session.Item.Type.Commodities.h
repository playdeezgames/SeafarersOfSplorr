#pragma once
#include "Game.Session.Item.Type.Commodity.h"
#include <vector>
namespace game::session::item::type
{
	struct Commodities
	{
		constexpr explicit Commodities(int itemTypeId) : itemTypeId(itemTypeId) {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(itemTypeId, commodity); }
		std::vector<Commodity> GetAll() const;
	private:
		int itemTypeId;
	};
}
