#pragma once
#include "Common.XY.h"
namespace game::Avatar
{
	common::XY<double> GetLocation();
	double GetDirection();
	void SetDirection(double);
	double GetSpeed();
	void SetSpeed(double);
}
