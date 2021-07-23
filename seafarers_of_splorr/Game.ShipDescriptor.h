#pragma once
#include <string>
#include "Game.Commodity.h"
#include <map>
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		double speedFactor;//TODO: move this to ship statistics
		double tonnage;//TODO: move this to ship statistics
		std::map<game::Commodity, double> commodities;//this is what it takes to build one of these ships
	};
}