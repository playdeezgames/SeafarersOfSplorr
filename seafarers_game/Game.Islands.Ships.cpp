#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.Ships.h"
namespace game::islands
{
	static double GetPurchasePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& commodities = game::Ships::GetCommodities(ship);
		return Commodities::GetPurchasePrice(location, commodities);
	}



	double Ships::GetSalePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& commodities = game::Ships::GetCommodities(ship);
		return Commodities::GetSalePrice(location, commodities);
	}

	std::map<game::Ship, double> Ships::GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetPurchasePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}

	std::map<game::Ship, double> Ships::GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Ship, double> result;
		auto shipsAvailable = game::Ships::All();
		for (auto& ship : shipsAvailable)
		{
			double price = GetSalePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}
}