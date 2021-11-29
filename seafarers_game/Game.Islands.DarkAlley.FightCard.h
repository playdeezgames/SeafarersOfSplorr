#pragma once
#include <Cards.Card.h>
#include <map>
#include <optional>
namespace game::islands::dark_alley
{
	struct FightCard
	{
		cards::Card card;
		int adjacentSuccesses;
		bool revealed;
		bool success;

		static void Generate(int, int);
		static std::map<size_t, FightCard> Read(int);
		static std::optional<FightCard> Pick(int, size_t);
	};
}