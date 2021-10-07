#pragma once
#include "Game.Commodity.h"
#include "Game.Difficulty.h"
#include <map>
#include <optional>
#include <string>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::map<game::Commodity, double> commodities;
		size_t present;
		size_t notPresent;
		double tonnage;
		std::map<Difficulty, size_t> initialInventories;
		std::optional<size_t> avatarRationWeight;
	};
}
