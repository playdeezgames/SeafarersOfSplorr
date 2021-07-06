#include "Game.Heading.h"
#include <cmath>
#include "Common.Utility.h"
namespace game::Heading
{
	const double TAU = 2.0 * 3.141592653589793;

	double ToDegrees(double radians)
	{
		return common::Utility::ModuloDouble(radians * DEGREES / TAU, DEGREES);
	}

	double ToRadians(double degrees)
	{
		return degrees * TAU / DEGREES;
	}

	common::XY<double> DegreesToXY(double degrees)
	{
		double radians = ToRadians(degrees);
		return { sin(radians), cos(-radians) };

	}

	double XYToDegrees(const common::XY<double>& xy)
	{
		double x = xy.GetX();
		double y = xy.GetY();
		return (x == 0.0 && y == 0.0) ? (0.0) : (game::Heading::ToDegrees(atan2(x, -y)));
	}

}