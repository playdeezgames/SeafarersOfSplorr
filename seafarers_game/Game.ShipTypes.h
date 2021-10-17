#pragma once
#include "Game.Commodity.h"
#include "Game.ShipType.h"
#include "Game.ShipStatistic.h"
#include <list>
#include <map>
#include <optional>
#include <string>
namespace game//20211013
{
	struct ShipTypes
	{
		static const std::list<game::ShipType>& All();
		static game::ShipType GenerateForAvatar();
		static double GetTotalTonnage(const game::ShipType&);
		static const std::string& GetName(const game::ShipType&);
		static double GetSpeedFactor(const game::ShipType&);
		static const std::map<game::Commodity, double> GetCommodities(const game::ShipType&);
		static std::list<ShipStatistic> GetStatistics(const game::ShipType&);
		static std::optional<double> GetMinimumStatistic(const game::ShipType&, const game::ShipStatistic&);
		static std::optional<double> GetMaximumStatistic(const game::ShipType&, const game::ShipStatistic&);
		static double GetInitialStatistic(const game::ShipType&, const game::ShipStatistic&);
	};
}
