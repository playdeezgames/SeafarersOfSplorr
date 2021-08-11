#pragma once
#include "Game.Difficulty.h"
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&);

	double GetMoney();
	void ChangeMoney(double);

	double GetHealth();
	void ChangeHealth(double);
	bool IsDead();

	double GetSatiety();
	void ChangeSatiety(double);
	bool IsStarving();
	void Eat(double);
	bool NeedToEat(double);

	double GetReputation();
	void ChangeReputation(double);

	int GetTurnsRemaining();
	void SpendTurn();
	bool IsOutOfTurns();

	double GetInfamy();
	double GetBrawling();
}
