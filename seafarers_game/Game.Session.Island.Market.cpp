#include <Data.Game.Island.Market.h>
#include "Game.Session.Commodity.h"
#include "Game.Session.Island.Market.h"
namespace game::session::island
{
	double Market::GetUnitPurchaseValue() const
	{
		auto currentCommodity = Commodity(this->commodity);
		auto market = data::game::island::Market::Read(islandId, (int)this->commodity).value();
		auto supply = currentCommodity.GetSupplyFactor() * market.sales + market.supply;
		auto demand = currentCommodity.GetDemandFactor() * market.purchases + market.demand;
		return currentCommodity.GetBasePrice() * demand / supply;
	}

	double Market::GetUnitSaleValue() const
	{
		auto currentCommodity = Commodity(this->commodity);
		return GetUnitPurchaseValue() * currentCommodity.GetDiscount();
	}
}