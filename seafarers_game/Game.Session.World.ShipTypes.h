#pragma once
#include "Game.Session.World.ShipType.h"
#include <vector>
namespace game::session::world
{
	struct ShipTypes
	{
		constexpr ShipTypes() {}
		constexpr ShipType GetShipType(const game::ShipType& shipType) const { return ShipType(shipType); }
		std::vector<ShipType> GetAll() const;
		ShipType Generate() const;//static game::ShipType GenerateForAvatar();
	};
}
