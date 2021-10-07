#pragma once
#include "Game.ShipType.h"
#include <optional>
namespace game
{
	struct Ship
	{
		static std::optional<ShipType> GetShipType(int);
		static int Add(const ShipType&, const std::string&, const common::XY<double>, double, double);
		static std::string GetName();
	};
}
