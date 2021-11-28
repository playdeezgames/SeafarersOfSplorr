#pragma once
#include <Common.XY.h>
#include "Game.ShipType.h"
#include <optional>
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

		static std::optional<ShipType> GetShipType(int);
		static std::optional<std::string> GetName(int);
		static void SetName(int, const std::string&);
		static std::optional<double> GetHeading(int);
		static std::optional<double> GetSpeed(int);
		static std::optional<common::XY<double>> GetLocation(int);
		static void ApplyTurnEffects(int);

		static void SetHeading(int, double);
		static void SetSpeed(int, double);
	};
}
