#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island::dark_alley::FightCard
{
	struct CardData
	{
		int rank;
		int suit;
		int adjacent;
		bool shown;
	};

	std::optional<CardData> Read(const common::XY<double>&, size_t);
	void Write(const common::XY<double>&, size_t, const CardData&);
	void Clear(const common::XY<double>&);
}
