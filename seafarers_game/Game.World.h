#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game
{
	struct World
	{
		static common::XY<double> GetSize();
		static int GetVersion();
		static double GetMinimumIslandDistance();
		static double GetViewDistance();
		static double GetDockDistance();
		static double GetWindHeading();
		static double GetWindSpeedMultiplier(double);
		static double GetUnfoulingLaborMultiplier();
		static void SetWindHeading(double);
		static void Reset(const game::Difficulty&);
	};
}
