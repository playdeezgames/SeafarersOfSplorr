#include <Data.Game.Island.Market.h>
#include "Game.Session.Commodity.h"
#include "Game.Session.Island.Market.h"
namespace game::session::island
{
	double Market::GetUnitPurchaseValue() const
	{
		auto commodity = Commodity(this->commodity);
		auto market = data::game::island::Market::Read(islandId, (int)this->commodity).value();
		auto supply = commodity.GetSupplyFactor() * market.sales + market.supply;
		auto demand = commodity.GetDemandFactor() * market.purchases + market.demand;
		return commodity.GetBasePrice() * demand / supply;
	}

	double Market::GetUnitSaleValue() const
	{
		auto commodity = Commodity(this->commodity);
		return GetUnitPurchaseValue() * commodity.GetDiscount();
	}
}