#pragma once
#include <string>
#include "Game.Commodity.h"
#include <map>
#include "Game.Ship.Property.h"
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