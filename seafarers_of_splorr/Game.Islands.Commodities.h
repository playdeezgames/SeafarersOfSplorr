#pragma once
#include "Common.XY.h"
#include "Game.Commodity.h"
namespace game::islands::Commodities
{
	double GetPurchasePrice(const common::XY<double>&, const game::Commodity&);
	double GetSalePrice(const common::XY<double>&, const game::Commodity&);
}