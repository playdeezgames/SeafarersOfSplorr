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
		static double GetWindHeadingLegacy();
		static double GetWindSpeedMultiplierLegacy(double);
		static void SetWindHeadingLegacy(double);
		static double GetUnfoulingLaborMultiplier();
		static double GetReputationReward();
		static double GetReputationPenalty();
		static void Reset(const game::Difficulty&);
		static bool ClampLocation(common::XY<double>&);
	};
}
