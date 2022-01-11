#include <algorithm>
#include <cmath>
#include <Common.RNG.h>
#include <Data.Game.Island.Market.h>
#include "Game.Session.h"
#include "Game.Session.Island.Markets.h"
#include "Game.Session.Commodities.h"
#include <numeric>
namespace game::session::island
{
	double Markets::GetUnitPurchaseValue(const item::Type& itemSubtype) const
	{
		auto itemCommodities = itemSubtype.GetCommodities().GetAll();
		return std::accumulate(
				itemCommodities.begin(),
				itemCommodities.end(),
				0.0,
				[this](double accumulator, const game::session::item::type::Commodity& itemCommodity)
				{
					auto market = GetMarket(itemCommodity);
					auto unitPrice = market.GetUnitPurchaseValue();
					auto amount = itemCommodity.GetAmount();
					return accumulator + unitPrice * amount;
				});
	}

	int Markets::GetMaximumPurchaseQuantity(const item::Type& itemSubtype, double value) const
	{
		auto unitValue = GetUnitPurchaseValue(itemSubtype);
		return (int)std::floor(value / unitValue);
	}


	double Markets::GetUnitSaleValue(const item::Type& itemSubtype) const
	{
		auto itemCommodities = itemSubtype.GetCommodities().GetAll();
		return std::accumulate(
			itemCommodities.begin(),
			itemCommodities.end(),
			0.0,
			[this](double accumulator, const game::session::item::type::Commodity& itemCommodity)
			{
				auto market = GetMarket(itemCommodity);
				return accumulator + market.GetUnitSaleValue() * itemCommodity.GetAmount();
			});
	}

	int Markets::GetMaximumSaleQuantity(const item::Type& itemSubtype, double value) const
	{
		auto unitValue = GetUnitSaleValue(itemSubtype);
		return (int)std::ceil(value / unitValue);
	}

	static const std::map<double, size_t> supplyDemandGenerator =
	{
		{ 3.0,1},
		{ 4.0,3},
		{ 5.0,6},
		{ 6.0,10},
		{ 7.0,15},
		{ 8.0,21},
		{ 9.0,25},
		{10.0,27},
		{11.0,27},
		{12.0,25},
		{13.0,21},
		{14.0,15},
		{15.0,10},
		{16.0,6},
		{17.0,3},
		{18.0,1}
	};

	void Markets::Populate(const game::Difficulty& difficulty) const
	{
		const int INITIAL_PURCHASES = 0;
		const int INITIAL_SALES = 0;
		auto commodities = game::Session().GetWorld().GetCommodities().GetAll();
		std::for_each(
			commodities.begin(),
			commodities.end(),
			[this](const game::session::Commodity& commodity)
			{
				data::game::island::Market::Data data =
				{
					common::RNG::FromGenerator(supplyDemandGenerator),
					common::RNG::FromGenerator(supplyDemandGenerator),
					INITIAL_PURCHASES,
					INITIAL_SALES
				};
				data::game::island::Market::Write(islandId, (int)commodity.operator game::Commodity(), data);
			});
	}
}