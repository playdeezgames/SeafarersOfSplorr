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
		static bool IsDead(int);
		static double GetSatiety(int);
		static void ChangeSatiety(int, double);
		static bool IsStarving(int);
		static void Eat(int, double);
		static bool NeedToEat(int, double);
		static double GetReputation(int);
		static void ChangeReputation(int, double);
		static int GetTurnsRemaining(int);
		static void SpendTurn(int);
		static bool IsOutOfTurns(int);
		static double GetInfamy(int);
		static void ChangeInfamy(int, double);
		static double GetBrawling(int);
		static void ChangeBrawling(int, double);
		static double GetDignity(int);
		static double GetPoshness(int);
		static bool IsPlayerDead();
		static bool IsPlayerOutOfTurns();
	};
}
