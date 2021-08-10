#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.Ships.h"
namespace game::islands::Ships
{
	static double GetShipPurchasePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& shipDescriptor = game::Ships::Read(ship);
		return Commodities::GetPurchasePrice(location, shipDescriptor.commodities);
	}

	double GetShipSalePrice(
		const common::XY<double>& location,
		const game::Ship& ship)
	{
		auto& shipDescriptor = game::Ships::Read(ship);
		return Commodities::GetSalePrice(location, shipDescriptor.commodities);
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
			double price = GetShipSalePrice(location, ship);
			result[ship] = price;
		}
		return result;
	}
}