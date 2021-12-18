#include <Common.RNG.h>
#include <Data.Game.Island.Tavern.h>
#include <format>
#include "Game.Characters.Islands.h"
#include "Game.Islands.Features.Tavern.h"
#include "Game.Session.h"
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

	static void AddTavernCharacter(int islandId)
	{
		auto character = game::Session().GetCharacters().Create(game::characters::State::TAVERN);
		character.SetIsland(game::Session().GetIslands().GetIsland(islandId));
	}

	static const std::map<int, size_t> npcCountGenerator = 
	{
		{1, 1},
		{2, 2},
		{3, 3},
		{4, 2},
		{5, 1}
	};

	static void InitializeTavernCharacters(int islandId)
	{
		auto characterCount = common::RNG::FromGenerator(npcCountGenerator);
		while (characterCount > 0)
		{
			AddTavernCharacter(islandId);
			characterCount--;
		}
	}

	void Tavern::Initialize(int islandId)
	{
		InitializeTavernName(islandId);
		InitializeTavernCharacters(islandId);
	}
}