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
	double GetWindHeading();
	void SetWindHeading(double);
	void Reset(const game::Difficulty&);
}
