#pragma once
#include "Common.XY.h"
#include "Game.Commodity.h"
#include <map>
namespace game::islands::Commodities
{
	double GetPurchasePrice(const common::XY<double>&, const std::map<Commodity, double>&);
	double GetSalePrice(const common::XY<double>&, const std::map<Commodity, double>&);
}