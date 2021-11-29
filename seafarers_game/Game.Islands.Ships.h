#pragma once
#include "Game.ShipType.h"
#include <map>
namespace game::islands
{
	struct Ships
	{
		static std::map<game::ShipType, double> GetPurchasePrices(int);
		static std::map<game::ShipType, double> GetSalePrices(int);
		static double GetSalePrice(int, const game::ShipType&);
	};
}