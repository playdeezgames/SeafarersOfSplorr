#pragma once
#include "Common.XY.h"
namespace game::Heading
{
	common::XY<double> DegreesToXY(double);
	double XYToDegrees(const common::XY<double>&);
	double ToDegrees(double);
	double ToRadians(double);
	const double DEGREES = 360.0;
	double Distance(const common::XY<double>&, const common::XY<double>&);
	common::XY<double> ClampDistance(const common::XY<double>&, double);
}
