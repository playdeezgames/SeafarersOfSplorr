#pragma once
#include "Game.ShipType.h"
#include "Game.Session.World.ShipType.Commodity.h"
#include <list>
namespace game::session::world::ship_type
{
	struct Commodities
	{
		constexpr Commodities(const game::ShipType& shipType) : shipType(shipType) {}
		std::list<Commodity> GetCommodities() const;
	private:
		game::ShipType shipType;
	};
}
