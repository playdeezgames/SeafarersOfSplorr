#pragma once
#include "Game.Commodity.h"
#include "Game.Ship.Property.h"
#include <map>
#include <string>
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Property, double> properties;
		std::map<game::Commodity, double> commodities;
		size_t initialShipGenerationWeight;
	};
}