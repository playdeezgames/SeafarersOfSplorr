#pragma once
#include <Cards.Card.h>
#include <vector>
namespace game::islands::dark_alley
{
	struct GamblingHand
	{
		static bool Deal(int);
		static std::vector<cards::Card> Read(int);
		static bool IsWinner(int);
	};
}