#pragma once
#include "Game.Ship.h"
#include <map>
#include "Common.XY.h"
namespace game::islands::Ships
{
	std::map<game::Ship, double> GetPurchasePrices(const common::XY<double>&);
	std::map<game::Ship, double> GetSalePrices(const common::XY<double>&);
	double GetShipSalePrice(const common::XY<double>&, const game::Ship&);
}