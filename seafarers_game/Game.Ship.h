#pragma once
#include <Common.XY.h>
#include "Game.ShipType.h"
#include <optional>
#include <string>
namespace game
{
	struct Ship
	{
		ShipType shipType;
		std::string name;
		common::XY<double> location;
		double heading;
		double speed;

		static void ApplyTurnEffects(int);
	};
}
