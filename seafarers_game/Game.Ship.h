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

		static int Add(const Ship&);

		static std::optional<ShipType> GetShipType(int);
		static std::optional<std::string> GetName(int);
		static std::optional<double> GetHeading(int);
		static std::optional<double> GetSpeed(int);
		static std::optional<common::XY<double>> GetLocation(int);

		static void SetHeading(double);
		static void SetSpeed(double);
		enum class MoveResult
		{
			MOVED,
			CLAMPED
		};
		static MoveResult Move();
	};
}
