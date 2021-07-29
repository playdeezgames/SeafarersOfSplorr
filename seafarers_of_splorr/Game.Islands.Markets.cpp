#include "Data.Game.Island.Market.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
namespace game::islands::Markets
{
	void BuyItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto descriptor = game::Items::Read(item);
		for (auto entry : descriptor.commodities)
		{
			auto data = data::game::island::Market::Read(location, (int)entry.first);
			if (data)
			{
				auto market = data.value();
				market.purchases+=(double)quantity * entry.second;
				data::game::island::Market::Write(location, (int)entry.first, market);
			}
		}
	}

	void SellItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto descriptor = game::Items::Read(item);
		for (auto entry : descriptor.commodities)
		{
			auto data = data::game::island::Market::Read(location, (int)entry.first);
			if (data)
			{
				auto market = data.value();
				market.sales += (double)quantity * entry.second;
				data::game::island::Market::Write(location, (int)entry.first, market);
			}
		}
	}
}