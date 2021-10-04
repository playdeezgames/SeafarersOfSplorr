#pragma once
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
namespace game::avatar
{
	struct Ship
	{
		static void Reset(const game::Difficulty&);
		static void Write(const game::ShipType&);
		static game::ShipType Read();
		static double AvailableTonnage();
	};
}
