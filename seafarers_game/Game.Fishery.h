#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
namespace game
{
	struct Fishery
	{
		int fisheryId;
		common::XY<double> location;
		double radius;
		Fish fish;
		size_t stock;
		size_t depletion;
	};
}
