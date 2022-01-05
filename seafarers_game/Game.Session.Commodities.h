#pragma once
#include "Game.Session.Commodity.h"
#include <vector>
namespace game::session
{
	struct Commodities
	{
		constexpr Commodities() {}
		constexpr Commodity GetCommodity(const game::Commodity& commodity) const { return Commodity(commodity); }
		const std::vector<Commodity>& GetAll() const;
		void Reset() const {}
	};
}
