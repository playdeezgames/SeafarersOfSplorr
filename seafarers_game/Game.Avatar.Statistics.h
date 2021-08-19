#pragma once
#include "Game.Difficulty.h"
#include <string>
namespace game::avatar::Statistics
{
	const std::string FORMAT_HEALTH = "\x83 {:.0f}";
	const std::string FORMAT_BRAWLING = "\x88 {:.1f}";

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
	void ChangeInfamy(double);

	double GetBrawling();
	void ChangeBrawling(double);
}
