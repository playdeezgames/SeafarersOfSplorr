#pragma once
#include <Cards.Rank.h>
#include <Cards.Suit.h>
#include <map>
#include <string>
namespace state::in_play
{
	struct DarkAlleyEntrance
	{
		static const std::map<cards::Rank, std::string> Ranks;
		static const std::map<cards::Suit, std::string> SuitColors;
		static const std::map<cards::Suit, std::string> Suits;
		static void Start();
	};
}
