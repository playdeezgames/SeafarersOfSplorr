#pragma once
#include "Game.Difficulty.h"
namespace game::avatar
{
	enum class Statistic
	{
		TURNS_REMAINING,
		HEALTH,
		SATIETY,
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

	double GetMoney();

	double GetHealth();
	double GetSatiety();
	void Eat(double);
	bool NeedToEat(double);

	double GetReputation();
	void ChangeReputation(double);

	int GetTurnsRemaining();
	void SpendTurn();
	bool IsOutOfTurns();
}
