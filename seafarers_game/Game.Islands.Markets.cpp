#include <Data.Game.Island.h>
#include <Data.Game.Island.Market.h>
#include <functional>
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Game.ShipTypes.h"
namespace game::islands//20211014
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
	
	static void ExchangeQuantities(const common::XY<double>& location, const std::map<Commodity, double> commodities, size_t quantity, std::function<double&(data::game::island::Market&)> quantifier)
	{
		auto islandId = data::game::Island::Find(location).value();
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

	static void BuyQuantities(const common::XY<double>& location, const std::map<Commodity, double> commodities, size_t quantity)
	{
		ExchangeQuantities(location, commodities, quantity, Purchases);
	}

	static void SellQuantities(const common::XY<double>& location, const std::map<Commodity, double> commodities, size_t quantity)
	{
		ExchangeQuantities(location, commodities, quantity, Sales);
	}

	void Markets::BuyItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		BuyQuantities(location, game::Items::GetCommodities(item), quantity);
	}

	void Markets::SellItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		SellQuantities(location, game::Items::GetCommodities(item), quantity);
	}

	void Markets::BuyShipType(const common::XY<double>& location, const game::ShipType& ship)
	{
		BuyQuantities(location, game::ShipTypes::GetCommodities(ship), 1);
	}

	void Markets::SellShipType(const common::XY<double>& location, const game::ShipType& ship)
	{
		SellQuantities(location, game::ShipTypes::GetCommodities(ship), 1);
	}
}