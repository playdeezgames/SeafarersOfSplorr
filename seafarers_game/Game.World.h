#pragma once
#include "Game.Difficulty.h"
#include "Common.XY.h"
namespace game::World
{
	common::XY<double> GetSize();
	int GetVersion();
	double GetMinimumIslandDistance();
	double GetViewDistance();
	double GetDockDistance();
	void Reset(const game::Difficulty&);
}
