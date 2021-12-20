#pragma once
#include "Game.Session.Island.Market.h"
#include "Game.Session.ItemSubtype.h"
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
		constexpr Market GetMarket(const game::Commodity& commodity) const { return Market(islandId, commodity); }
		double GetPurchaseValue(const ItemSubtype&) const;
		int GetPurchaseQuantity(const ItemSubtype&, double) const;
		double GetSaleValue(const ItemSubtype&) const;
		int GetSaleQuantity(const ItemSubtype&, double) const;
	private:
		int islandId;
	};
}
