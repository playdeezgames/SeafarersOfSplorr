#pragma once
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
namespace game::avatar
{
	struct Ship
	{
		static void Reset(const game::Difficulty&);
		static void Write(int);
		static std::optional<int> Read();
		static std::optional<double> AvailableTonnage();
	};
}
