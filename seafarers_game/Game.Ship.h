#pragma once
#include <Common.XY.h>
#include "Game.ShipType.h"
#include <optional>
#include <vector>
namespace game//20211013
{
	struct Ship
	{
		ShipType shipType;
		std::string name;
		common::XY<double> location;
		double heading;
		double speed;

		static std::optional<ShipType> GetShipType(int);
		static int Add(const Ship&);
		static std::string GetName();
		static double GetHeading();
		static void SetHeading(double);
		static double GetSpeed();
		static void SetSpeed(double);
		static common::XY<double> GetLocation();
		enum class MoveResult
		{
			MOVED,
			CLAMPED
		};
		static MoveResult Move();
	};
}
