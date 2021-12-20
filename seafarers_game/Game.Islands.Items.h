#pragma once
#include "Game.ItemLegacy.h"
#include <map>
namespace game::islands
{
	struct Items
	{
		static std::map<game::Item, double> GetPurchasePrices(int);
		static std::map<game::Item, double> GetSalePrices(int);
	};
}