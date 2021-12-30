#pragma once
#include "Game.Commodity.h"
#include "Game.ShipType.h"
#include <list>
#include <map>
#include <optional>
#include <string>
namespace game
{
	struct ShipTypes
	{
		static double GetSpeedFactor(const game::ShipType&);
		static const std::map<game::Commodity, double> GetCommodities(const game::ShipType&);
	};
}
