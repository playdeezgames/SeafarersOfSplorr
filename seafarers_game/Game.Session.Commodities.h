#pragma once
#include "Game.Session.Commodity.h"
#include <list>
namespace game::session
{
	struct Commodities
	{
		constexpr Commodities() {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(commodity); }
		const std::list<Commodity>& GetAll() const;
	};
}
