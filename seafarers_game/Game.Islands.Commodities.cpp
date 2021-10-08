#include <Data.Game.Island.Market.h>
#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
namespace game::islands
{
	static double GetPurchasePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		auto market = data::game::island::Market::Read(location, (int)commodity).value();
		return 
			game::Commodities::GetBasePrice(commodity) *
			(market.demand + (double)market.purchases * game::Commodities::GetDemandFactor(commodity)) /
			(market.supply + (double)market.sales * game::Commodities::GetSupplyFactor(commodity));
	}

	static double GetSalePrice(const common::XY<double>& location, const game::Commodity& commodity)
	{
		return 
			GetPurchasePrice(location, commodity) *
			(1.0 - game::Commodities::GetDiscount(commodity));
	}

	double Commodities::GetPurchasePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		double price = 0.0;
		for (auto entry : table)
		{
			auto commodity = entry.first;
			auto commodityAmount = entry.second;
			price +=
				commodityAmount * game::islands::GetPurchasePrice(location, entry.first);
		}
		return price;
	}

	double Commodities::GetSalePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		double price = 0.0;
		for (auto entry : table)
		{
			auto commodity = entry.first;
			auto commodityAmount = entry.second;
			price +=
				commodityAmount * game::islands::GetSalePrice(location, entry.first);
		}
		return price;
	}
}