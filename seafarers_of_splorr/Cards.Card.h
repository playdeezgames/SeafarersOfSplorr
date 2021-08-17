#pragma once
#include "Cards.Rank.h"
#include "Cards.Suit.h"
#include <tuple>
namespace cards
{
	typedef std::tuple<Rank, Suit> Card;
}