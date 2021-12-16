#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game
{
	struct World
	{
		static int GetVersion();

		static double GetMinimumIslandDistance();
		static double GetViewDistance();
		static double GetDockDistance();

		static double GetUnfoulingLaborMultiplier();
		static double GetReputationReward();
		static double GetReputationPenalty();
	};
}
