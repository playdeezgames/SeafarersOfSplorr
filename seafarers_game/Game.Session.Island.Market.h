#pragma once
#include "Game.Commodity.h"
namespace game::session::island
{
	struct Market
	{
		constexpr Market(int islandId, const game::Commodity& commodity) : islandId(islandId), commodity(commodity) {}
		double GetUnitPurchaseValue() const;
		double GetUnitSaleValue() const;
	private:
		int islandId;
		game::Commodity commodity;
	};
}
