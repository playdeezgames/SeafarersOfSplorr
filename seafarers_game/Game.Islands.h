#pragma once
#include "Game.Difficulty.h"
#include "Game.Island.h"
#include <list>
#include <optional>
#include <string>
namespace game
{
	struct Islands
	{
		static std::list<Island> GetDockableIslands(int);
		static bool CanDock(int);
		static std::list<Island> GetKnownIslands(int);
		static void SetKnown(int, int, int);
		static std::optional<Island> Read(int, int);
		static const std::string UNKNOWN;
		static void ApplyTurnEffects();
	};
}
