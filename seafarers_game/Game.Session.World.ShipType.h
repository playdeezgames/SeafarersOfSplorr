#pragma once
#include "Game.ShipType.h"
namespace game::session::world
{
	struct ShipType
	{
		constexpr explicit ShipType(const game::ShipType& shipType) : shipType(shipType) {}
		double GetSpeedFactor() const;
	private:
		game::ShipType shipType;
	};
}
