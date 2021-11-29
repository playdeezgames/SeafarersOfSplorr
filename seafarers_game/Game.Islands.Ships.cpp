#include <Common.Utility.h>
#include <Common.Utility.List.h>
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

	static std::map<game::ShipType, double> GetPrices(
		const common::XY<double>& location, 
		std::function<double(const common::XY<double>&, const ShipType&)> pricer)
	{
		std::map<game::ShipType, double> result;
		common::utility::List::Iterate<ShipType>(ShipTypes::All, [&result, location, pricer](const ShipType& ship)
			{
				result[ship] = pricer(location, ship);
			});
		return result;
	}

	std::map<game::ShipType, double> Ships::GetPurchasePrices(const common::XY<double>& location)
	{
		return GetPrices(location, GetPurchasePrice);
	}

	std::map<game::ShipType, double> Ships::GetSalePrices(const common::XY<double>& location)
	{
		return GetPrices(location, GetSalePrice);
	}
}