#pragma once
#include <optional>
#include <map>
namespace data::game::island::dark_alley
{
	struct FightCard
	{
		int rank;
		int suit;
		int adjacent;
		bool shown;

		static std::optional<FightCard> Read(int, size_t);
		static std::map<size_t, FightCard> Read(int);
		static void Write(int, size_t, const FightCard&);
		static void Clear(int);
	};
}
