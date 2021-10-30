#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Island.h"
#include <list>
#include <optional>
#include <string>
namespace game//20211011
{
	struct Islands
	{
		static void Reset(const game::Difficulty&);
		static std::list<Island> GetViewableIslands();
		static std::list<Island> GetDockableIslands();
		static bool CanDock();
		static std::list<Island> GetKnownIslands(const common::XY<double>&);
		static void AddVisit(const common::XY<double>&, const int&);//location and turn
		static void SetKnown(const common::XY<double>&, const int&);//location and turn
		static std::optional<Island> Read(const common::XY<double>&);
		static const std::string UNKNOWN;
		static void ApplyTurnEffects();
	};
}
