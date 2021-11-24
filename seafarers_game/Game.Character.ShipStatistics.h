#pragma once
#include "Game.Side.h"
namespace game::character
{
	struct ShipStatistics
	{
		static double GetFouling();
		static double GetFoulingPercentage();
		static double GetFoulingPercentage(const game::Side&);
		static void IncreaseFouling(double);
		static void CleanHull(const Side&);
	};
}