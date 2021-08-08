#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.Ships.h"
namespace game::islands::Ships
{
	static double GetShipPurchasePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		double price = 0.0;
		auto& shipDescriptor = game::Ships::Read(ship);
		for (auto shipCommodity : shipDescriptor.commodities)
		{
			auto commodity = shipCommodity.first;
			auto commodityAmount = shipCommodity.second;
			price +=
				commodityAmount * Commodities::GetCommodityUnitPurchasePrice(location, shipCommodity.first);
		}
		return price;
	}

	static double GetShipSellPrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		double price = 0.0;
		auto& shipDescriptor = game::Ships::Read(ship);
		for (auto shipCommodity : shipDescriptor.commodities)
		{
			auto commodity = shipCommodity.first;
			auto commodityAmount = shipCommodity.second;
			price +=
				commodityAmount * Commodities::GetCommodityUnitSalePrice(location, shipCommodity.first);
		}
		return price;
	}

	std::map<game::Ship, double> GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetShipPurchasePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}

	std::map<game::Ship, double> GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetShipSellPrice(location, ship);
			result[ship] = price;
		}
		return result;
	}
}