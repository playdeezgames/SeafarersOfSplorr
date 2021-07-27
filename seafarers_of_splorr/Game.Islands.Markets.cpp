#include "Data.Game.Island.Market.h"
#include "Game.Islands.Markets.h"
namespace game::islands::Markets
{
	void BuyItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto data = data::game::island::Market::Read(location, (int)item);
		if (data)
		{
			auto market = data.value();
			market.purchases+=(int)quantity;
			data::game::island::Market::Write(location, (int)item, market);
		}
	}

	void SellItems(const common::XY<double>& location, const game::Item& item, size_t quantity)
	{
		auto data = data::game::island::Market::Read(location, (int)item);
		if (data)
		{
			auto market = data.value();
			market.sales += (int)quantity;
			data::game::island::Market::Write(location, (int)item, market);
		}
	}
}