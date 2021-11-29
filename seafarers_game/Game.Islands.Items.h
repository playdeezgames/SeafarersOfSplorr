#pragma once
#include "Game.Item.h"
#include <map>
namespace game::islands
{
	struct Items
	{
		static std::map<game::Item, double> GetPurchasePrices(int);
		static std::map<game::Item, double> GetSalePrices(int);
	};
}