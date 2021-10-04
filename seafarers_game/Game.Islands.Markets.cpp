#include <Data.Game.Island.Market.h>
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Game.Ships.h"
namespace game::islands
{
	static void BuyQuantities(const common::XY<double>& location, const std::map<Commodity, double> commodities, size_t quantity)
	{
		for (auto entry : commodities)
		{
			auto data = data::game::island::Market::Read(location, (int)entry.first);
			if (data)
			{
				auto market = data.value();
				market.purchases += (double)quantity * entry.second;
				data::game::island::Market::Write(location, (int)entry.first, market);
			}
		}
	}

	static void SellQuantities(const common::XY<double>& location, const std::map<Commodity, double> commodities, size_t quantity)
	{
		for (auto entry : commodities)
		{
			auto data = data::game::island::Market::Read(location, (int)entry.first);
			if (data)
			{
				auto market = data.value();
				market.sales += (double)quantity * entry.second;
				data::game::island::Market::Write(location, (int)entry.first, market);
			}
		}
	}

	void Markets::BuyItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto descriptor = game::Items::Read(item);
		BuyQuantities(location, descriptor.commodities, quantity);
	}

	void Markets::SellItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto descriptor = game::Items::Read(item);
		SellQuantities(location, descriptor.commodities, quantity);
	}

	void Markets::BuyShipType(const common::XY<double>& location, const game::ShipType& ship)
	{
		auto commodities = game::ShipTypes::GetCommodities(ship);
		BuyQuantities(location, commodities, 1);
	}

	void Markets::SellShipType(const common::XY<double>& location, const game::ShipType& ship)
	{
		auto commodities = game::ShipTypes::GetCommodities(ship);
		SellQuantities(location, commodities, 1);
	}
}