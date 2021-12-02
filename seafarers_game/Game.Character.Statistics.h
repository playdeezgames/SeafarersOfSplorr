#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game::character
{
	namespace statistics
	{
		struct Money
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
		};
		struct Health
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
			static std::optional<bool> IsDead(int);
		};
		struct Satiety
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
			static std::optional<bool> IsStarving(int);
			static void Eat(int, double);
			static std::optional<double> NeedsToEat(int, double);
		};
	}
	struct Statistics
	{
		static void Reset(int, const game::Difficulty&);
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
		static bool IsPlayerOutOfTurns(int);
	};
}
