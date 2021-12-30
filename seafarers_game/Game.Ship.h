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

		static int Add(const Ship&);

		static std::optional<std::string> GetName(int);
		static void SetName(int, const std::string&);
		static std::optional<common::XY<double>> GetLocation(int);
		static void ApplyTurnEffects(int);
	};
}
