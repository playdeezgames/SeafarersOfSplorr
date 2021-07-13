#pragma once
#include "Game.Difficulty.h"
namespace game::avatar
{
	enum class Statistic
	{
		TURNS_REMAINING,
		HEALTH,
		HUNGER,
		MONEY,
		REPUTATION
	};
}
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&);
	double GetMaximum(const game::avatar::Statistic&);
	double GetMinimum(const game::avatar::Statistic&);
	double GetCurrent(const game::avatar::Statistic&);
	double ChangeCurrent(const game::avatar::Statistic&, double);
}
