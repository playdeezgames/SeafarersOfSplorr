#pragma once
#include "Game.Side.h"
namespace game::avatar
{
	struct ShipStatistics
	{
		static double GetFouling();
		static double GetFoulingPercentage();
		static void IncreaseFouling(double);
		static void CleanHull(const Side&);
	};
}