#include <Data.Game.Island.ItemLegacy.h>
#include <functional>
#include "Game.Islands.Commodities.h"
#include "Game.Islands.Items.h"
#include "Game.Items.h"
namespace game::islands
{
	static double GetItemPurchasePrice(
		int islandId,
		const game::Item& item)
	{
		return Commodities::GetPurchasePrice(islandId, game::Items::GetCommodities(item));
	}

	static double GetItemSellPrice(
		int islandId,
		const game::Item& item)
	{
		return Commodities::GetSalePrice(islandId, game::Items::GetCommodities(item));
	}

	static std::map<game::Item, double> GetPrices(
		int islandId, 
		std::function<double(int, const Item&)> unitPricer)
	{
		std::map<game::Item, double> result;
		auto itemsAvailable = data::game::island::ItemLegacy::GetAll(islandId);
		for (auto& item : itemsAvailable)
		{
			double price = unitPricer(islandId, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}

	std::map<game::Item, double> Items::GetPurchasePrices(int islandId)
	{
		return GetPrices(islandId, GetItemPurchasePrice);
	}

	std::map<game::Item, double> Items::GetSalePrices(int islandId)
	{
		return GetPrices(islandId, GetItemSellPrice);
	}
}