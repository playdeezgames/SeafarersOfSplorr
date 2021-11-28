#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Island.h"
#include <list>
#include <optional>
#include <string>
namespace game
{
	struct Islands
	{
		static void Reset(const game::Difficulty&);
		static std::list<Island> GetViewableIslands(int);
		static std::list<Island> GetDockableIslands(int);
		static bool CanDock(int);
		static std::list<Island> GetKnownIslands(const common::XY<double>&);
		static void AddVisit(int, int);//location and turn
		static void SetKnown(int, int);//location and turn
		static std::optional<Island> Read(const common::XY<double>&);
		static const std::string UNKNOWN;
		static void ApplyTurnEffects();
	};
}
