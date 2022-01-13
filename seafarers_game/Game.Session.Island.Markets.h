#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.Market.h"
#include "Game.Session.Item.Type.h"
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
		constexpr Market GetMarket(const game::Commodity& commodity) const { return Market(islandId, commodity); }
		double GetUnitPurchaseValue(const item::Type& itemType) const;
		int GetMaximumPurchaseQuantity(const item::Type& itemType, double) const;
		double GetUnitSaleValue(const item::Type& itemType) const;
		int GetMaximumSaleQuantity(const item::Type& itemType, double) const;
		void Populate(const game::Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int islandId;
	};
}
