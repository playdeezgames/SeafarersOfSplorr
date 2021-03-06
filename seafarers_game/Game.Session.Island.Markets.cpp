#include "Game.Session.Island.Markets.h"
#include "Game.Session.Commodities.h"
#include "Game.Session.World.h"
#include <algorithm>
#include <cmath>
#include <Common.RNG.h>
#include <Data.Game.Island.Market.h>
#include <numeric>
#include "Game.Session.Item.Type.Commodities.h"
namespace game::session::island
{
	double Markets::GetUnitPurchaseValue(int itemSubtype) const
	{
		auto itemCommodities = item::type::Commodities(itemSubtype).GetAll();
		return std::accumulate(
				itemCommodities.begin(),
				itemCommodities.end(),
				0.0,
				[this](double accumulator, const auto& itemCommodity)
				{
					auto market = GetMarket(itemCommodity);
					auto unitPrice = market.GetUnitPurchaseValue();
					auto amount = itemCommodity.GetAmount();
					return accumulator + unitPrice * amount;
				});
	}

	int Markets::GetMaximumPurchaseQuantity(int itemSubtype, double value) const
	{
		auto unitValue = GetUnitPurchaseValue(itemSubtype);
		return (int)std::floor(value / unitValue);
	}


	double Markets::GetUnitSaleValue(int itemSubtype) const
	{
		auto itemCommodities = item::type::Commodities(itemSubtype).GetAll();
		return std::accumulate(
			itemCommodities.begin(),
			itemCommodities.end(),
			0.0,
			[this](double accumulator, const auto& itemCommodity)
			{
				auto market = GetMarket(itemCommodity);
				return accumulator + market.GetUnitSaleValue() * itemCommodity.GetAmount();
			});
	}

	int Markets::GetMaximumSaleQuantity(int itemSubtype, double value) const
	{
		auto unitValue = GetUnitSaleValue(itemSubtype);
		return (int)std::ceil(value / unitValue);
	}

	static const std::map<double, size_t> supplyDemandGenerator =
	{
		{  3.0,  1 },
		{  4.0,  3 },
		{  5.0,  6 },
		{  6.0, 10 },
		{  7.0, 15 },
		{  8.0, 21 },
		{  9.0, 25 },
		{ 10.0, 27 },
		{ 11.0, 27 },
		{ 12.0, 25 },
		{ 13.0, 21 },
		{ 14.0, 15 },
		{ 15.0, 10 },
		{ 16.0,  6 },
		{ 17.0,  3 },
		{ 18.0,  1 }
	};

	void Markets::Populate(const game::Difficulty&) const
	{
		const int INITIAL_PURCHASES = 0;
		const int INITIAL_SALES = 0;
		auto commodities = game::session::Commodities().GetAll();
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

	void Markets::ApplyTurnEffects() const
	{

	}
}