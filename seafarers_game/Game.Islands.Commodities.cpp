#include <Data.Game.Island.Market.h>
#include <functional>
#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
#include "Game.Session.h"
namespace game::islands
{
	static double GetCommodityUnitPurchasePrice(
		int islandId, 
		const game::Commodity& commodity)
	{
		auto market = data::game::island::Market::Read(islandId, (int)commodity).value();
		return 
			game::Commodities::GetBasePrice(commodity) *
			(market.demand + (double)market.purchases * game::Commodities::GetDemandFactor(commodity)) /
			(market.supply + (double)market.sales * game::Session().GetWorld().GetCommodities().GetCommodity(commodity).GetSupplyFactor());
	}

	static double GetCommodityUnitSalePrice(
		int islandId, 
		const game::Commodity& commodity)
	{
		return 
			GetCommodityUnitPurchasePrice(islandId, commodity) *
			(1.0 - game::Session().GetWorld().GetCommodities().GetCommodity(commodity).GetDiscount());
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
		int islandId, 
		std::function<double(int,const game::Commodity&)> unitPricer)
	{
		return [islandId, unitPricer](const Commodity& commodity)
		{
			return unitPricer(islandId, commodity);
		};
	}

	double Commodities::GetPurchasePrice(int islandId, const std::map<Commodity, double>& table)
	{
		return DeterminePrice(table, ToPricer(islandId, GetCommodityUnitPurchasePrice));
	}

	double Commodities::GetSalePrice(int islandId, const std::map<Commodity, double>& table)
	{
		return DeterminePrice(table, ToPricer(islandId, GetCommodityUnitSalePrice));
	}
}