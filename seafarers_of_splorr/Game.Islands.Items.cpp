#include "Data.Game.Island.Item.h"
//#include "Data.Game.Island.Market.h"
//#include "Game.Commodities.h"
#include "Game.Islands.Commodities.h"
#include "Game.Islands.Items.h"
#include "Game.Items.h"
namespace game::islands::Items
{
	static double GetItemPurchasePrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		double price = 0.0;
		auto& itemDescriptor = game::Items::Read(item);
		for (auto itemCommodity : itemDescriptor.commodities)
		{
			auto commodity = itemCommodity.first;
			auto commodityAmount = itemCommodity.second;
			price +=
				commodityAmount * Commodities::GetPurchasePrice(location, itemCommodity.first);
		}
		return price;
	}

	static double GetItemSellPrice(
		const common::XY<double>& location,
		const game::Item& item)
	{
		double price = 0.0;
		auto& itemDescriptor = game::Items::Read(item);
		for (auto itemCommodity : itemDescriptor.commodities)
		{
			auto commodity = itemCommodity.first;
			auto commodityAmount = itemCommodity.second;
			price +=
				commodityAmount * Commodities::GetSalePrice(location, itemCommodity.first);
		}
		return price;
	}

	std::map<game::Item, double> GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		auto itemsAvailable = data::game::island::Item::GetAll(location);
		for (auto& item : itemsAvailable)
		{
			double price = GetItemPurchasePrice(location, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}

	std::map<game::Item, double> GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		auto items = game::Items::All();
		for (auto& item : items)
		{
			double price = GetItemSellPrice(location, (game::Item)item);
			result[(game::Item)item] = price;
		}
		return result;
	}
}