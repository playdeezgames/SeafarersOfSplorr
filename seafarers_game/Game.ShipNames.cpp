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

	static const std::string FORMAT_SHIP_NAME = "{} {}";

	static std::vector<std::string> adjectives;
	static std::vector<std::string> nouns;

	const std::vector<std::string>& ShipNames::GetAdjectives()
	{
		if (adjectives.empty())
		{
			for (auto entry : shipNameAdjectives)
			{
				adjectives.push_back(entry.first);
			}
		}
		return adjectives;
	}

	const std::vector<std::string>& ShipNames::GetNouns()
	{
		if (nouns.empty())
		{
			for (auto entry : shipNameNouns)
			{
				nouns.push_back(entry.first);
			}
		}
		return nouns;
	}

	std::string ShipNames::Generate()
	{
		return std::format(
			FORMAT_SHIP_NAME,
			common::RNG::FromGenerator(shipNameAdjectives),
			common::RNG::FromGenerator(shipNameNouns));
	}

}