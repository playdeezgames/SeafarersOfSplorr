#pragma once
#include "Game.ShipType.h"
#include "Game.Session.World.ShipType.Commodity.h"
#include <vector>
namespace game::session::world::ship_type
{
	struct Commodities
	{
		constexpr explicit Commodities(const game::ShipType& shipType) : shipType(shipType) {}
		std::vector<Commodity> GetCommodities() const;
	private:
		game::ShipType shipType;
	};
}
