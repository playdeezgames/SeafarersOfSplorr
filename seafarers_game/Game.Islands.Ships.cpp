#include <Common.Utility.List.h>
#include "Game.Islands.Commodities.h"
#include "Game.Islands.Ships.h"
#include "Game.ShipTypes.h"
namespace game::islands
{
	static double GetPurchasePrice(
		int islandId,
		const game::ShipType& ship)
	{
		auto& commodities = game::ShipTypes::GetCommodities(ship);
		return Commodities::GetPurchasePrice(islandId, commodities);
	}

	double Ships::GetSalePrice(
		int islandId,
		const game::ShipType& ship)
	{
		auto& commodities = game::ShipTypes::GetCommodities(ship);
		return Commodities::GetSalePrice(islandId, commodities);
	}

	static std::map<game::ShipType, double> GetPrices(
		int islandId, 
		std::function<double(int, const ShipType&)> pricer)
	{
		std::map<game::ShipType, double> result;
		common::utility::List::Iterate<ShipType>(ShipTypes::All, [&result, islandId, pricer](const ShipType& ship)
			{
				result[ship] = pricer(islandId, ship);
			});
		return result;
	}

	std::map<game::ShipType, double> Ships::GetPurchasePrices(int islandId)
	{
		return GetPrices(islandId, GetPurchasePrice);
	}

	std::map<game::ShipType, double> Ships::GetSalePrices(int islandId)
	{
		return GetPrices(islandId, GetSalePrice);
	}
}