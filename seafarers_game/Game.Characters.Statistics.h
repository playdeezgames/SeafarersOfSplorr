#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game::characters
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
	}
	struct Statistics
	{
		static void Reset(int, const game::Difficulty&);
	};
}
