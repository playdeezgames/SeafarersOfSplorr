#pragma once
#include "Game.Side.h"
namespace game::ship
{
	struct Statistics
	{
		static double GetFouling(int);
		static double GetFoulingPercentage(int);
		static double GetFoulingPercentage(int, const game::Side&);
		static void IncreaseFouling(int, double);
		static void CleanHull(int, const Side&);
	};
}