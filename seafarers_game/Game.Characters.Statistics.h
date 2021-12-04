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
		struct Reputation
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
		};
		struct Infamy
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
		};
		struct Brawling
		{
			static std::optional<double> Current(int);
			static std::optional<double> Change(int, double);
		};
		struct Turns
		{
			static std::optional<int> Remaining(int);
			static std::optional<int> Change(int, int);
			static std::optional<bool> HasRemaining(int);
		};
		struct Dignity
		{
			static std::optional<double> Current(int);
		};
		struct Poshness
		{
			static std::optional<double> Current(int);
		};
	}
	struct Statistics
	{
		static void Reset(int, const game::Difficulty&);
	};
}
