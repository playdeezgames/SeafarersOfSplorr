#pragma once
#include <Cards.Card.h>
#include <Common.XY.h>
#include <map>
#include <optional>
namespace game::islands::dark_alley//20211011
{
	struct FightCard
	{
		cards::Card card;
		int adjacentSuccesses;
		bool revealed;
		bool success;

		static void Generate(const common::XY<double>&);
		static std::map<size_t, FightCard> Read(const common::XY<double>&);
		static std::optional<FightCard> Pick(const common::XY<double>&, size_t);
	};
}