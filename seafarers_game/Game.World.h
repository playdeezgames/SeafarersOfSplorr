#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game
{
	struct World
	{
		static double GetUnfoulingLaborMultiplier();
		static double GetReputationReward();
		static double GetReputationPenalty();
	};
}
