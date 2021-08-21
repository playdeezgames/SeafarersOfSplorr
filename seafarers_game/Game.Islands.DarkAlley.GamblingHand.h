#pragma once
#include <Cards.Card.h>
#include <Common.XY.h>
#include <vector>
namespace game::islands::dark_alley::GamblingHand
{
	bool Deal(const common::XY<double>&);
	std::vector<cards::Card> Read(const common::XY<double>&);
	bool IsWinner(const common::XY<double>&);
}