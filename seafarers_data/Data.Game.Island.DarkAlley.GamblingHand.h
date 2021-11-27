#pragma once
#include <Common.XY.h>
#include <optional>
namespace data::game::island::dark_alley
{
	struct GamblingHand
	{
		int firstCard;
		int secondCard;
		int thirdCard;

		static void Write(int, const GamblingHand&);
		static std::optional<GamblingHand> Read(int);
	};
}
