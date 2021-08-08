#pragma once
#include "Game.Ship.h"
#include <map>
#include "Common.XY.h"
namespace game::islands::Ships
{
	std::map<game::Ship, double> GetPurchasePrices(const common::XY<double>&);
	std::map<game::Ship, double> GetSalePrices(const common::XY<double>&);
}