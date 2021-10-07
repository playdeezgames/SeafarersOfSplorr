#include <Common.RNG.h>
#include <format>
#include "Game.ShipNames.h"
#include <map>
namespace game
{
	static const std::map<std::string, size_t> shipNameAdjectives =
	{
		{"Delicious",1},
		{"Voluptuous",1},
		{"Sturdy",1},
		{"Grumpy",1},
		{"Hard",1},
		{"Slimy",1},
		{"Boaty",1},
		{"Smelly",1},
		{"Focus on",1},
		{"Huge",1},
		{"Flogging",1}
	};

	static const std::map<std::string, size_t> shipNameNouns =
	{
		{"Booty",1},
		{"Deliverance",1},
		{"Tosser",1},
		{"Wumpus",1},
		{"Wood",1},
		{"McBoatface",1},
		{"McGrumpyface",1},
		{"Cat",1},
		{"Happiness",1},
		{"Tracks of Land",1},
		{"Bishop",1}
	};

	std::string ShipNames::Generate()
	{
		auto adjective = common::RNG::FromGenerator(shipNameAdjectives, std::string());
		auto noun = common::RNG::FromGenerator(shipNameNouns, std::string());
		return std::format("{} {}", adjective, noun);
	}

}