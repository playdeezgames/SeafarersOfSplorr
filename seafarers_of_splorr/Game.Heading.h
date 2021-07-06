#pragma once
#include "Common.XY.h"
namespace game::Heading
{
	common::XY<double> DegreesToXY(double);
	double XYToDegrees(const common::XY<double>&);
	double ToDegrees(double);
	double ToRadians(double);
	const double DEGREES = 360.0;

}
