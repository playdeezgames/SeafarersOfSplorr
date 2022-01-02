#include <cmath>
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
}