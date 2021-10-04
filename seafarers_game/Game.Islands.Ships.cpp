#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.ShipTypes.h"
namespace game::islands
{
	static double GetPurchasePrice(
		const common::XY<double>& location,
		const game::ShipType& ship)
	{
		auto& commodities = game::ShipTypes::GetCommodities(ship);
		return Commodities::GetPurchasePrice(location, commodities);
	}



	double Ships::GetSalePrice(
		const common::XY<double>& location,
		const game::ShipType& ship)
	{
		auto& commodities = game::ShipTypes::GetCommodities(ship);
		return Commodities::GetSalePrice(location, commodities);
	}

	std::map<game::ShipType, double> Ships::GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::ShipType, double> result;
		auto shipsAvailable = game::ShipTypes::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetPurchasePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}

	std::map<game::ShipType, double> Ships::GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::ShipType, double> result;
		auto shipsAvailable = game::ShipTypes::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetSalePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}
}