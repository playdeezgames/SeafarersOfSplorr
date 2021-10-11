#pragma once
#include <Common.XY.h>
#include "Game.Commodity.h"
#include <map>
namespace game::islands//20211011
{
	struct Commodities
	{
		static double GetPurchasePrice(const common::XY<double>&, const std::map<Commodity, double>&);
		static double GetSalePrice(const common::XY<double>&, const std::map<Commodity, double>&);
	};
}