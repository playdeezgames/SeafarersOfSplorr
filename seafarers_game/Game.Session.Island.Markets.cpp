#include <cmath>
#include "Game.Session.Island.Markets.h"
#include "Game.Session.Commodities.h"
namespace game::session::island
{
	double Markets::GetPurchaseValue(const item::Type& itemSubtype) const
	{
		auto itemCommodities = itemSubtype.GetCommodities().GetAll();
		double totalValue = 0.0;
		for (auto itemCommodity : itemCommodities)
		{
			auto market = GetMarket(itemCommodity);
			auto unitPrice = market.GetUnitPurchaseValue();
			auto amount = itemCommodity.GetAmount();
			totalValue += unitPrice * amount;
		}
		return totalValue;
	}

	int Markets::GetPurchaseQuantity(const item::Type& itemSubtype, double value) const
	{
		auto unitValue = GetPurchaseValue(itemSubtype);
		return (int)std::floor(value / unitValue);
	}


	double Markets::GetSaleValue(const item::Type& itemSubtype) const
	{
		auto itemCommodities = itemSubtype.GetCommodities().GetAll();
		double totalValue = 0.0;
		for (auto itemCommodity : itemCommodities)
		{
			auto market = GetMarket(itemCommodity);
			totalValue += market.GetUnitSaleValue() * itemCommodity.GetAmount();
		}
		return totalValue;
	}

	int Markets::GetSaleQuantity(const item::Type& itemSubtype, double value) const
	{
		auto unitValue = GetSaleValue(itemSubtype);
		return (int)std::ceil(value / unitValue);
	}
}