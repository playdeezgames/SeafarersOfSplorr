#pragma once
#include <Common.XY.h>
#include "Game.Item.h"
#include <map>
namespace game::islands//20211011
{
	struct Items
	{
		static std::map<game::Item, double> GetPurchasePrices(const common::XY<double>&);
		static std::map<game::Item, double> GetSalePrices(const common::XY<double>&);
	};
}