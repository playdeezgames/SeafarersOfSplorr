#pragma once
#include <Common.XY.h>
#include "Game.Ship.h"
#include <map>
namespace game::islands::Ships
{
	std::map<game::Ship, double> GetPurchasePrices(const common::XY<double>&);
	std::map<game::Ship, double> GetSalePrices(const common::XY<double>&);
	double GetSalePrice(const common::XY<double>&, const game::Ship&);
}