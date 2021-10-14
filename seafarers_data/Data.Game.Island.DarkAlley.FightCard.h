#pragma once
#include <Common.XY.h>
#include <optional>
#include <map>
namespace data::game::island::dark_alley//20211014
{
	struct FightCard
	{
		int rank;
		int suit;
		int adjacent;
		bool shown;

		static std::optional<FightCard> Read(const common::XY<double>&, size_t);
		static std::map<size_t, FightCard> Read(const common::XY<double>&);
		static void Write(const common::XY<double>&, size_t, const FightCard&);
		static void Clear(const common::XY<double>&);
	};
}
