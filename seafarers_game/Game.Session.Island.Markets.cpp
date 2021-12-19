#include "Game.Session.Island.Markets.h"
#include "Game.Session.Commodities.h"
namespace game::session::island
{
	double Markets::GetPurchaseValue(const ItemSubtype& itemSubtype) const
	{
		auto itemCommodities = itemSubtype.GetCommodities().GetAll();
		double totalValue = 0.0;
		for (auto itemCommodity : itemCommodities)
		{
			auto market = GetMarket(itemCommodity);
			totalValue += market.GetUnitPurchaseValue() * itemCommodity.GetAmount();
		}
		return totalValue;
	}

	double Markets::GetSaleValue(const ItemSubtype& itemSubtype) const
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
}