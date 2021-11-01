#pragma once
#include "Game.Difficulty.h"
namespace game::avatar//20211017
{
	struct Statistics
	{
		static void Reset(const game::Difficulty&);
		static double ReadMoney(int);
		static void ChangeMoney(int, double);
		static double GetHealth(int);
		static void ChangeHealth(int, double);
		static bool IsDead();
		static double GetSatiety();
		static void ChangeSatiety(double);
		static bool IsStarving();
		static void Eat(double);
		static bool NeedToEat(double);
		static double GetReputation();
		static void ChangeReputation(double);
		static int GetTurnsRemaining();
		static void SpendTurn();
		static bool IsOutOfTurns();
		static double GetInfamy();
		static void ChangeInfamy(double);
		static double GetBrawling();
		static void ChangeBrawling(double);
		static double GetDignity();
		static double GetPoshness();
	};
}
