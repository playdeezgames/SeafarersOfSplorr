#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.Market.h"
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
		constexpr Market GetMarket(const game::Commodity& commodity) const { return Market(islandId, commodity); }
		double GetUnitPurchaseValue(int itemType) const;
		int GetMaximumPurchaseQuantity(int itemType, double) const;
		double GetUnitSaleValue(int itemType) const;
		int GetMaximumSaleQuantity(int itemType, double) const;
		void Populate(const game::Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int islandId;
	};
}
