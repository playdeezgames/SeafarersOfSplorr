#pragma once
#include "Game.Commodity.h"
#include "Game.Ship.h"
#include "Game.ShipStatistic.h"
#include "Game.ShipStatisticDescriptor.h"
#include <list>
#include <string>
namespace game
{
	struct Ships//TODO: rename to shiptypes
	{
		static const std::list<game::ShipType>& All();
		static game::ShipType GenerateForAvatar();
		static double GetTotalTonnage(const game::ShipType&);
		static const std::string& GetName(const game::ShipType&);
		static double GetSpeedFactor(const game::ShipType&);
		static const std::map<game::Commodity, double> GetCommodities(const game::ShipType&);
		static const std::map<ShipStatistic, ShipStatisticDescriptor>& GetStatistics(const game::ShipType&);
	};
}
