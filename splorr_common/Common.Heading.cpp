#include "Common.Data.h"
#include "Common.Utility.h"
#include "Common.Heading.h"
#include <cmath>
namespace common::Heading
{
	const double TAU = 2.0 * 3.141592653589793;

	double ToDegrees(double radians)
	{
		return common::Data::ModuloDouble(radians * DEGREES / TAU, DEGREES).value();
	}

	double ToRadians(double degrees)
	{
		return degrees * TAU / DEGREES;
	}

	const std::vector<std::string> compassPoints =
	{
		"E",
		"ExS",
		"ESE",
		"SExE",
		"SE",
		"SExS",
		"SSE",
		"SxE",
		"S",
		"SxW",
		"SSW",
		"SWxS",
		"SW",
		"SWxW",
		"WSW",
		"WxS",
		"W",
		"WxN",
		"WNW",
		"NWxW",
		"NW",
		"NWxN",
		"NNW",
		"NxW",
		"N",
		"NxE",
		"NNE",
		"NExN",
		"NE",
		"NExE",
		"ENE",
		"ExN"
	};

	const auto COMPASS_POINT_COUNT = compassPoints.size();
	const double DEGREES_PER_COMPASS_POINT = DEGREES / (double)COMPASS_POINT_COUNT;
	const double DEGREES_PER_HALF_COMPASS_POINT = DEGREES_PER_COMPASS_POINT / 2.0;

	const std::string& ToCompassPoint(double degrees)
	{
		return compassPoints[
			(size_t)std::floor(
				ToDegrees(ToRadians(degrees + DEGREES_PER_HALF_COMPASS_POINT))/DEGREES_PER_COMPASS_POINT)];
	}

	common::XY<double> DegreesToXY(double degrees)
	{
		double radians = ToRadians(degrees);
		return { cos(radians), sin(radians) };

	}

	double XYToDegrees(const common::XY<double>& xy)
	{
		double x = xy.GetX();
		double y = xy.GetY();
		return (x == 0.0 && y == 0.0) ? (0.0) : (ToDegrees(atan2(y, x)));
	}

	double XYToRadians(const common::XY<double>& xy)
	{
		return ToRadians(XYToDegrees(xy));
	}


	double Distance(const common::XY<double>& first, const common::XY<double>& second)
	{
		return sqrt((first.GetX() - second.GetX()) * (first.GetX() - second.GetX()) + (first.GetY() - second.GetY()) * (first.GetY() - second.GetY()));
	}

	common::XY<double> ClampDistance(const common::XY<double>& position, double maximum)
	{
		auto distance = Distance(position, { 0.0,0.0 });
		if (distance > maximum)
		{
			return { position.GetX() * maximum / distance, position.GetY() * maximum / distance };
		}
		return position;
	}

	double Difference(double first, double second)
	{
		auto difference = ToDegrees(ToRadians(first - second));
		return (difference < DEGREES / 2.0) ? (difference) : (DEGREES - difference);
	}
}