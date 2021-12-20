#pragma once
#include "Game.Session.Island.Market.h"
#include "Game.Session.Item.Type.h"
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
		constexpr Market GetMarket(const game::Commodity& commodity) const { return Market(islandId, commodity); }
		double GetPurchaseValue(const item::Type&) const;
		int GetPurchaseQuantity(const item::Type&, double) const;
		double GetSaleValue(const item::Type&) const;
		int GetSaleQuantity(const item::Type&, double) const;
	private:
		int islandId;
	};
}
