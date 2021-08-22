#pragma once
#include <Cards.Card.h>
#include <Common.XY.h>
#include <map>
#include <optional>
namespace game::islands::DarkAlley
{
	struct FightCard
	{
		cards::Card card;
		int adjacent;
		bool shown;
		bool success;
	};
}
namespace game::islands::dark_alley::FightCards
{
	void Generate(const common::XY<double>&);
	std::map<size_t, DarkAlley::FightCard> Read(const common::XY<double>&);
	std::optional<DarkAlley::FightCard> Pick(const common::XY<double>&, size_t);
}