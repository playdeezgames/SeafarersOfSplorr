#include "Data.Game.Island.Item.h"
#include "Data.Game.Island.Market.h"
#include "Game.Commodities.h"
#include "Game.Islands.Items.h"
#include "Game.Items.h"
namespace game::islands::Items
{
	static double GetCommodityUnitPurchasePrice(
		const game::CommodityDescriptor& commodityDescriptor,
		const data::game::island::Market::MarketData& market)
	{
		return commodityDescriptor.basePrice *
			(market.demand + (double)market.purchases * commodityDescriptor.demandFactor) /
			(market.supply + (double)market.sales * commodityDescriptor.supplyFactor);
	}

	static double GetCommodityUnitSellPrice(
		const game::CommodityDescriptor& commodityDescriptor,
		const data::game::island::Market::MarketData& market)
	{
		return commodityDescriptor.basePrice *
			(market.demand + (double)market.purchases * commodityDescriptor.demandFactor) /
			(market.supply + (double)market.sales * commodityDescriptor.supplyFactor) *
			(1.0 - commodityDescriptor.discount);
	}

	static double GetItemPurchasePrice(
		const game::Item& item,
		const std::map<int, data::game::island::Market::MarketData>& markets)
	{
		double price = 0.0;
		auto& itemDescriptor = game::Items::Read(item);
		for (auto itemCommodity : itemDescriptor.commodities)
		{
			auto commodity = itemCommodity.first;
			auto commodityAmount = itemCommodity.second;
			auto& commodityDescriptor = game::Commodities::Read(commodity);
			auto market = markets.find((int)commodity)->second;
			price +=
				commodityAmount * GetCommodityUnitPurchasePrice(commodityDescriptor, market);
		}
		return price;
	}

	static double GetItemSellPrice(
		const game::Item& item,
		const std::map<int, data::game::island::Market::MarketData>& markets)
	{
		double price = 0.0;
		auto& itemDescriptor = game::Items::Read(item);
		for (auto itemCommodity : itemDescriptor.commodities)
		{
			auto commodity = itemCommodity.first;
			auto commodityAmount = itemCommodity.second;
			auto& commodityDescriptor = game::Commodities::Read(commodity);
			auto market = markets.find((int)commodity)->second;
			price +=
				commodityAmount * GetCommodityUnitSellPrice(commodityDescriptor, market);
		}
		return price;
	}

	std::map<game::Item, double> GetPurchasePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		const auto markets = data::game::island::Market::All(location);
		auto itemsAvailable = data::game::island::Item::GetAll(location);
		for (auto& item : itemsAvailable)
		{
			double price = GetItemPurchasePrice((game::Item)item, markets);
			result[(game::Item)item] = price;
		}
		return result;
	}

	std::map<game::Item, double> GetSalePrices(const common::XY<double>& location)
	{
		std::map<game::Item, double> result;
		const auto markets = data::game::island::Market::All(location);
		auto items = game::Items::All();
		for (auto& item : items)
		{
			double price = GetItemSellPrice((game::Item)item, markets);
			result[(game::Item)item] = price;
		}
		return result;
	}
}