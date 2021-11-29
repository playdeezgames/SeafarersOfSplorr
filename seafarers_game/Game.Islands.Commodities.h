#pragma once
#include <Common.XY.h>
#include "Game.Commodity.h"
#include <map>
namespace game::islands
{
	struct Commodities
	{
		static double GetPurchasePrice(int, const std::map<Commodity, double>&);
		static double GetSalePrice(int, const std::map<Commodity, double>&);
	};
}