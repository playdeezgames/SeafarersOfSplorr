#pragma once
#include <Common.XY.h>
namespace data::game
{
	struct Fishery
	{
		int fisheryId;
		int fishType;
		common::XY<double> location;
		common::XY<double> movement;
		double radius;

		static void Clear();
		static void Add(const Fishery&);
	};
}
