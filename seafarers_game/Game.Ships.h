#pragma once
#include "Game.Commodity.h"
#include "Game.Ship.h"
#include "Game.ShipStatistic.h"
#include "Game.ShipStatisticDescriptor.h"
#include <list>
#include <string>
namespace game
{
	struct Ships
	{
		static const std::list<game::Ship>& All();
		static game::Ship GenerateForAvatar();
		static double GetTotalTonnage(const game::Ship&);
		static const std::string& GetName(const game::Ship&);
		static double GetSpeedFactor(const game::Ship&);
		static const std::map<game::Commodity, double> GetCommodities(const game::Ship&);
		static const std::map<ShipStatistic, ShipStatisticDescriptor>& GetStatistics(const game::Ship&);
	};
}
