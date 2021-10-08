#pragma once
#include "Game.Commodity.h"
#include <list>
namespace game
{
	struct Commodities
	{
		static const std::list<game::Commodity>& All();
		static double GetBasePrice(const game::Commodity&);
		static double GetDemandFactor(const game::Commodity&);
		static double GetSupplyFactor(const game::Commodity&);
		static double GetDiscount(const game::Commodity&);
	};
}
