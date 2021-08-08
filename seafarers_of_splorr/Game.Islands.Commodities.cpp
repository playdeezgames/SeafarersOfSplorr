#include "Data.Game.Island.Market.h"
#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
namespace game::islands::Commodities
{
	double GetCommodityUnitPurchasePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		auto& commodityDescriptor = game::Commodities::Read(commodity);
		auto market = data::game::island::Market::Read(location, (int)commodity).value();
		return commodityDescriptor.basePrice *
			(market.demand + (double)market.purchases * commodityDescriptor.demandFactor) /
			(market.supply + (double)market.sales * commodityDescriptor.supplyFactor);
	}

	double GetCommodityUnitSalePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		auto& commodityDescriptor = game::Commodities::Read(commodity);
		auto market = data::game::island::Market::Read(location, (int)commodity).value();
		return commodityDescriptor.basePrice *
			(market.demand + (double)market.purchases * commodityDescriptor.demandFactor) /
			(market.supply + (double)market.sales * commodityDescriptor.supplyFactor) *
			(1.0 - commodityDescriptor.discount);
	}
}