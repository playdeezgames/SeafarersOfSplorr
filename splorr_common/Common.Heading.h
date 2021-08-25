#pragma once
#include "Common.XY.h"
#include <string>
namespace common::Heading
{
	common::XY<double> DegreesToXY(double);
	double XYToDegrees(const common::XY<double>&);
	double XYToRadians(const common::XY<double>&);
	double ToDegrees(double);
	double ToRadians(double);
	const std::string& ToCompassPoint(double);
	const double DEGREES = 360.0;
	double Distance(const common::XY<double>&, const common::XY<double>&);
	common::XY<double> ClampDistance(const common::XY<double>&, double);
	double Difference(double, double);
}
