#pragma once
#include "Common.XY.h"
#include "Game.Difficulty.h"
namespace game::avatar::AtSea
{
	void Reset(const game::Difficulty&);

	double GetHeading();
	void SetHeading(double);

	double GetSpeed();
	void SetSpeed(double);

	common::XY<double> GetLocation();
}
