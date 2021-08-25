#pragma once
#include <Common.XY.h>
namespace data::game
{
	struct Merchant
	{
		common::XY<double> location;
		common::XY<double> destination;
		static void Add(const Merchant&);
	};
}