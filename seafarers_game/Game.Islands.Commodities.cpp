#include <Data.Game.Island.Market.h>
#include <functional>
#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
namespace game::islands//20211014
{
	static double GetCommodityUnitPurchasePrice(
		const common::XY<double>& location, 
		const game::Commodity& commodity)
	{
		auto market = data::game::island::Market::Read(location, (int)commodity).value();
		return 
			game::Commodities::GetBasePrice(commodity) *
			(market.demand + (double)market.purchases * game::Commodities::GetDemandFactor(commodity)) /
			(market.supply + (double)market.sales * game::Commodities::GetSupplyFactor(commodity));
	}

	static double GetCommodityUnitSalePrice(
		const common::XY<double>& location, 
		const game::Commodity& commodity)
	{
		return 
			GetCommodityUnitPurchasePrice(location, commodity) *
			(1.0 - game::Commodities::GetDiscount(commodity));
	}

	static double DeterminePrice(const std::map<Commodity, double>& table, std::function<double(const Commodity&)> unitPricer)
	{
		double price = 0.0;
		for (auto entry : table)
		{
			price +=
				entry.second * unitPricer(entry.first);
		}
		return price;
	}

	static std::function<double(const Commodity&)> ToPricer(
		const common::XY<double>& location, 
		std::function<double(const common::XY<double>&,const game::Commodity&)> unitPricer)
	{
		return [location, unitPricer](const Commodity& commodity)
		{
			return unitPricer(location, commodity);
		};
	}

	double Commodities::GetPurchasePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		return DeterminePrice(table, ToPricer(location, GetCommodityUnitPurchasePrice));
	}

	double Commodities::GetSalePrice(const common::XY<double>& location, const std::map<Commodity, double>& table)
	{
		return DeterminePrice(table, ToPricer(location, GetCommodityUnitSalePrice));
	}
}