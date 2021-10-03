#pragma once
#include "Game.Ship.h"
#include "Game.ShipDescriptor.h"
#include <list>
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
