#include <Common.RNG.h>
#include <Data.Game.Island.Tavern.h>
#include <format>
#include "Game.Islands.Features.Tavern.h"
#include <string>
#include <list>
namespace game::islands::features
{
	static const std::list<std::string> adjectives = 
	{
		"prancing",
		"farting",
		"spitting",
		"puking",
		"licking",
		"queefing",
		"hemorrhaging",
		"bulging",
		"upchucking",
		"lubricating",
		"lapdancing",
		"pulsating",
		"undulating"
	};

	static const std::list<std::string> nouns =
	{
		"pony",
		"giraffe",
		"velociraptor",
		"dodo",
		"alligator",
		"sloth",
		"woodchuck",
		"turtle",
		"vampire",
		"ghost",
		"maiden",
		"maseuse",
		"kitten",
		"vixen",
		"mermaid",
		"hobo",
		"wench"
	};

	static void InitializeTavernName(int islandId)
	{
		auto adjective = common::RNG::FromList(adjectives).value();
		auto noun = common::RNG::FromList(nouns).value();
		auto name = std::format("{} {}", adjective, noun);
		data::game::island::Tavern::Write({ islandId, name });
	}

	void Tavern::Initialize(int islandId)
	{
		InitializeTavernName(islandId);
		//TODO: fill the tavern with NPC hirelings
	}
}