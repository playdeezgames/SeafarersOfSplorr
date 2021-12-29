#include <Data.Game.Island.Market.h>
#include <functional>
#include "Game.Islands.Markets.h"
#include "Game.ShipTypes.h"
namespace game::islands
{
	static double& Purchases(
		data::game::island::Market& market)
	{
		return market.purchases;
	}

	static double& Sales(
		data::game::island::Market& market)
	{
		return market.sales;
	}
	
	static void ExchangeQuantities(int islandId, const std::map<Commodity, double> commodities, size_t quantity, std::function<double&(data::game::island::Market&)> quantifier)
	{
		for (auto entry : commodities)
		{
			auto data = data::game::island::Market::Read(islandId, (int)entry.first);
			if (data)
			{
				auto market = data.value();
				quantifier(market) += (double)quantity * entry.second;
				data::game::island::Market::Write(islandId, (int)entry.first, market);
			}
		}
	}

	static void BuyQuantities(int islandId, const std::map<Commodity, double> commodities, size_t quantity)
	{
		ExchangeQuantities(islandId, commodities, quantity, Purchases);
	}

	static void SellQuantities(int islandId, const std::map<Commodity, double> commodities, size_t quantity)
	{
		ExchangeQuantities(islandId, commodities, quantity, Sales);
	}

	void Markets::BuyShipType(int islandId, const game::ShipType& ship)
	{
		BuyQuantities(islandId, game::ShipTypes::GetCommodities(ship), 1);
	}

	void Markets::SellShipType(int islandId, const game::ShipType& ship)
	{
		SellQuantities(islandId, game::ShipTypes::GetCommodities(ship), 1);
	}
}