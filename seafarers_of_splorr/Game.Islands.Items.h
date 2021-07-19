#pragma once
#include "Game.Item.h"
#include <map>
#include "Common.XY.h"
namespace game::islands::Items
{
	std::map<game::Item, double> GetPrices(const common::XY<double>&);
}