#pragma once
#include "Game.Item.h"
#include <map>
#include "Common.XY.h"
namespace game::islands::Items
{
	std::map<game::Item, double> GetPurchasePrices(const common::XY<double>&);
	std::map<game::Item, double> GetSalePrices(const common::XY<double>&);
}