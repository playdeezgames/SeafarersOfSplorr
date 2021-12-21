#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game::characters
{
	namespace statistics
	{
		struct Brawling
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
		};
	}
	struct Statistics
	{
		static void Reset(int, const game::Difficulty&);
	};
}
