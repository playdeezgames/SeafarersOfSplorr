#pragma once
#include "Game.Avatar.Statistic.h"
#include "Game.Difficulty.h"
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&);
	double GetCurrent(const game::avatar::Statistic&);
	double ChangeCurrent(const game::avatar::Statistic&, double);

	double GetMoney();

	double GetHealth();
	bool IsDead();

	double GetSatiety();
	bool IsStarving();
	void Eat(double);
	bool NeedToEat(double);

	double GetReputation();
	void ChangeReputation(double);

	int GetTurnsRemaining();
	void SpendTurn();
	bool IsOutOfTurns();
}
