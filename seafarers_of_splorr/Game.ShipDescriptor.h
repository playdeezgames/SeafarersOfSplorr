#pragma once
#include <string>
#include "Game.Commodity.h"
#include <map>
#include "Game.Ship.Statistic.h"
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Statistic, double> statistics;
		std::map<game::Commodity, double> commodities;
		size_t initialShipGenerationWeight;
	};
}