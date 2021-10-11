#pragma once
#include "Game.ShipType.h"
#include <optional>
#include <vector>
namespace game
{
	struct Ship
	{
		static std::optional<ShipType> GetShipType(int);
		static int Add(const ShipType&, const std::string&, const common::XY<double>, double, double);//TODO: yuck!
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
		static std::vector<int> GetCrew();//TODO: ????
	};
}
