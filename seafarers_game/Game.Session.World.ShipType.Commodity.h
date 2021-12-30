#pragma once
#include "Game.ShipType.h"
#include "Game.Commodity.h"
namespace game::session::world::ship_typ
{
	struct Commodity
	{
		constexpr Commodity(
			const game::ShipType& shipType,
			const game::Commodity& commodity)
			: shipType(shipType)
			, commodity(commodity) {}
	private:
		game::ShipType shipType;
		game::Commodity commodity;
	};
}
