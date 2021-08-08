#pragma once
#include "Common.XY.h"
#include "Game.Commodity.h"
namespace game::islands::Commodities
{
	double GetCommodityUnitPurchasePrice(const common::XY<double>&, const game::Commodity&);
	double GetCommodityUnitSalePrice(const common::XY<double>&, const game::Commodity&);
}