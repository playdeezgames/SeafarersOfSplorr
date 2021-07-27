#pragma once
#include <string>
#include "Game.Commodity.h"
#include <map>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::map<game::Commodity, double> commodities;
		size_t present;
		size_t notPresent;
		double tonnage;
	};
}
