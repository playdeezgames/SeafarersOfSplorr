#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game//20211013
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
		static void ApplyTurnEffects();
		static bool ClampLocation(common::XY<double>&);
	};
}
