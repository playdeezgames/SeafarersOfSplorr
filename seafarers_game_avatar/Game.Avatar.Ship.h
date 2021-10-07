#pragma once
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
namespace game::avatar
{
	struct Ship
	{
		int shipId;

		static void Reset(const game::Difficulty&);
		static void Write(int);
		static std::optional<Ship> Read();
		static std::optional<double> AvailableTonnage();
	};
}
