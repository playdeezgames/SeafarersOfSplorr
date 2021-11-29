#include <Data.Game.Island.h>
#include <Data.Game.Island.Item.h>
#include <functional>
#include "Game.Islands.Commodities.h"
#include "Game.Islands.Items.h"
#include "Game.Items.h"
namespace game::islands//20211014
{
	static double GetItemPurchasePrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		return Commodities::GetPurchasePrice(data::game::Island::Find(location).value(), game::Items::GetCommodities(item));
	}

	static double GetItemSellPrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		return Commodities::GetSalePrice(data::game::Island::Find(location).value(), game::Items::GetCommodities(item));
	}

	static std::map<game::Item, double> GetPrices(const common::XY<double>& location, std::function<double(const common::XY<double>&, const Item&)> unitPricer)
	{
		std::map<game::Item, double> result;
		auto itemsAvailable = data::game::island::Item::GetAll(data::game::Island::Find(location).value());
		for (auto& item : itemsAvailable)
		{
			double price = unitPricer(location, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}

	std::map<game::Item, double> Items::GetPurchasePrices(const common::XY<double>& location)
	{
		return GetPrices(location, GetItemPurchasePrice);
	}

	std::map<game::Item, double> Items::GetSalePrices(const common::XY<double>& location)
	{
		return GetPrices(location, GetItemSellPrice);
	}
}